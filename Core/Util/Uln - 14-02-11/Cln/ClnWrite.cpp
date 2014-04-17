/*******************
 *** LnWrite.c   ***
 *******************/

#include	<string.h>

#include	"../PlnType.h"

#include	"ClnType.h"


static void	pln_write( pln_type *pl, FILE *fp );



plnA_type *
cln_to_plnA( cln_type *cln )
{
	plnA_type *apl;
	int	i;

	apl = plnA_alloc( cln->nA );

	for( i = 0 ; i < cln->nA ; i++ )
		apl->a[apl->nA++] = cln->a[i];


	return( apl );
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
	plnA_type *apl = cln_to_plnA( cln );

	plnA_write( apl, file );

	apl->nA = 0;

	plnA_destroy( apl );

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