/************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"Image3Tool.h"





image_type *
image3_1280X720_to_640X480( image_type *sim, image_type *im )
{
	u_char	*tp,	*tp1;
	u_char	*sp,	*sp1;
	u_char	*bp,	*bp1,	*bp2;
	int	i,	j,	k,	i1;
	u_char	ab[640*3*3];

	//	im = image_realloc( im, 640, 480, 3, IMAGE_TYPE_U8, 1  );
	im = image_recreate( im, 480, 640, 3, 1  );


	tp = im->data;
	for( i = 0, i1 = 0 ; i < im->height ; i += 2, i1 += 3 ){
		bp = ab;
		for( k = 0 ; k < 3 ; k++ ){
			sp = IMAGE_PIXEL( sim, i1+k, 160 );
			sp1 = sp + 3;

			for( j = 0 ; j < im->width ; j += 2 ){
				*bp++ = ((*sp++) *3 + (*sp1++)) >>2;
				*bp++ = ((*sp++) *3 + (*sp1++)) >>2;
				*bp++ = ((*sp++) *3 + (*sp1++)) >>2;

				*bp++ = ((*sp++) + (*sp1++) *3) >>2;
				*bp++ = ((*sp++) + (*sp1++) *3) >>2;
				*bp++ = ((*sp++) + (*sp1++) *3) >>2;
				sp += 3;
				sp1 += 3;
			}
		}

		bp = ab;
		bp1 = bp + 3*im->width;
		bp2 = bp1 + 3*im->width;

		tp1 = tp + 3*im->width;

		for( j = 0 ; j < 640 ; j++ ){
			*tp++ = ((*bp) *3 + (*bp1)) >>2;
			*tp1++ = ((*bp1) + (*bp2)*3) >>2;
			bp++; bp1++; bp2++;

			*tp++ = ((*bp) *3 + (*bp1)) >>2;
			*tp1++ = ((*bp1) + (*bp2)*3) >>2;
			bp++; bp1++; bp2++;



			*tp++ = ((*bp) *3 + (*bp1)) >>2;
			*tp1++ = ((*bp1) + (*bp2)*3) >>2;
			bp++; bp1++; bp2++;

		}

		tp = tp1;

	}

	return( im );
}