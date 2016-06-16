/*************************
 ***   HeadPose.cpp   ***
 *************************/
#include	<math.h>
#include	 <string.h>
#include	"Uigp/igp.h"

#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif
#include "Uigp/GpMemoryLeak.h"
#include "Umath/EigenType.h"
#include "Umath/Ellipse/EllipseType.h"


#ifdef _DEBUG
#define _DUMP
#endif

#include "UMath/St/StType.h"
#include "Ucamera/Pt2dType.h"

#include "Uln/PlnType.h"

#include	"HeadPose.h"


#define HEAD_VERSION 0x01

//static void	headPose_write( headPose_type *pose, FILE *fp );
static headPose_type *	headPose_readA( FILE *fp, int version );


headPose_type *
headPose_alloc()
{

	headPose_type *f = (headPose_type*)malloc( sizeof(headPose_type) );

	f->p.x = f->p.y = 0;
	f->angle = 0;
	f->scale = 0;

	f->iFrame = 0;

	f->group = 0;
	f->smooth = 0;


	f->qulity = 0;



	

	return( f );
}



void
headPose_destroy( headPose_type *pose )
{
	free( pose );
}

headPose_type *
headPose_copy( headPose_type *pose, headPose_type *f )
{

	if( f == NULL)
		f = headPose_alloc();

	*f = *pose;

	return( f );
}


headPoseA_type *
headPoseA_alloc( int no )
{
headPoseA_type *af;
int	i;

	af = (headPoseA_type*)malloc( sizeof(headPoseA_type) );

	af->NA = no;
	af->a = (headPose_type **)malloc( af->NA * sizeof(headPose_type *) );

	for( i = 0 ; i < af->NA ; i++ )
		af->a[i] = NULL;

	af->nA = 0;



	GPMEMORY_LEAK_ALLOC( af );


	return( af );
}


headPoseA_type *
headPoseA_realloc( headPoseA_type *ah, int no )
{


	if( ah == NULL ){
		ah = headPoseA_alloc( no );
		return( ah );
	}

	int i0 = ah->NA;
	ah->NA = no;
	ah->a = ( headPose_type **)realloc( ah->a, ah->NA * sizeof( headPose_type*) );

	int	i;
	for( i = i0 ; i < ah->NA ; i++ )
		ah->a[i] = NULL;

	return( ah );
}

void 
headPoseA_destroy( headPoseA_type *ah, int fData )
{	
	if( fData == 1)
		headPoseA_clear( ah );

	free( ah->a );
	free( ah );
}


void 
headPoseA_add( headPoseA_type *ah, headPose_type *h, int iFrame )
{	

	if( iFrame >= ah->NA )
		headPoseA_realloc( ah, iFrame+32 );

	if( ah->a[iFrame] != NULL )
		headPose_destroy( ah->a[iFrame] );

	ah->a[iFrame] = h;

	if( iFrame >= ah->nA )
		ah->nA = iFrame+1;
}



headPoseB_type *
headPoseB_alloc( int no )
{
	headPoseB_type *af;
	int	i;

	af = (headPoseB_type*)malloc( sizeof(headPoseB_type) );

	af->NA = no;
	af->a = (headPoseA_type **)malloc( af->NA * sizeof(headPoseA_type *) );

	for( i = 0 ; i < af->NA ; i++ )
		af->a[i] = NULL;

	af->nA = 0;
	return( af );
}


headPoseB_type *
	headPoseB_realloc( headPoseB_type *ah, int no )
{


	if( ah == NULL ){
		ah = headPoseB_alloc( no );
		return( ah );
	}

	int i0 = ah->NA;
	ah->NA = no;
	ah->a = ( headPoseA_type **)realloc( ah->a, ah->NA * sizeof( headPoseA_type*) );

	int	i;
	for( i = i0 ; i < ah->NA ; i++ )
		ah->a[i] = NULL;

	return( ah );
}


