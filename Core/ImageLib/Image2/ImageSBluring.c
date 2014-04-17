  /************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"


image_type *imageS_bluring_x( image_type *sim, int N );

image_type *imageS_bluring_y( image_type *sim, int N, image_type *im );



image_type *
imageS_bluring( image_type *sim, int nx, int ny, image_type *im )
{
image_type *tim;

	tim  = imageS_bluring_x( sim, nx );

	im  = imageS_bluring_y( tim, ny, im );

	image_destroy( tim, 1 );
	return( im );
}


image_type *
imageS_bluring_x( image_type *sim, int N )
{
short	*sp, *tp, *sp2;
int	i,	j, sum, N2;
float	N2_inv;
image_type * im;

	N2 = 2*N+1;
	N2_inv = 1.0/N2;

	im = image_alloc( sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );

	for( i = 0 ; i < im->row ; i++ ){
		tp = IMAGES_PIXEL( im, i, 0 );
		sp = IMAGES_PIXEL( sim, i, 0 );


		for( j = 0, sum = 0; j < N; j++, sp++, tp++){
			*tp = *sp;
			sum += *sp;
		}

		for( ; j < N2 ; j++, sp++){
			sum += *sp;
		}



		*tp++ = sum *N2_inv; 


		for( sp2 = sp - N2 ; j < im->column ; j++, sp++, sp2++, tp++){
			sum += (*sp - *sp2);
			*tp = sum *N2_inv; 
		}



		// copy source
		sp = IMAGES_PIXEL( sim, i, sim->column - N );
		for( j = 0; j < N ; j++, tp++, sp++ ){
			*tp = *sp;
		}


	}
	return( im );
}



image_type *
imageS_bluring_y( image_type *sim, int N, image_type *im )
{
	short	*sp, *tp, *sp2;
	int	i,	j, sum, N2;
	float	N2_inv;
	int	width;

	width = sim->width;

	N2 = 2*N+1;
	N2_inv = 1.0/N2;
	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );

	for( j = 0 ; j < im->width ; j++ ){
		tp = IMAGES_PIXEL( im, 0, j );
		sp = IMAGES_PIXEL( sim, 0, j );


		for( i = 0, sum = 0; i < N; i++, sp += width, tp += width){
			*tp = *sp;
			sum += *sp;
		}

		for( ; i < N2 ; i++, sp += width ){
			sum += *sp;
		}



		*tp = sum *N2_inv; 
		tp += width;


		for( sp2 = sp - N2*width ; i < im->height ; i++, sp += width, sp2 += width, tp += width){
			sum += (*sp - *sp2);
			*tp = sum *N2_inv; 
		}



		// copy source
		sp = IMAGES_PIXEL( sim, sim->height - N, j );
		for( i = 0; i < N ; i++, tp += width, sp += width ){
			*tp = *sp;
		}


	}
	return( im );
}





