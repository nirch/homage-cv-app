/*************************
 ***   HeadPose.cpp   ***
 *************************/
#include	<math.h>
#include	"Uigp/igp.h"
#include "Ucamera/Pt2dType.h"

#include	"HeadPose.h"


//static void	headPose_write( headPose_type *pose, FILE *fp );
static headPose_type *	headPose_readA( FILE *fp );


headPose_type *
headPose_alloc()
{

	headPose_type *f = (headPose_type*)malloc( sizeof(headPose_type) );

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
headPoseA_destroy( headPoseA_type *af )
{	
	headPoseA_clear( af );

	free( af->a );
	free( af );
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

	fprintf( fp, "%d\n", nA );


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
	fprintf( fp, "%d\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\n", pose->iFrame, pose->state, pose->p.x, pose->p.y, pose->scale, pose->angle, pose->qulity, pose->smooth );
}



int
headPoseA_read( headPoseA_type **ah, char *file )
{
headPose_type *s;
FILE	*fp;
int	nS;

//	gp_filename_force_extension( file, ".skl2" );

	if(  (fp = fopen( file, "rb" ) ) == NULL )
		return( -1 );


	fscanf( fp, "%d", &nS );

	*ah = headPoseA_alloc( nS+64 );



	while( (s = headPose_readA( fp )) != NULL ){
		(*ah)->a[ s->iFrame ] = s;

		(*ah)->nA = s->iFrame + 1;
	}


	fclose( fp );

	return( 1 );
}


static headPose_type *
headPose_readA( FILE *fp )
{
	
	headPose_type *pose = headPose_alloc();

//	if( fscanf( fp, "%d  %d  %f  %f  %f   %f %f %d", &pose->iFrame, &pose->state, &pose->p.x, &pose->p.y, &pose->scale, &pose->angle, &pose->qulity, &pose->smooth  ) != 8 )
//		return( NULL );

	if( headPose_read( pose, fp ) < 0 ){
		headPose_destroy( pose );
		return( NULL );
	}

		
	return( pose );
}


int 
headPose_read( headPose_type *pose, FILE *fp )
{

	if( fscanf( fp, "%d  %d  %f  %f  %f   %f %f %d", &pose->iFrame, &pose->state, &pose->p.x, &pose->p.y, &pose->scale, &pose->angle, &pose->qulity, &pose->smooth  ) != 8 )
		return( -1 );



	return( 1 );
}

headPoseA_type *
headPoseA_copy( headPoseA_type *af )
{
	int	i;

	headPoseA_type *as = headPoseA_alloc( af->nA+1 );


	for( i = 0 ; i < af->nA ; i++ ){
		af->a[i] = NULL;

		if( af->a[i] != NULL )
			af->a[i] = headPose_copy( af->a[i], NULL );

	}

	af->nA = af->nA;

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