void 
headPoseB_destroy( headPoseB_type *af, int fData )
{	
int	i;

	for( i = 0 ; i < af->nA ; i++ )
		headPoseA_destroy( af->a[i], fData );

	free( af->a );
	free( af );
}

void 
	headPoseB_decrease( headPoseB_type *bh )
{	
	int	i,	j;

	for( i = 0, j = 0 ; i < bh->nA ; i++ ){
		if( bh->a[i] == NULL )
			continue;
		bh->a[j++] = bh->a[i];
	}

	bh->nA = j;
}

//void 
//headPoseB_add( headPoseB_type *af, headPoseA_type *ah, int iFrame )
//{	
//
//	if( af->NA >= iFrame )
//		headPoseB_realloc( af, iFrame+32 );
//
//
//	if( af->a[iFrame] != NULL )
//		headPoseA_destroy( af->a[iFrame] );
//
//	af->a[iFrame] = ah;
//}

void 
headPoseB_add( headPoseB_type *af, headPoseA_type *ah, int i0 )
{	

	if( i0 < 0 )
		i0 = af->nA;

	if( i0 >= af->NA )
		headPoseB_realloc( af, i0+32 );

	if( af->a[i0] != NULL )
			headPoseA_destroy( af->a[i0] );

	af->a[i0] = ah;

	if( i0 >= af->nA )
		af->nA = i0 + 1;
}


headPoseB_type * 
headPoseB_create(  headPoseA_type *ah )
{	
headPoseB_type *bh;
int	i,	nGroup,	nFrame;


	nGroup = nFrame = 0;
	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];

		if( nGroup < h->group )	nGroup = h->group;
		if( nFrame < h->iFrame )	nFrame = h->iFrame;

	}

	nGroup++;
	nFrame++;

	bh = headPoseB_alloc( nGroup );
	bh->nA = nGroup;


	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];
		if( h->group < 0 )	continue;

		if( bh->a[h->group] == NULL )
			bh->a[h->group] = headPoseA_alloc( nFrame );


		headPoseA_type *tah = bh->a[h->group];

		tah->a[h->iFrame] = h;
		if( tah->nA <= h->iFrame )
			tah->nA = h->iFrame+1;

	}

	return( bh );
}


headPoseB_type * 
	headPoseB_createG(  headPoseA_type *ah )
{	
	headPoseB_type *bh;
	int	i,	nGroup,	nFrame;


	nGroup = nFrame = 0;
	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];

		if( nGroup < h->group )	nGroup = h->group;
		if( nFrame < h->iFrame )	nFrame = h->iFrame;

	}

	nGroup++;
	nFrame++;

	bh = headPoseB_alloc( nGroup );
	bh->nA = nGroup;


	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];
		if( h->group < 0 )	continue;

		if( bh->a[h->group] == NULL )
			bh->a[h->group] = headPoseA_alloc( nFrame );


		headPoseA_type *tah = bh->a[h->group];

		tah->a[tah->nA++] = h;
	}

	return( bh );
}


void
headPose_set( headPose_type *hp, int iFrame, int state, float x, float y, float scale, float angle )
{

	hp->iFrame = iFrame;
	hp->state = state;
	hp->p.x = x;
	hp->p.y = y;
	hp->scale = scale;
	hp->angle = angle;

	hp->qulity = 0;
	hp->smooth = 0;
	hp->group = -1;
}

void 
headPoseA_clear( headPoseA_type *af )
{
	int	i;

	for( i = 0 ; i < af->nA ; i++ ){
		if( af->a[i] != NULL ){
			free( af->a[i] );
			af->a[i] = NULL;
		}
	}

	af->nA = 0;
}

void 
headPoseA_clear( headPoseA_type *af, int i0, int i1 )
{
	int	i;

	for( i = i0 ; i < i1 ; i++ ){
		if( af->a[i] != NULL ){
			free( af->a[i] );
			af->a[i] = NULL;
		}
	}
}


