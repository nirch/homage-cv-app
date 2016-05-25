/*******************
 *** LnWrite.c   ***
 *******************/

#include	<string.h>

#include	"../PlnType.h"

#include	"ClnType.h"


//static void	pln_write( pln_type *pl, FILE *fp );



plnA_type *
cln_to_plnA( cln_type *cln, int fData )
{
	plnA_type *apl;
	int	i;

	apl = plnA_alloc( cln->nA );

	apl->p = cln->p;
	apl->angle = cln->angle;
	apl->scale = cln->scale;

	for( i = 0 ; i < cln->nA ; i++ ){
		if( fData == 1 )
			apl->a[apl->nA++] = pln_copy( cln->a[i] );
		else apl->a[apl->nA++] = cln->a[i];
	}


	return( apl );
}
	

cln_type *
cln_from_plnA( plnA_type *apl, int fData )
{ 
	int	i;

	cln_type *cln = cln_alloc();

	cln->p = apl->p;
	cln->scale = apl->scale;
	cln->angle = apl->angle;
	cln->iFrame = apl->iFrame;
	
	for( i = 0 ; i < apl->nA ; i++ ){
		if( fData == 1 )
			cln->a[cln->nA++] = pln_copy( apl->a[i] );
		else cln->a[cln->nA++] = apl->a[i];
	}


	return( cln );
}

	
void	
clnA_dump( clnA_type *ac, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( ac == NULL )
		return;


	if( gpDump_filename( prefix, index, suffix, ".ctr", file ) < 0 )
		return;




	clnA_write( ac, file );
}


void	
cln_dump( cln_type *cln, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( cln == NULL )
		return;

	
	if( gpDump_filename( prefix, index, suffix, ".pl", file ) < 0 )
		return;




	cln_write( cln, file );
}




int
cln_write(cln_type *cln, char *file )
{
	plnA_type *apl = cln_to_plnA( cln, 0 );

	plnA_write( apl, file );

	apl->nA = 0;
	plnA_destroy( apl );

	return( 1 );
}


int
cln_read(cln_type **cln, char *file )
{
	plnA_type *apl;

	if( plnA_read( &apl, file ) < 0 )
		return( -1 );

	*cln = cln_from_plnA( apl, 0 );


	apl->nA = 0;
	plnA_destroy( apl );

	return( 1 );
}


static int	cln_read( cln_type **cl, FILE *fp );


int
clnA_read( clnA_type **acl, char *file )
{
	FILE	*fp;
	int	version,	nV;
	char	signature[64];
	int	pose;


	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	pose = ftell( fp );
	if( fscanf(fp, "%s  %d  %d", signature, &version, &nV ) != 3 ){
		fclose( fp );
		return( -1 );
	}
	
	
	if( strcmp( signature, "CL") != 0 ){

		fclose( fp );

		plnA_type *apl;
		if( plnA_read( &apl, file ) < 0 )
			return( -1 );

		*acl = clnA_alloc( 1 );

		(*acl)->a[(*acl)->nA++] = cln_from_plnA( apl, 0 );


		apl->nA = 0;
		plnA_destroy( apl );

		return( 1 );
	}


	
	*acl = clnA_alloc( nV );

	cln_type *cl;
	while( cln_read( &cl, fp) > 0  ){
		if( version == 1 )
			cln_unnormelize( cl );
		(*acl)->a[(*acl)->nA++] = cl;
	}




	fclose( fp );

	return( 1 );
}



static int
cln_read( cln_type **cl, FILE *fp )
{
	pln_type	*pl;


	*cl = cln_alloc();

	int iFrame = 0;



#ifdef _AA_
	contour
		0
		transform
		416.389282  667.000000  1.000000   0.000000
#endif
	char	buf[256];
	(*cl)->iFrame = iFrame;

	if( fscanf(fp, "%s  %d", buf, &(*cl)->type ) != 2 )
		return( -1 );
	if( gp_strnicmp(buf , "contour", 7) != 0 )
		return( -1 );
	fscanf( fp, "%s  %f %f %f %f", buf, &(*cl)->p.x, &(*cl)->p.y, &(*cl)->scale, &(*cl)->angle );
	(*cl)->angle = ANGLE_D2R( (*cl)->angle );



	int pose = ftell(fp);
	int	ret;
	while( ( ret = pln_read( &pl, fp ))  > 0 ){
		(*cl)->a[ (*cl)->nA++ ] = pl;
		pose = ftell(fp);
	}


	fseek( fp, pose, SEEK_SET );
	return( 1 );	
}



