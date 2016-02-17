/***************************
 ***   ImageFusion.cpp   ***
 ***************************/
#include	"Uigp/igp.h"

#include	"ImageType/ImageType.h"






image_type *
image_fusion( image_type *sim0, image_type *sim1, float t, image_type *im )
{
	int	i,	j;

	im = image_reallocL( sim0, im );

	u_char	*sp0 = sim0->data;
	u_char *sp1 = sim1->data;
	u_char	*tp = im->data;

	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width*im->channel ; j++, sp0++, sp1++, tp++ ){
			int tmp = (1-t)* *sp0 + t * *sp1;
			*tp = PUSH_TO_RANGE( tmp, 0, 255 );

			//if( ( j %4) == 3 && *sp0 == 255 )
			//	*tp = 255;
		}
	}

	return( im );
}