int 
headPoseA_clear( headPoseA_type *af, int iFrame )
{
	if( iFrame < 0 || iFrame >= af->nA )
		return( -1 );

	if( af->a[iFrame] != NULL ){
		free( af->a[iFrame] );
		af->a[iFrame] = NULL;
	}

	return( 1 );
}


void 
	headPoseA_aux( headPoseA_type *ah )
{
	int	i;

	st_type	scale,	quality;

	st_clear( &scale, NULL );
	st_clear( &quality, NULL );

	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type	*h = ah->a[i];
		if( h == NULL )	continue;

		st_add( &scale, h->scale );
		st_add( &quality, h->qulity );
	}

	st_final( &scale );
	st_final( &quality );

	ah->scale = scale.av;
	ah->qulity = quality.av;
}

void
headPoseA_icycle( headPoseA_type *af )
{
	int	k;

	headPose_type	*tim = af->a[af->nA-1];
	for( k = af->nA-1 ; k > 0 ;  k-- ){
		af->a[k] = af->a[k-1];
	}

	af->a[0] = tim;
}


headPose_type * 
headPoseA_get( headPoseA_type *af, int iFrame )
{
	if( iFrame < 0 || iFrame >= af->nA )
		return( NULL );

	return( af->a[iFrame] );
}

void 
headPoseA_set( headPoseA_type *af, headPose_type *pose, int iFrame, int fCopy )
{
	if( af == NULL || iFrame < 0 )
		return;

	if( iFrame >= af->NA ){
		headPoseA_realloc( af, af->NA + 64 );
	}

	if( fCopy == 1 && pose != NULL )
		pose = headPose_copy( pose, NULL );

	af->a[iFrame] = pose;
	if( pose != NULL )
		af->a[iFrame]->iFrame = iFrame;

	if( af->nA <= iFrame )
		af->nA = iFrame +1;
}


void 
headPoseA_append( headPoseA_type *ah, headPoseA_type *sah, int fCopy )
{	
int	i;

	if( ah->nA + sah->nA >= ah->NA )
		headPoseA_realloc( ah, ah->nA + sah->nA + 64 );



	for( i = 0 ; i < sah->nA ; i++ ){
		headPose_type *h = sah->a[i];
		
		if( fCopy == 1 )
			h = headPose_copy( h, NULL );
		
		ah->a[ah->nA++] = h;
	}

}


void 
headPoseA_set_iFrame( headPoseA_type *ah, int iFrame )
{	
	int	i;

	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];
		h->iFrame = iFrame;
	}
}



void 
headPoseA_set_group( headPoseA_type *ah, int group )
{	
	int	i;

	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];
		h->group = group;
	}
}

void 
	headPoseA_set_state( headPoseA_type *ah, int state )
{	
	int	i;

	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];
		h->state = state;
	}
}

headPoseA_type *
	headPoseA_merge( headPoseA_type *a[], int nA )
{


	int	i,	n;

	for( i = 0, n = 0 ; i < nA ; i++ ){
		if( a[i] == NULL )	continue;

		n += a[i]->nA;
	}


	headPoseA_type *ah = headPoseA_alloc( n );

	for( i = 0 ; i < nA ; i++ ){
		if( a[i] == NULL )	continue;
		headPoseA_append( ah, a[i], 0 );
	}

	return( ah );
}

int
headPoseA_write( headPoseA_type *af, char *file )
{
FILE	*fp;
int	i;


	if(  (fp = fopen( file, "wb" )) == NULL )
		return( -1 );

	int	nA;
	for( i = 0, nA = 0 ; i < af->nA ; i++ )
		if(  af->a[i] != NULL )	nA++;

	fprintf( fp, "HEAD  %d %d\n", HEAD_VERSION, nA );


	for( i = 0 ; i < af->nA ; i++ ){
		if(  af->a[i] == NULL )	continue;

		headPose_write( af->a[i], fp );
	}


	fclose( fp );

	return ( 1 );
}