static int	cln_write( cln_type *cl, FILE *fp );


int
clnA_write( clnA_type *acl, char *file )
{
	FILE	*fp;
	int i;


	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );


	fprintf(fp, "%s  %d  %d\n", "CL", CL_VERSION, acl->nA );


	for( i = 0 ; i < acl->nA ; i++ )
		cln_write( acl->a[i], fp );

	fclose( fp );

	return( 1 );

}

static int
cln_write( cln_type *cl, FILE *fp )
{


	//contour
	//0
	//transform
	//416.389282  667.000000  1.000000   0.000000

	fprintf(fp, "contour  %d\n", cl->type );

	fprintf( fp, "transform\n" );
	fprintf( fp, "%f %f %f %f\n", cl->p.x, cl->p.y, cl->scale, ANGLE_R2D( cl->angle ) );
	




	int	i = 0;
	for( i = 0 ; i < cl->nA ; i++ )
		pln_write( cl->a[i], fp );

	return( 1 );	
}





#ifdef _AA_

#define	PL_VERSION	1

int
plnA_write( plnA_type *apl, char *file )
{
	FILE	*fp;
	int	i;


	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );


	fprintf(fp, "%s  %d  %d\n", "PL", PL_VERSION, apl->nA );



//	fprintf( fp, "transform\n" );
//	fprintf( fp, "%.6f  %.6f  %.6f\n", 0.0,0.0, 1.0 );

	for( i = 0 ; i < apl->nA ; i++ ){
		pln_write( apl->a[i], fp );
	}

	fclose( fp );

	return( 1 );

}

static void
pln_write(pln_type *pl, FILE *fp )
{
	ln_type	*l;
	int	no;




	no = lnL_no( pl->link );

	fprintf( fp, "polylink\n" );
	fprintf( fp, "%d   %d   %d\n", no, pl->state, 0 );
	fprintf( fp, "%.6f  %.6f\n", pl->ctr.x, pl->ctr.y );




	for( l = pl->link ;  l != NULL ; l = LN_NEXT(l) )
		fprintf( fp, "%.6f  %.6f  %.6f\n", l->v.x, l->v.y, l->a );
}


int
pln_write_to_file( pln_type *pl, char *file )
{
plnA_type *apl;
int	ret;

	apl = plnA_alloc( 1 );
	apl->a[0] = pl;
	apl->nA = 1;


	ret = plnA_write( apl, file );


	apl->nA = 0;
	plnA_destroy( apl );

	return( ret );
}




static pln_type *	pln_read( FILE *fp );

int
plnA_read( char *file, plnA_type **apl )
{
	FILE	*fp;
	int	i,	version,	nPl;
	char	signature[64];
	pln_type	*pl;

	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	fscanf(fp, "%s  %d", signature, &version );

	fscanf(fp, "%d", &nPl );


	*apl = plnA_alloc( nPl+10 );

	for( i = 0 ; i < nPl ; i++ ){
		pl = pln_read( fp );

		(*apl)->a[(*apl)->nA++ ] = pl;
	}

	fclose( fp );

	return( 1 );

}



static pln_type *
pln_read( FILE *fp )
{
pln_type *pl;
	ln_type	*l;
	int	no,	type;
	char	line[512];

	while( 1 ){
		if( fgets( line, 512, fp ) == NULL )
			return( NULL );
		if( gp_strnicmp(line , "polylink", 8) == 0 )
			break;
	}



	fscanf( fp, "%d", &no );

	pl = pln_alloc( no );



	fscanf( fp, "%d   %d", &pl->state, &type );

	fscanf( fp, "%f  %f", &pl->ctr.x, &pl->ctr.y );

	for( l = pl->link ; l != NULL ; l = LN_NEXT(l) ){
		fscanf( fp, "%f  %f  %f", &l->v.x, &l->v.y, &l->a );
		ln_set_aux( l );
	}

	pl->len = lnL_length( pl->link );

	return( pl );
}

#endif