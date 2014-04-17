/****************************
***   Image1Integral.c   ***
****************************/

#include	"Uigp/igp.h"
#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"



image_type *
image1_integral_y( image_type *sim, box2i *b, int n, image_type *im )
{
int	width,	height;
int	*a,	i,	j,	i0,	N;
float	N_inv;
u_char	*sp,	*tp;


	width = b->x1 - b->x0;
	height = b->y1 - b->y0;


	im = image_recreate( im, height, width, 1, 1 );

	a = ( int * )malloc( width * sizeof(int) );



	for( i = 0 ; i < width ; i++ )
		a[i] = 0;


	N  = 2*n+1;
	N_inv = 1.0 / N;
	

	for( i = 0, i0 = b->y0-n ; i < N-1 ; i++, i0++ ){
		sp = IMAGE_PIXEL( sim, i0, b->x0);
		for( j = 0 ; j < width ; j++ )
			a[j] += *sp++;
	}


	for( i = 0 ; i < im->height ; i++, i0++ ){

		sp = IMAGE_PIXEL( sim, i0, b->x0);
		for( j = 0 ; j < width ; j++ )
			a[j] += *sp++;


		tp = IMAGE_PIXEL( im, i, 0 );
		for( j = 0; j < width ; j++  )
			*tp++ = a[j] * N_inv;


		sp = IMAGE_PIXEL( sim, i0-N, b->x0);
		for( j = 0 ; j < width ; j++ )
			a[j] -= *sp++;
	}


	free( a );

	return( im );

}

