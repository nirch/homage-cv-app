/***************************
 ***   Image1Average.c   ***
 ***************************/

#include	"ImageType/ImageType.h"




image_type *
image1_avrage4_diff( image_type *sim, image_type *var, image_type *im )
{
u_char	*vp;
u_char	*sp;
short	*tp,	*tp1;
int	i,	j;
int	k,	k1,	n,	n1,	align;
int	d,	sum;
int	no;


	im = image_recreate( im, sim->height/4 + 2, sim->width/4+2, 2, 1 );
	image2_const( im, 1 );

	
	no = 0;
	for( i = 0 ; i < var->row ; i += 4 ){
		tp = (short *)IMAGE_PIXEL( im, i/4+1, 1 );
		k1 = ( i+4 < var->height)? 4 : var->height-i;


		for( j = 0 ; j < var->column ; j += 4, tp++ ){


			n1 = ( j+4 < var->width)? 4 : var->width-j;

			align = var->width-n1;

			vp = (u_char *)IMAGE_PIXEL( var, i, j );
			sp = (u_char *)IMAGE_PIXEL( sim, i, j );


			sum = 0;
			for( k = 0 ; k < k1 ; k++, sp += align, vp += align ){
				for( n = 0 ; n < n1 ; n++, sp++, vp++ ){
					d = *sp - *vp;
					sum += d;
				}
			}

			*tp = sum;
		}
	}



	for( i = 1 ; i < im->height-1 ; i++ ){
		tp = (short *)IMAGE_PIXEL( im, i, 1 );
		tp1 = tp + im->width;
		for( j = 1 ; j < im->width-1 ; j++, tp++, tp1++ ){
			sum = *(tp) + *(tp+1) + *(tp1) + *(tp1+1);

			if( sum < 0 )	sum = -sum;
			sum >>= 6;
			*tp = sum;
		}
	}


	return( im );
}


image_type *
image1_average8( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;
	int	k,	k1,	n,	n1,	align;
	int	sum;
	int	no;


	im = image_recreate( im, sim->height/8, sim->width/8, 1, 1 );

	tp = im->data;
	no = 0;
	for( i = 0 ; i < sim->row ; i += 8 ){
		k1 = ( i+8 < sim->height)? 8 : sim->height-i;


		for( j = 0 ; j < sim->column ; j += 8, tp++ ){


			n1 = ( j+8 < sim->width)? 8 : sim->width-j;

			align = sim->width-n1;


			sp = (u_char *)IMAGE_PIXEL( sim, i, j );


			sum = 0;
			for( k = 0 ; k < k1 ; k++, sp += align ){
				for( n = 0 ; n < n1 ; n++, sp++ ){
					sum += (*sp);

				}
			}

			*tp = sum>>6;
		}
	}



	return( im );
}


image_type *
image1_average8B( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp,	*tp0;
	int	i,	j;
	int	k,	n,	align;

	im = image_recreate( im, sim->height/8, sim->width/8, 1, 1 );

	image1_const( im, 0 );


	align = sim->width - 8 *im->width;
	sp = sim->data;
	tp0 = im->data;

	for( i = 0 ; i < im->height ; i++, tp0 += im->width ){

		for( k = 0 ; k < 8 ; k++, sp += align ){

			for( j = 0, tp = tp0 ; j < im->width ; j++, tp++ )
				for( n = 0 ; n < 8 ; n++, sp++ ){
					*tp += *sp;
				}
		}
	}


	return( im );
}





image_type *
image1_boxN( image_type *sim, int b, int nT, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;
int	k,	k1,	n,	n1,	align;
int	sum;
int	no;


	im = image_recreate( im, sim->height/b, sim->width/b, 1, 1 );

	tp = im->data;
	no = 0;
	for( i = 0 ; i < sim->row ; i += b ){
		k1 = ( i+b < sim->height)? b : sim->height-i;


		for( j = 0 ; j < sim->column ; j += b, tp++ ){


			n1 = ( j+b < sim->width)? b : sim->width-j;

			align = sim->width-n1;


			sp = (u_char *)IMAGE_PIXEL( sim, i, j );


			sum = 0;
			for( k = 0 ; k < k1 ; k++, sp += align ){
				for( n = 0 ; n < n1 ; n++, sp++ ){
					if( *sp > 0 )	sum++;
				}
			}

			*tp =  ( sum > nT )? 255 : 0;
		}
	}



	return( im );
}
