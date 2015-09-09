/*******************
 *** LnWrite.c   ***
 *******************/

#include	<string.h>

#include	"PlnType.h"


//static void	pln_write( pln_type *pl, FILE *fp );


static int	plnA_write( plnA_type *apl, FILE *fp );


	
void	
plnF_dump( plnF_type *fpl, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".plf", file ) < 0 )
		return;


	plnF_write( fpl, file );
}



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
		if( fpl->a[i] != NULL && fpl->a[i]->nA > 0 )
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
	if( (pl->color[0].val & 0xff0000) != 0 )
		type = 0x02;

	if( pl->group > 0 )
		type |= 0x04;

	if( pl->height != 0 || pl->size != 0 || pl->qulity != 0 )
		type |= 0x08;

	fprintf( fp, "polylink\n" );


	fprintf( fp, "%d   %d   %d", no, pl->state, type );
	if( type & 0x02 )
		fprintf( fp, "  0x%X   0x%X", pl->color[0], pl->color[1] );

	if( type & 0x04 )
		fprintf( fp, "  %d", pl->group );


	if( type & 0x08 )
		fprintf( fp, "  %.4f  %.4f   %.4f", pl->qulity, pl->height, pl->size );


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



