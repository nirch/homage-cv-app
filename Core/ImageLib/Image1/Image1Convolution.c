
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"






image_type *
image1_convolution( image_type *im, image_type *gim, image_type *cim )
{
int	i,	j,	n,	k,	align;
u_char	*sp,	*cp;
float	*gp;
float	sum;
float g_inv;
int	dw,	dh;
int	tmp;

	cim = image_recreate( cim, im->row, im->width, 1, 1 );
	image1_const( cim, 0 );

	gp = gim->data_f;
	sum = 0;
	for( i = 0 ; i < gim->height ; i++ ){
		for( j = 0; j < gim->width ; j++ )
			sum += *gp++;
	}
	g_inv = 1.0 / sum;



	align = im->width - gim->width;

	dw = gim->width/2;
	dh = gim->height/2;
	for( i = dh ; i+dh < im->height ; i++ ){
		cp = IMAGE_PIXEL( cim, i, dw );

		for( j = dw ; j + dw < im->width ; j++ ){

			sp = IMAGE_PIXEL( im, i - dh, j - dw );
			gp = gim->data_f;
			sum = 0;
			for( k = 0 ; k < gim->height ; k++, sp += align ){
				for( n = 0; n < gim->width ; n++ ){
					sum += (*gp++) * (*sp++);
				}
			}

			tmp = sum  * g_inv;
			*cp++ = PUSH_TO_RANGE( tmp, 0 , 255 );

		}
	}


	return( cim );

}



image_type *
image1_convolution_bilateral( image_type *im, image_type *gim, float sigma, image_type *cim )
{
int	i,	j,	n,	k,	align;
u_char	*sp,	*cp,	*sp0;
float	*gp;
float	sum;
int	dw,	dh;
int	tmp;
float gi[513];
float	sw,	w;

	guasian( 256, sigma, gi );

	cim = image_recreate( cim, im->row, im->width, 1, 1 );
	image1_const( cim, 0 );



	align = im->width - gim->width;

	dw = gim->width/2;
	dh = gim->height/2;
	for( i = dh ; i+dh < im->height ; i++ ){
		cp = IMAGE_PIXEL( cim, i, dw );

		for( j = dw ; j + dw < im->width ; j++ ){
			sp0 = IMAGE_PIXEL( im, i, j );

			sp = IMAGE_PIXEL( im, i - dh, j - dw );
			gp = gim->data_f;
			sum = 0;
			sw = 0;
			for( k = 0 ; k < gim->height ; k++, sp += align ){
				for( n = 0; n < gim->width ; n++ ){
					w = *gp++ * gi[ *sp - *sp0 + 256 ];
					sum += w * (*sp++);
					sw += w;
				}
			}

			tmp = sum  / sw;//* g_inv;
			*cp++ = PUSH_TO_RANGE( tmp, 0 , 255 );

		}
	}


	return( cim );

}