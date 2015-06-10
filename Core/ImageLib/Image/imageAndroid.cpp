/**************************
 ***	Image4From.c	***
 *************************/

#include "ImageType/ImageType.h"




image_type *
image_android( image_type *sim, image_type *im )
{
	
	if( sim->depth == 3 ){
		im = image3_to_android( sim, im );
		return( im );
	}


	if( sim->depth == 4 ){
		im = image4_to_android( sim, im );
		return( im );
	}

	if( im != NULL )
	image_destroy( im, 1 );

	return( NULL );
}


image_type *
image3_to_android( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp,	sp0,	sp2;
	int	i,	j;


	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	tp = im->data;
	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, tp += 4, sp += 3 ){


			sp0 = sp[0];
			sp2 = sp[2];

			tp[0] = sp0;
			tp[1] = sp[1];
			tp[2] = sp2;
			tp[3] = 255;
		}
	}

	return( im );
}


image_type *
image4_to_android( image_type *sim, image_type *im )
{
u_char	*sp,	*tp,	sp0,	sp2;
	int	i,	j;

	if( sim->depth == 3 ){
		im = image3_to_android( sim, im );
		return( im );
	}

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	tp = im->data;
	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, tp += 4, sp += 4 ){

			
			sp0 = sp[0];
			sp2 = sp[2];

			tp[0] = sp2;
			tp[1] = sp[1];
			tp[2] = sp0;
			tp[3] = sp[3];
		}
	}

	return( im );
}