void
headPose_write( headPose_type *pose, FILE *fp )
{
	fprintf( fp, "%d\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\t%d\n", pose->iFrame, pose->state, pose->p.x, pose->p.y, pose->scale, pose->angle, pose->qulity, pose->group, pose->smooth );
}



int
headPoseA_read( headPoseA_type **ah, char *file )
{
headPose_type *s;
FILE	*fp;
int	nS,	version;
char siegnture[64];

//	gp_filename_force_extension( file, ".skl2" );

	if(  (fp = fopen( file, "rb" ) ) == NULL )
		return( -1 );


	char buf[256];
	fgets( buf, 1024, fp );

	if( strncmp( buf, "HEAD", 4) != 0 ){
		sscanf( buf, "%d", &nS );
		version = 0;
	}
	else	sscanf( buf, "%s %d %d", siegnture, &version, &nS );
		
		
//	fscanf( fp, "%d", &nS );

	*ah = headPoseA_alloc( nS+64 );



	while( (s = headPose_readA( fp, version )) != NULL ){
		(*ah)->a[ s->iFrame ] = s;

		(*ah)->nA = s->iFrame + 1;
	}


	fclose( fp );

	return( 1 );
}



int
	headPoseA_readG( headPoseA_type **ah, char *file )
{
	headPose_type *s;
	FILE	*fp;
	int	nS,	version;
	char siegnture[64];

	//	gp_filename_force_extension( file, ".skl2" );

	if(  (fp = fopen( file, "rb" ) ) == NULL )
		return( -1 );


	char buf[256];
	fgets( buf, 1024, fp );

	if( strncmp( buf, "HEAD", 4) != 0 ){
		sscanf( buf, "%d", &nS );
		version = 0;
	}
	else	sscanf( buf, "%s %d %d", siegnture, &version, &nS );
//	fscanf( fp, "%d", &nS );

	*ah = headPoseA_alloc( nS+64 );



	while( (s = headPose_readA( fp, version )) != NULL ){
		if( (*ah)->nA == (*ah)->NA )
			headPoseA_realloc( *ah, (*ah)->nA + 64  );
		(*ah)->a[ (*ah)->nA++] = s;
	}


	fclose( fp );

	return( 1 );
}


static headPose_type *
headPose_readA( FILE *fp, int version )
{
	
	headPose_type *pose = headPose_alloc();

//	if( fscanf( fp, "%d  %d  %f  %f  %f   %f %f %d", &pose->iFrame, &pose->state, &pose->p.x, &pose->p.y, &pose->scale, &pose->angle, &pose->qulity, &pose->smooth  ) != 8 )
//		return( NULL );

	if( headPose_read( pose, version, fp ) < 0 ){
		headPose_destroy( pose );
		return( NULL );
	}

		
	return( pose );
}


int 
headPose_read( headPose_type *pose, int version, FILE *fp )
{

	if( version == 0 ){
		if( fscanf( fp, "%d  %d  %f  %f  %f   %f %f %d", &pose->iFrame, &pose->state, &pose->p.x, &pose->p.y, &pose->scale, &pose->angle, &pose->qulity, &pose->smooth  ) != 8 )
			return( -1 );
		return( 1 );
	}

	if( fscanf( fp, "%d  %d  %f  %f  %f   %f %f %d %d", &pose->iFrame, &pose->state, &pose->p.x, &pose->p.y, &pose->scale, &pose->angle, &pose->qulity, &pose->group, &pose->smooth  ) != 9 )
			return( -1 );

	return( 1 );
}


headPoseA_type *
headPoseA_copy( headPoseA_type *ah )
{
	int	i;

	headPoseA_type *as = headPoseA_alloc( ah->nA+1 );

	for( i = 0 ; i < ah->nA ; i++ ){

		ah->a[i] = NULL;

		if( ah->a[i] != NULL )
			as->a[i] = headPose_copy( as->a[i], NULL );

	}

	as->nA = ah->nA;

	return( as );
}


