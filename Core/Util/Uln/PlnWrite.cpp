/*******************
 *** LnWrite.c   ***
 *******************/

#include	<string.h>

#include	"PlnType.h"


//static void	pln_write( pln_type *pl, FILE *fp );

static int	plnA_read( plnA_type **apl, FILE*fp );


static int	plnA_write( plnA_type *apl, FILE *fp );


	




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





//#define	PL_VERSION	2


int
plnF_write( plnF_type *fpl, char *file )
{
	FILE	*fp;
	int	i;


	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );


	fprintf(fp, "%s  %d  %d\n", "PLV", PL_VERSION, fpl->nA );
	
	for( i = 0 ; i < fpl->nA ; i++ ){
		if( fpl->a[i] != NULL )
			plnA_write( fpl->a[i], fp );
	}


	fclose( fp );

	return( 1 );

}



int
plnA_write( plnA_type *apl, char *file )
{
	FILE	*fp;



	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );

	plnA_write( apl, fp );


	fclose( fp );

	return( 1 );

}

static int
plnA_write( plnA_type *apl, FILE *fp )
{
	int	i;


	fprintf( fp, "%s  %d  %d   %d\n", "PL", PL_VERSION, apl->iFrame, apl->nA );

	fprintf( fp, "AXIS  %f %f %f %f\n", apl->p.x, apl->p.y, apl->scale, apl->angle );


	for( i = 0 ; i < apl->nA ; i++ )
		pln_write( apl->a[i], fp );


	return( 1 );

}


void
pln_write(pln_type *pl, FILE *fp )
{
	ln_type	*l;
	int	no;




	no = lnL_no( pl->link );

	int	type = 0;
	if( (pl->color[0] & 0xff0000) != 0 )
		type = 0x02;

	if( pl->group > 0 )
		type |= 0x04;

	fprintf( fp, "polylink\n" );


	fprintf( fp, "%d   %d   %d", no, pl->state, type );
	if( type & 0x02 )
		fprintf( fp, "  0x%X   0x%X", pl->color[0], pl->color[1] );

	if( type & 0x04 )
		fprintf( fp, "  %d", pl->group );

	fprintf( fp, "\n" );




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



#ifdef _AA_
static pln_type *	pln_read( FILE *fp );

int
plnF_read( plnF_type **vpl, char *file )
{
FILE	*fp;
int	i,	version,	nV;
char	signature[64];


	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	if( fscanf(fp, "%s  %d", signature, &version ) != 2 ){
		fclose( fp );
		return( NULL );
	}


	if( strcmp( signature, "PLV") != 0 ){
		fclose( fp );
		if( strcmp( signature, "PL") != 0 ){
			plnA_type	*apl;
			plnA_read( file, &apl );
			*vpl = plnF_alloc( 1 );
			plnF_add( *vpl, apl, apl->iFrame );

			return( 1 );
		}

		return( NULL );
	}

	fscanf(fp, "%d", &nV );

	*vpl = plnF_alloc( nV );


	plnA_type *apl;
	for( i = 0 ; i < nV ; i++ ){
		plnA_read( &apl, fp );

		plnF_add( *vpl, apl, apl->iFrame );
	}

	fclose( fp );

	return( 1 );
}


int
plnA_read( char *file, plnA_type **apl )
{
	FILE	*fp;

	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	int ret = plnA_read( apl, fp );


	fclose( fp );

	return( ret );

}


static int
plnA_read( plnA_type **apl, FILE *fp )
{
	int	i,	version,	nPl;
	char	signature[64];
	pln_type	*pl;



	if( fscanf(fp, "%s  %d", signature, &version ) != 2 )
		return( NULL );

	if( strcmp( signature, "PL") != 0 )
		return( NULL );

	int iFrame = 0;
	if( version >=2 )
		fscanf(fp, "%d", &iFrame );
	
	fscanf(fp, "%d", &nPl );


	*apl = plnA_alloc( nPl+10 );

	(*apl)->iFrame = iFrame;

	for( i = 0 ; i < nPl ; i++ ){
		pl = pln_read( fp );

		(*apl)->a[(*apl)->nA++ ] = pl;
	}


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