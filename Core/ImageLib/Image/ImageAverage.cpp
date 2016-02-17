/*****************************
 ***   Image1Average.cpp   ***
 *****************************/

#include	"ImageType/ImageType.h"

//static image_type *	image1_average8( image_type *sim, image_type *im );
static image_type *	imageUS_average8( image_type *sim, image_type *im );



image_type *
image_average8( image_type *sim, image_type *im )
{


	switch( IMAGE_TYPE(sim) ){
	case IMAGE_TYPE_U8:
		im = image1_average8( sim, im );
		break;

	case IMAGE_TYPE_U16:
			im = imageUS_average8( sim, im );
		break;
	default:
		gpWarning( "image_average8", "Not supported" );
		return( NULL );
	}

	return( im );
} 




image_type *
image1_average8( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j,	i0,	j0;
	int	k,	n,	align;
	int	sum;
	int	no;


	int	N = 8;

	im = image_realloc( im, sim->width/N, sim->height/N, 1, IMAGE_TYPE_U8, 1 );

	align = sim->width-N;



	tp = im->data;
	no = 0;
	for( i = 0, i0 = 0  ; i < im->height ; i++, i0 += N ){


		for( j = 0, j0 = 0 ; j < im->width ; j++, j0 += N, tp++ ){


			sp = (u_char *)IMAGE_PIXEL( sim, i0, j0 );


			sum = 0;
			for( k = 0 ; k < N ; k++, sp += align ){
				for( n = 0 ; n < N ; n++, sp++ ){
					sum += (*sp);

				}
			}

			*tp = sum>>6;
		}
	}



	return( im );
}



static image_type *
imageUS_average8( image_type *sim, image_type *im )
{
	int	i,	j,	i0,	j0;
	int	k,	n,	align;
	int	sum;
	int	no;


	int	N = 8;

	im = image_realloc( im, sim->width/N, sim->height/N, 1, IMAGE_TYPE_U16, 1 );

	align = sim->width-N;


	u_short *tp = im->data_us;
	no = 0;
	for( i = 0, i0 = 0  ; i < im->height ; i++, i0 += N ){


		for( j = 0, j0 = 0 ; j < im->width ; j++, j0 += N, tp++ ){


			u_short *sp = (u_short *)IMAGE_PIXEL( sim, i0, j0 );


			sum = 0;
			for( k = 0 ; k < N ; k++, sp += align ){
				for( n = 0 ; n < N ; n++, sp++ ){
					sum += (*sp);

				}
			}

			*tp = sum>>6;
		}
	}



	return( im );
}

