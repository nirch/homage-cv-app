
#include	<string.h>
#include	<math.h>

#ifdef _DEBUG
#define _DUMP 
#endif


#include "ImageType/ImageType.h"

int
image_boundary( image_type *sim, int color )
{

	if( sim->depth == 1 ){
		image1_boundary( sim, color );
		return( 1 );
	}

	if( sim->depth == 1 ){
		image4_boundary( sim, color );
		return( 1 );
	}

	return( -1 );
}


void
image4_boundary( image_type *sim, int color )
{
	int	j;

	u_int *sp = sim->data_ui;
	for( j = 0 ; j < sim->width ; j++ )
		*sp++ = color;

	sp = (u_int *)IMAGE_PIXEL( sim, sim->height-1, 0 );
	for( j = 0 ; j < sim->width ; j++ )
		*sp++ = color;


	sp = sim->data_ui;
	for( j = 0 ; j < sim->height ; j++, sp += sim->width )
		*sp = color;


	sp = (u_int *)IMAGE_PIXEL( sim, 0, sim->width-1 );
	for( j = 0 ; j < sim->height ; j++, sp += sim->width )
		*sp = color;
}



void
	image1_boundary( image_type *sim, int color )
{
	int	j;

	u_char *sp = sim->data;
	for( j = 0 ; j < sim->width ; j++ )
		*sp++ = color;

	sp = IMAGE_PIXEL( sim, sim->height-1, 0 );
	for( j = 0 ; j < sim->width ; j++ )
		*sp++ = color;


	sp = sim->data;
	for( j = 0 ; j < sim->height ; j++, sp += sim->width )
		*sp = color;


	sp = IMAGE_PIXEL( sim, 0, sim->width-1 );
	for( j = 0 ; j < sim->height ; j++, sp += sim->width )
		*sp = color;
}




