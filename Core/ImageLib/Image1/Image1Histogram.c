/********************************
 ***   Image3DenaimcRange.c   ***
 ********************************/

#include "ImageType/ImageType.h"
#include "ImageType/ImageTool.h"


int	image1_histogram_percent( image_type *im, float pT );




int
image1_histogram( image_type *im, box2i *box, int h[], int fclear )
{
u_char	*sp;
int	i,	j,	n;
int	i0,	i1,	j0,	j1;


	imageA_box( im , box, &j0, &i0, &j1, &i1 );

	if( fclear )
		for( i = 0 ; i < 256 ; i++ )	h[i] = 0;


	for( i = i0, n = 0 ; i < i1 ; i++ ){
		sp = IMAGE_PIXEL( im, i, j0 );
		for( j = j0 ; j < j1 ; j++ ){
			h[*sp++] ++;

			n++;
		}
	}

	return( n );
}

void
image1M_histogram( image_type *im, image_type *mim, int h[] )
{
u_char	*sp,	*mp;
int	i,	j;


	if( mim == NULL ){
		image1_histogram( im, NULL, h, 1 );
		return;
	}


	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;

	sp = im->data;
	mp = mim->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++ ){
			if( *mp != 0 )
				h[*sp] ++;
		}
}



void
image1M_histogramS( image_type *im, image_type *mim, int x0, int y0, int h[] )
{
	u_char	*sp,	*mp;
	int	i,	j;




	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;


	mp = mim->data;
	for( i = 0 ; i < mim->row ; i++ ){
		sp = IMAGE_PIXEL( im, i+y0, x0 );
		for( j = 0 ; j < mim->column ; j++, sp++, mp++ ){
			if( *mp != 0 )
				h[*sp] ++;
		}
	}
}



void
image1_range( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 )
{
	int	h[256];

	image1_histogram( im, box, h, 1 );


	histogram_range( h, 256, h0, h1, a0, a1 );
}




//void
//image1_dynamic_range( image_type *im, float h0, float h1 )
//{
//u_char	*sp;
//int	i,	j;
//int	h[256];
//int	sum,	max,	tmp;
//float	a0,	a1,	a,	b;
//
//	
//	
//
//	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;
//
//	sp = (u_char *)im->data;
//	for( i = 0 ; i < im->row ; i++ )
//		for( j = 0 ; j < im->column ; j++ ){
//			h[*sp++] ++;
//		}
//
//	max = im->row *im->column * h0/100;
//	for( i = 0, sum = 0 ; i < 256 ; i++ ){
//		sum += h[i];
//		if( sum > max )	break;
//	}
//	a0 = i -1;
//
//	max = im->row *im->column * h1/100;
//	for( i = 255, sum = 0 ; i > 0 ; i-- ){
//		sum += h[i];
//		if( sum > max )	break;
//	}
//	a1 = i + 1;
//
//
//	a = 255/(a1-a0);
//	b = - a* a0;
//
//	
//
//	for( i = 0 ; i < 256 ; i++ ){
//		tmp = a * i + b;
//		h[i] = PUSH_TO_RANGE( tmp, 0, 255 );
//	}
//	
//
//	sp = (u_char *)im->data;
//	for( i = 0 ; i < im->row ; i++ )
//		for( j = 0 ; j < im->column ; j++ ){
//			*sp = h[ *sp];
//			sp++;
//		}
//}
//
//
//
//
//
//void
//image1_dynamic_range_R( image_type *im, box2i *box, float h0, float h1 )
//{
//int	h[256];
//u_char	*sp;
//int	i,	j;
//int	tmp;
//float	a0,	a1,	a,	b;
//float	m1,	m0;
//
//	image1_range( im, box, h0, h1, &a0, &a1 );
//
//	m1 = MAX( a1, 245 );
//	m0 = MIN( a0, 15 );
//
//	
//
//	a = ( m1 - m0 )/( a1-a0);
//	b = m0 - a*a0;
//
//
//
//	for( i = 0 ; i < 256 ; i++ ){
//		tmp = a * i + b;
//		h[i] = PUSH_TO_RANGE( tmp, 0, 255 );
//	}
//
//
//	sp = (u_char *)im->data;
//	for( i = 0 ; i < im->row ; i++ )
//		for( j = 0 ; j < im->column ; j++ ){
//			*sp = h[ *sp];
//			sp++;
//		}
//}


