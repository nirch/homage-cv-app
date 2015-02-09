
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"ImageUSTool.h"




void
imageUS_dynamic_rangeH( image_type *im, float h0, float h1, int *a0, int *a1 )
{
	u_short	*tp;
	int	i,	j,	tmp;

	int	h[4096];
	int	channel;



	channel = im->depth / 2;


	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;

	tp = (u_short *)im->data_s;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < channel * im->column ; j++ ){
			tmp = *tp++;
			tmp >>= 4;
			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
			h[tmp]++;

		}
	}



	float	max,	sum;

	max = im->row *im->column *channel * h0/100;
	for( i = 0, sum = 0 ; i < 4096 ; i++ ){
		sum += h[i];
		if( sum > max )	break;
	}

///	if( i > 0 )	i -= 1;
	int i0 = i;
	*a0 = i*16;



	max = im->row *im->column *channel * h1/100;
	for( i = 4096-1, sum = 0 ; i > 0 ; i-- ){
		sum += h[i];
		if( sum > max )	break;
	}

//	if( i < 4095 )	i += 1;
	int i1 = i;
	*a1 = 16*i;



	for( i = i0 ; i < i1 && h[i] < 10 ; i++ );
	*a0 = i*16;

	for( i = i1 ; i > i0 && h[i] < 10 ; i-- );
	*a1 = 16*i;


	fprintf( stdout, "a0: %d    a1: %d\n", *a0, *a1 );


}