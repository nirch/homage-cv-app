
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"ImageFTool.h"


void
imageF_dynamic_range( image_type *im, float h0, float h1 )
{
float	*tp;
int	i,	j;

int	h[256];
int	sum,	max,	tmp;
float	a0,	a1,	a,	b;



	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;

	tp = (float *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			tmp = *tp++;
			if( tmp < 0 )	tmp = 0;
			else	if( tmp > 255 )	tmp = 255;
			h[tmp] ++;

		}
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



	tp = (float *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = a * (*tp) + b;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}
}


void
imageF_dynamic_rangeH( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 )
{
float	*sp;
int		x0,	y0,	x1,	y1;
int	i,	j,	no;

int	h[4096];
int	sum,	max,	tmp;




	imageA_box( im, box, &x0, &y0, &x1, &y1 );



	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;


	no = 0;
	for( i = y0 ; i < y1 ; i++ ){
		sp = (float *)IMAGE_PIXEL( im, i, x0 );
		for( j = x0 ; j < x1 ; j++, sp++ ){
			tmp = 16 * *sp;
			tmp = PUSH_TO_RANGE( tmp, 0, 4095 );
			h[tmp] ++;
			no++;
		}
	}




	if( a0 != NULL ){
		max = no * h0/100;
		for( i = 0, sum = 0 ; i < 4096 ; i++ ){
			sum += h[i];
			if( sum > max )	break;
		}

		*a0 = (i + 0.5)/16.0;
	}



	if( a1 != NULL ){
		max = no * h1/100;
		for( i = 4095, sum = 0 ; i > 0 ; i-- ){
			sum += h[i];
			if( sum > max )	break;
		}
		*a1 = (i + 1)/16.0;
	}
}




void
imageF_dynamic_rangeHS( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 )
{
	float	*sp;
	int		x0,	y0,	x1,	y1;
	int	i,	j,	no;

	int	h[4096];
	int	sum,	max,	tmp;




	imageA_box( im, box, &x0, &y0, &x1, &y1 );



	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;


	no = 0;
	for( i = y0 ; i < y1 ; i++ ){
		sp = (float *)IMAGE_PIXEL( im, i, x0 );
		for( j = x0 ; j < x1 ; j++, sp++ ){
			tmp = 16 * (*sp + 128);
			tmp = PUSH_TO_RANGE( tmp, 0, 4095 );
			h[tmp] ++;
			no++;
		}
	}




	if( a0 != NULL ){
		max = no * h0/100;
		for( i = 0, sum = 0 ; i < 4096 ; i++ ){
			sum += h[i];
			if( sum > max )	break;
		}

		*a0 = (i + 0.5)/16.0 - 128;
	}



	if( a1 != NULL ){
		max = no * h1/100;
		for( i = 4095, sum = 0 ; i > 0 ; i-- ){
			sum += h[i];
			if( sum > max )	break;
		}
		*a1 = (i + 1)/16.0 - 128;
	}
}
