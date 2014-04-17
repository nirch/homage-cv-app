/********************************
 ***   Image3DenaimcRange.c   ***
 ********************************/

#include "ImageType/ImageType.h"
#include "ImageType/ImageTool.h"

void
image3_dynamic_range( image_type *im, float h0, float h1 )
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
			h[*sp++] ++;
			h[*sp++] ++;
		}

	max = im->row *im->column *3 * h0/100;
	for( i = 0, sum = 0 ; i < 256 ; i++ ){
		sum += h[i];
		if( sum > max )	break;
	}
	a0 = i -1;

	max = im->row *im->column *3 * h1/100;
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

			*sp = h[ *sp];
			sp++;

			*sp = h[ *sp];
			sp++;


/*
			tmp = a * (*sp) + b;
			*sp++ = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = a * (*sp) + b;
			*sp++ = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = a * (*sp) + b;
			*sp++ = PUSH_TO_RANGE( tmp, 0, 255 );
*/
		}

}



image_type *
image3_dynamic_rangeN( image_type *sim, box2i *box, float h0, float h1, image_type *im )
{
float	m0,		m1;
float	a,	b;

	image3_dynamic_rangeH_R( sim, box,  h0, h1, &m0, &m1 );


	a = 255/(m1-m0);
	b = - a* m0;

	im = image3_stretching( sim, a, b, im );

	return( im );
}


void
image3_dynamic_rangeH_R( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 )
{
	u_char	*sp;
	int	i,	j,	i0,	i1,	j0,	j1,	n;
	int	h[256];
	int	sum,	max;

	imageA_box( im ,box, &j0, &i0, &j1, &i1 );

	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;


	for( i = i0, n = 0 ; i < i1 ; i++ ){
		sp = IMAGE_PIXEL( im, i, j0 );
		for( j = j0 ; j < j1 ; j++ ){
			h[*sp++] ++;
			h[*sp++] ++;
			h[*sp++] ++;
			n += 3;

		}
	}




	max = n * h0/100;
	for( i = 0, sum = 0 ; i < 256 ; i++ ){
		sum += h[i];
		if( sum > max )	break;
	}
	*a0 = i -1;



	max = n * h1/100;
	for( i = 255, sum = 0 ; i > 0 ; i-- ){
		sum += h[i];
		if( sum > max )	break;
	}
	*a1 = (i == 255)? 255 : i + 1;
}



image_type *
image3_stretching( image_type *sim, float a, float b, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;
int	h[256];
int	tmp;


	im = image_reallocL( sim, im );


	for( i = 0 ; i < 256 ; i++ ){
		tmp = a * i + b;
		h[i] = PUSH_TO_RANGE( tmp, 0, 255 );
	}


	sp = sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = h[ *sp++];
			*tp++ = h[ *sp++];
			*tp++ = h[ *sp++];
		}


	return( im );

}