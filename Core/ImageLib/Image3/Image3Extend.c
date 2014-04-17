/**************************
 ***   Image3Extend.c   ***
 **************************/
#include	<string.h>

#include	"ImageType/ImageType.h"



image_type *
image3_extend_pad( image_type *sim, int d )
{
int	width,	height;
image_type	*im;
u_char	*sp,	*tp;
int	i,	j;

	width  = ((sim->width +d-1)/ d )*d;
	height = ((sim->height+d-1)/d ) *d ;


	if( width == sim->width && height == sim->height )
		return( sim );

	im = image_create( height, width, 3, 1, NULL );


	sp = sim->data;
	tp = im->data;

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;

		}

		for( ; j < im->width ; j++ ){
			*tp++ = 0;
			*tp++ = 0;
			*tp++ = 0;
		}
	}

	for( ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++ ){
			*tp++ = 0;
			*tp++ = 0;
			*tp++ = 0;
		}
	}

	return( im );
}