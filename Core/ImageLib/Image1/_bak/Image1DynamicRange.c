/********************************
 ***   Image1DenaimcRange.c   ***
 ********************************/

#include "ImageType/ImageType.h"
#include "Image1Tool.h"



void
image1_dynamic_range( image_type *im, float h0, float h1 )
{
u_char	*sp;
int	i,	j;
int	h[256];
int	sum,	max,	tmp;
float	a0,	a1,	a,	b;

	
	

	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;

	sp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			h[*sp++] ++;
		}

	max = im->row *im->column * h0/100;
	for( i = 0, sum = 0 ; i < 256 ; i++ ){
		sum += h[i];
		if( sum > max )	break;
	}
	a0 = i -1;

	max = im->row *im->column * h1/100;
	for( i = 255, sum = 0 ; i > 0 ; i-- ){
		sum += h[i];
		if( sum > max )	break;
	}
	a1 = i + 1;


	a = 255/(a1-a0);
	b = - a* a0;

	

	for( i = 0 ; i < 256 ; i++ ){
		tmp = a * i + b;
		h[i] = PUSH_TO_RANGE( tmp, 0, 255 );
	}
	

	sp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*sp = h[ *sp];
			sp++;
		}

}
