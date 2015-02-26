/**************************
 ***	BstTool.c	***
 **************************/
#include	<stdio.h>
#include	<stdlib.h>


#include	"Uigp/igp.h"

#include	"GeoApp.h"


gapp_type *
gapp_alloc( int no )
{
gapp_type *gapp;

	gapp = (gapp_type *)malloc( sizeof(gapp_type) );

	gapp->N = no+1;

	gapp->v = (vec2d *) malloc( sizeof(vec2d)*gapp->N );

	gapp->tang = (vec2d *) malloc( sizeof(vec2d)*gapp->N );

	gapp->flag = (int *) malloc( sizeof(int)*gapp->N );

	gapp->a = (float *) malloc( sizeof(float)*gapp->N );
    
    gapp->no = 0;

	return( gapp );

}

gapp_type *
gapp_realloc( gapp_type *gapp, int no )
{
	if( gapp == NULL ){
		gapp = gapp_alloc( no );
		return( gapp );
	}

	if( no < gapp->N )
		return( gapp );

	free( gapp->v );
	free( gapp->tang );
	free( gapp->flag );
	free( gapp->a );


	gapp->N = no+1;

	gapp->v = (vec2d *) malloc( sizeof(vec2d)*gapp->N );

	gapp->tang = (vec2d *) malloc( sizeof(vec2d)*gapp->N );

	gapp->flag = (int *) malloc( sizeof(int)*gapp->N );

	gapp->a = (float *) malloc( sizeof(float)*gapp->N );

	return( gapp );
}


void
gapp_destroy( gapp_type *gapp )
{
	free( gapp->v );
	free( gapp->tang );
	free( gapp->flag );
	free( gapp->a );

	free( gapp );
}
