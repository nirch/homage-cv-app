/*******************
 *** LnWrite.c   ***
 *******************/

#include	<string.h>

#include	"PlnType.h"


static void	pln_write( pln_type *pl, FILE *fp );




	




void	
plnA_dump( plnA_type *apl, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".pl", file ) < 0 )
		return;


	plnA_write( apl, file );
}


void	
pln_dump( pln_type *pl, char *prefix, int index, char *suffix )
{
plnA_type *apl;

	apl = plnA_alloc( 1 );
	apl->a[0] = pl;
	apl->nA = 1;

	plnA_dump( apl, prefix, index, suffix );

	apl->nA = 0;

	plnA_destroy( apl );
}



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