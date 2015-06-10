/********************************
 ***   Image3DenaimcRange.c   ***
 ********************************/
#include "ImageType/ImageType.h"
#include "Image3Tool.h"



float
image3_distance( image_type *im0, image_type *im1 )
{
u_char	*sp0,	*sp1;

int	i,	j;
int	tmp,	max;
float	sum;


	sp0 = (u_char *)im0->data;
	sp1 = (u_char *)im1->data;

	sum = 0;
	for( i = 0 ; i < im0->row ; i++, sp0 += im0->pad, sp1 += im1->pad ){

		for( j = 0 ; j < im0->column ; j++ ){

			max = (int)*sp0++ - (int)*sp1++;
			if( max < 0 )	max = -max;
	
			tmp = (int)*sp0++ - (int)*sp1++;
			if( tmp < 0 )	tmp = -tmp;
			if( tmp > max )	max = tmp;

			tmp = (int)*sp0++ - (int)*sp1++;
			if( tmp < 0 )	tmp = -tmp;
			if( tmp > max )	max = tmp;

			sum += max;
		}
	}

	return( sum );
}