headPose_type *
headPoseA_getG( headPoseA_type *ah, int iGroup, int iFrame )
{
	int	i;

	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];
		if( h->iFrame == iFrame && h->group == iGroup )
			return( h );
	}

	return( NULL );
}


headPoseA_type *
headPoseA_copy_group( headPoseA_type *ah, int group, int fCopy )
{
	int	i;

	headPoseA_type *as = headPoseA_alloc( ah->nA );


	for( i = 0 ; i < ah->nA ; i++ ){

		headPose_type *h = ah->a[i];

		if( h == NULL || h->group != group )
			continue;


		if( fCopy == 1 )
			h = headPose_copy( h, NULL );


		as->a[as->nA++] = h;
	}


	return( as );
}


headPoseA_type *
	headPoseA_copy_groupG( headPoseA_type *ah, int group, int fCopy )
{
	int	i;

	headPoseA_type *as = headPoseA_alloc( ah->nA );
	for( i = 0 ; i < ah->nA ; i++ )
		as->a[i] = NULL;

	for( i = 0 ; i < ah->nA ; i++ ){

		headPose_type *h = ah->a[i];

		if( h == NULL || h->group != group ){
//			as->a[i] = NULL;
			continue;
		}


		if( fCopy == 1 )
			h = headPose_copy( h, NULL );

		if( h->iFrame >= as->NA )
			headPoseA_realloc( as, h->iFrame + 64 );

		as->a[h->iFrame] = h;

		if( as->nA <= h->iFrame )
			as->nA = h->iFrame+1;
	}



	return( as );
}



void
headPoseA_fill( headPoseA_type *af )
{
	int	i;

	for( i = 1; i < af->nA ; i++ ){
		if( af->a[i] == NULL && af->a[i-1] != NULL ){
			af->a[i] = headPose_copy( af->a[i-1], NULL );
			af->a[i]->iFrame += 1 ;
		}
	}
}



void
headPoseA_box( headPoseA_type *af, box2f_type *b )
{
	int	i,	k;

	headPoseA_type *as = headPoseA_alloc( af->nA+1 );


	for( i = 0, k = 0 ; i < af->nA ; i++ ){
		headPose_type *f = af->a[i];
		if( f == NULL )	continue;


		if( k == 0 ){
			BOX2D_INIT( *b, f->p.x, f->p.y );
		}
		else {
			BOX2D_UPDATE( *b, f->p.x, f->p.y );
		}

		k++;
	}
}


int 
headPoseA_decrease( headPoseA_type *ah )
{
int	i,	j;

	for( i = 0, j = 0 ; i < ah->nA ; i++ ){
		if( ah->a[i] == NULL )
			continue;
		
		ah->a[j++] = ah->a[i];
	}

	ah->nA = j;

	return( ah->nA );
}



int
	headPoseA_insert( headPoseA_type *bah, headPoseA_type *ah )
{
	int i;

	for( i = ah->nA-1 ; i >= 0 ; i-- ){

		headPoseA_insert( bah, ah->a[i] );
	}

	return( 1 );
}

int
	headPoseA_insert( headPoseA_type *ah, headPose_type *h )
{
	int i;

	for( i = ah->nA-1 ; i >= 0 ; i-- ){
		if( ah->a[i]->iFrame < h->iFrame )
			break;

		ah->a[i+1] = ah->a[i];
	}

	ah->a[i+1] = h;

	ah->nA++;

	return( 1 );
}


int headPoseA_frame_index( headPoseA_type *ah, int i0, int iFrame )
{
	int	i;
	for( i = i0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];

		if( iFrame <= h->iFrame )
			return( i );
	}

	return( -1 );

}