image_type *
image1_dynamic_range( image_type *sim, box2i *box, float h0, float h1, image_type *im )
{
	float	m0,		m1;
	float	a,	b;

	image1_range( sim, box,  h0, h1, &m0, &m1 );


	a = 255/(m1-m0);
	b = - a* m0;

	im = image1_linear_intensity( sim, a, b, im );

	return( im );
}



image_type *
image1_linear_intensity( image_type *sim, float a, float b, image_type *im )
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
		}


		return( im );

}


//void
//image1_dynamic_rangeH_R( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 )
//{
//u_char	*sp;
//int	i,	j,	i0,	i1,	j0,	j1,	n;
//int	h[256];
//int	sum,	max;
//
//	imageA_box( im ,box, &j0, &i0, &j1, &i1 );
//
//	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;
//
//
//	for( i = i0, n = 0 ; i <= i1 ; i++ ){
//		sp = IMAGE_PIXEL( im, i, j0 );
//		for( j = j0 ; j <= j1 ; j++ ){
//			h[*sp++] ++;
//			n++;
//		}
//	}
//
//
//
//
//	max = n * h0/100;
//	for( i = 0, sum = 0 ; i < 256 ; i++ ){
//		sum += h[i];
//		if( sum > max )	break;
//	}
//	*a0 = i -1;
//
//
//
//	max = n * h1/100;
//	for( i = 255, sum = 0 ; i > 0 ; i-- ){
//		sum += h[i];
//		if( sum > max )	break;
//	}
//	*a1 = (i == 255)? 255 : i + 1;
//}
//



int
image1_histogram_percent( image_type *im, float pT )
{
u_char	*sp;
int	i,	j;
int	sum,	h[256];
float	T;



	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;

	sp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			h[*sp++] ++;
		}




	T = im->row *im->column * pT/100;
	for( i = 0, sum = 0 ; i < 256 ; i++ ){
		sum += h[i];
		if( sum > T )	break;

	}

	return( i );
}



int
image1M_histogram_percent( image_type *im, image_type *mim, float pT )
{
u_char	*sp,	*mp;
int	i,	j,	no;
int	sum,	h[256];
float	T;



	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;

	sp = (u_char *)im->data;
	mp = (u_char *)mim->data;
	for( i = 0, no = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++ ){
			if( *mp == 1 ){
				h[*sp] ++;
				no++;
			}
		}




		T = no * pT/100;
		for( i = 0, sum = 0 ; i < 256 ; i++ ){
			sum += h[i];
			if( sum > T )	break;

		}

		return( i );
}




int
image1M_histogram_range( image_type *im, image_type *mim, int nRange, int range[] )
{
u_char	*sp,	*mp;
int	i,	j,	no,	n;
int	sum,	h[256];
float	T;



	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;

	sp = (u_char *)im->data;
	mp = (u_char *)mim->data;
	for( i = 0, no = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++ ){
			if( *mp == 1 ){
				h[*sp] ++;
				no++;
			}
		}



	T = no / nRange;

	
	for( i = 0, sum = 0, n = 0 ; i < 256 ; i++ ){
		sum += h[i];
		if( sum > T ){
			range[n++] = i;
			sum = 0;
		}
	}

	if( sum > 0.5*T )
		range[n++] = i;

	return( n );
}




image_type *
image1_histogram_equlization( image_type *sim, image_type *im )
{
int	T[256],	H[256];
int	pixles,	sum;
u_char	*p,	*sp;
int	i,	j;

	image1_histogram( sim, NULL, H, 1 );

	pixles = IMAGE_ROW(sim) * IMAGE_COLUMN(sim);


	for( i = 0, sum = 0 ; i < 256 ; i++ ){
		sum += H[i];
		T[i] = (int)(255 * sum / (float)pixles);
	}

	
	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = (u_char *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
			*p++ = T[ *sp++ ];

	return( im );
}




float
image1_dark( image_type *sim, int T0 )
{
	int	H[256];
	int	pixles,	sum;

	int	i;

	image1_histogram( sim, NULL, H, 1 );

	pixles = sim->width*sim->height;


	for( i = 0, sum = 0 ; i < T0 ; i++ )
		sum += H[i];
	

	return( sum / (float)(pixles) );

}