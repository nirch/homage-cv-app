/*********************
 ***   LsfTool.h   ***
 *********************/
#include <stdio.h>
#include <math.h>

#include "Uigp/igp.h"
#include "LsfType.h"



lsf_type *
lsf_alloc( int n )
{
lsf_type	*m;

	m = ( lsf_type *)malloc( sizeof( lsf_type ) + sizeof( lsfB_type )* n );

	m->nL = n;
	m->NL = n;

	m->f = (lsfB_type *)( m + 1 );

	return( m );
}



void
lsf_destroy( lsf_type *m )
{
	if( m = NULL )
		return;

	free( m );
}



float
lsf_value( lsf_type *lsf, float	x )
{
int	i;
lsfB_type	*f,	*f0;
float	y;


	if( lsf == NULL )	return( x );


	for( i = 0, f = lsf->f ; i < lsf->nL ; i++, f++ ){
		if( x < f->x ) {
			if( i == 0 )	return( f->y );

			f0 = f -1;

			y = f0->y + (f->y - f0->y) * ( x - f0->x)/(f->x-f0->x);

			return( y );

		}
	}

	return( (f-1)->y );
}



lsf_type *
lsf_parse( char *data )
{
lsf_type	*lsf;
lsfB_type	*f;
int	i,	no;

	if( gp_strtok_int( data, " \t\r\n", &no ) < 0 )
		return(NULL );



	lsf = lsf_alloc( no );
	
	lsf->nL = 0;
	for( i = 0, f = lsf->f ; i < no ; i++, f++ ){

		if( gp_strtok_float( NULL, " \t\r\n", &f->x ) < 0 ){
			lsf_destroy( lsf );
			return(NULL );
		}

		if( gp_strtok_float( NULL, " \t\r\n", &f->y ) < 0 ){
			lsf_destroy( lsf );
			return(NULL );
		}

		lsf->nL++;
	}

	return( lsf );
}


void
lsf_print( lsf_type *lsf, FILE *fp )
{
	int	i;
	lsfB_type	*f;


	if( lsf == NULL )	return;

	fprintf( fp, "%d\n", lsf->nL );

	for( i = 0, f = lsf->f ; i < lsf->nL ; i++, f++ ){
		fprintf( fp, "%f   %f\n", f->x, f->y );
	}
}