plnA_type *	headPoseA_to_plnA(  headPoseA_type *ah, pln_type *bpl )
{
int	i;

	plnA_type *apl = plnA_alloc( ah->nA );

	for( i = 0 ; i < ah->nA ; i++ ){
		apl->a[apl->nA++] = headPose_pl( ah->a[i], bpl );
	}

	return( apl );
}

plnF_type *	headPoseA_to_plnF(  headPoseA_type *ah, pln_type *bpl )
{
	int	i;

	plnF_type *fpl = plnF_alloc( ah->nA );

	//plnA_type *apl = plnA_alloc( ah->nA );

	for( i = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];
		plnA_type *apl = plnF_get( fpl, h->iFrame );

		if( apl == NULL ){
			apl = plnA_alloc( 36 );
			plnF_add( fpl, apl, h->iFrame);
		}
		
		
		pln_type *pl = headPose_pl( ah->a[i], bpl );
		plnA_add( apl, pl );
		//apl->a[apl->nA++] = headPose_pl( ah->a[i], bpl );
	}

	return( fpl );
}


pln_type *	headPose_pl(  headPose_type *h, pln_type *bpl  )
{
	pln_type *pl;
	lt2_type	lt;

	lt2_similarity_set( &lt, h->p.x, h->p.y, h->angle, h->scale );

	pl =  pln_affine_lt( bpl, &lt, NULL );

	return( pl );
}



void
headPoseA_dump_pl( headPoseA_type *ah, pln_type *bpl, char *prefix, int index, char *suffix )
{

	plnA_type *apl = headPoseA_to_plnA( ah, bpl );

	PLNA_DUMP( apl, prefix, index, suffix );

	plnA_destroy( apl );
}


void
	headPoseA_dump_plF( headPoseA_type *ah, pln_type *bpl, char *prefix, int index, char *suffix )
{

	plnF_type *fpl = headPoseA_to_plnF( ah, bpl );

	PLNF_DUMP( fpl, prefix, index, suffix );

	plnF_destroy( fpl );
}

void
	headPoseA_dump( headPoseA_type *ag[], int nG, pln_type *hpl, char *prefix, int index, char *suffix )
{

	headPoseA_type *ah = headPoseA_merge( ag, nG );

	HEADPOSEA_DUMP( ah, prefix, index, suffix );
	HEADPOSEA_DUMP_PLF( ah, hpl, prefix, index, suffix );

	ah->nA = 0;
	headPoseA_destroy( ah );
}


void
	headPoseB_dump( headPoseB_type *bh, pln_type *hpl, char *prefix, int index, char *suffix )
{

	headPoseA_type *ah = headPoseA_merge( bh->a, bh->nA );

	HEADPOSEA_DUMP( ah, prefix, index, suffix );
	HEADPOSEA_DUMP_PLF( ah, hpl, prefix, index, suffix );

	ah->nA = 0;
	headPoseA_destroy( ah );
}


void
headPoseA_dump( headPoseA_type *ah, char *prefix, int index, char *suffix )
{
char	file[256];

	gpDump_filename( prefix, index, suffix, ".txt", file );

	headPoseA_write( ah, file );
}



float 
	headPoseA_qulity( headPoseA_type *ah )
{
	int	i;

	float sum = 0;

	for( i = 0 ; i < ah->nA ; i++ ){
		sum  += ah->a[i]->qulity;
	}

	sum /= ah->nA;

	return( sum );
}


int 
	headPoseA_ellipse( headPoseA_type *ah, ellipse_type *e )
{
	int	i;

	eigen2dAux_type	a;
	eigen2dAux_init( &a, 1 );

	for( i = 0 ; i < ah->nA ; i++ ){

		headPose_type *h = ah->a[i];

		EIGEN2D_UPDATE( a, h->p.x, h->p.y );
	}


	eigen2d_type eg;
	eigen2dAux_final( &a, &eg );

	ellipse_set( e, &eg );

	return( 1 );

}