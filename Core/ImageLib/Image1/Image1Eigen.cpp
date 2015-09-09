
#include	<stdio.h>
#include	<math.h>
#include	<string.h>



#include "Uigp/igp.h"
#include "Utime/GpTime.h"
#include "Ucamera/Pt2dType.h"
#include "Umath/Matrix2Type.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"


#define DD	8

image_type *
image1_eigen8( image_type *yim, image_type *im )
{
	int	i,	j,	k,	n;


	im = image_realloc( im, yim->width/DD, yim->height/DD, 1, IMAGE_TYPE_U8, 1 );
	imageF_const( im, 0 );





	u_char *tp = im->data + im->width + 1;

	double finv = 1.0 /( 64*DD*DD *64*DD*DD );

	int align = yim->width - DD;

	for( i = DD ; i+DD < yim->height ; i += DD, tp += 2 ){

		u_char *sp00 = IMAGE_PIXEL( yim, i, DD );
		for( j = DD ; j + DD < yim->width ; j += DD, sp00 += DD,  tp++ ){

			u_char *sp = sp00;
			u_char *sp0 = sp - yim->width;
			u_char *sp2 = sp + yim->width;

			int no = 0;

			int	Xx,	Yy,	Xy;

			Xx = Yy = Xy = 0;

			for( k = 0 ; k < DD ; k++, sp += align, sp0 += align, sp2 += align ){
				for( n = 0  ; n < DD ; n++, sp++, sp0++, sp2++ ){

					int cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
					int cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));

					Xx += cx * cx;
					Xy += cx * cy;
					Yy += cy * cy;


				}
			}

	

//			float det = (Xx * Yy - Xy *Xy)/(64.0*DD*DD);
			float det = (Xx * Yy - Xy *Xy)*finv;
			if( det < 0 )	det = -det;
			*tp = PUSH_TO_RANGE( det, 0, 255 );

		}
	}

	return( im );
}



image_type *
	image1_eigen8E( image_type *yim, image_type *im )
{
	int	i,	j,	k,	n;


	im = image_realloc( im, yim->width/DD, yim->height/DD, 1, IMAGE_TYPE_U8, 1 );
	imageF_const( im, 0 );





	u_char *tp = im->data + im->width + 1;

	double finv = 1.0 /( 64*DD*DD  );

	int align = yim->width - DD;

	for( i = DD ; i+DD < yim->height ; i += DD, tp += 2 ){

		u_char *sp00 = IMAGE_PIXEL( yim, i, DD );
		for( j = DD ; j + DD < yim->width ; j += DD, sp00 += DD,  tp++ ){

			u_char *sp = sp00;
			u_char *sp0 = sp - yim->width;
			u_char *sp2 = sp + yim->width;

			int no = 0;

			int	Xx,	Yy,	Xy;

			Xx = Yy = Xy = 0;

			for( k = 0 ; k < DD ; k++, sp += align, sp0 += align, sp2 += align ){
				for( n = 0  ; n < DD ; n++, sp++, sp0++, sp2++ ){

					int cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
					int cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));

					Xx += cx * cx;
					Xy += cx * cy;
					Yy += cy * cy;


				}
			}



			Xx *= finv;
			Yy *= finv;
			Xy *= finv;

			float desc = hypot( Xx-Yy, 2*Xy );

			float e1 = ((Xx+Yy) + desc ) * 0.5;
			float e2 = ((Xx+Yy) - desc ) * 0.5;

			int tmp = 128 * e2/e1;

			*tp = PUSH_TO_RANGE( e1, 0, 255 );

		}
	}

	return( im );
}