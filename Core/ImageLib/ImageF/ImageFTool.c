
#include	<math.h>
#include "Umath/Matrix3Type.h"

#include	"ImageType/ImageType.h"
#include	"ImageFTool.h"



static image_type *	imageF_from_imageUS( image_type *im, image_type *imc );



image_type *
imageF_from( image_type *sim, image_type *im )
{

	if( sim->format == IMAGE_FORMAT( IMAGE_TYPE_U16, 1 ) ){
		im = imageF_from_imageUS( sim, im );
		return( im );
	}

 
	if( sim->depth == 1 ){
		im = imageF_from_image1( sim, im );
		return( im );
	}

	if( sim->depth == 2 ){
		im = imageF_from_image2( sim, im );
		return( im );
	}

	return( NULL );
}



image_type *
imageF_from_image1( image_type *im, image_type *imc )
{
u_char	*sp;
float	*tp;
int	i,	j;


	imc = image_realloc( imc, im->width, im->height, 1, IMAGE_TYPE_F, 1 );


	sp = (u_char *)im->data;

	tp = (float *)imc->data;

	for( i = 0; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}

	return( imc );
}






image_type *
imageF_to_image1( image_type *sim, image_type *im )
{
float	*sp;
u_char	*tp;
int	tmp;
int	i,	j;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );



	sp = (float *)sim->data;

	tp = (u_char *)im->data;

	for( i = 0; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			tmp = *sp++ + 0.5;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );
		}

	return( im );

}



image_type *
imageF_to_image1_S( image_type *sim, image_type *im )
{
float	*sp;
u_char	*tp;
float	min,	max,	factor,	tmp;
int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	imageF_minmax( sim, &min, &max );


	factor = 255/(max - min);

	sp = (float *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = ( *sp - min ) * factor;
			*tp = PUSH_TO_RANGE( tmp, 0, 255 );
		}


	return( im );
}





image_type *
imageF_from_image2( image_type *im, image_type *imc )
{
short	*sp;
float	*tp;
int	i,	j;

	imc = image_realloc( imc, im->width, im->height, 1, IMAGE_TYPE_F, 1 );



	sp = (short *)im->data;

	tp = (float *)imc->data;

	for( i = 0; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++ / 16.0;
		}

	return( imc );
}


static image_type *
imageF_from_imageUS( image_type *im, image_type *imc )
{
	short	*sp;
	float	*tp;
	int	i,	j;

	imc = image_realloc( imc, im->width, im->height, 1, IMAGE_TYPE_F, 1 );



	sp = (short *)im->data;

	tp = (float *)imc->data;

	for( i = 0; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}

		return( imc );
}


image_type *
imageF_to_image2( image_type *im, image_type *imc )
{
float	*sp;
short	*tp;
int	i,	j;



	imc = image_realloc( imc, im->width, im->height, 1, IMAGE_TYPE_S12, 1 );



	sp = (float *)im->data;

	tp = (short *)imc->data;

	for( i = 0; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++ * 16;
		}

	return( imc );
}

image_type *
imageF_negative( image_type *sim, image_type *im )
{
	int	i,	j;
	float	*sp,	*tp;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );


	sp = sim->data_f;
	tp = im->data_f;

	for( i = 0 ; i < im->height ; i++ )
		for( j = 0 ; j < im->width ; j++, sp++, tp++ )
			*tp = - *sp;

	return( im );
}

//image_type *
//imageF_sum( image_type *sim, image_type *aim, image_type *im )
//{
//	int	i,	j;
//	float	*sp,	*ap,	*tp;
//
//	
//	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );
//
//
//	sp = sim->data_f;
//	ap = aim->data_f;
//	tp = im->data_f;
//
//	for( i = 0 ; i < im->height ; i++ )
//		for( j = 0 ; j < im->width ; j++, sp++, ap++,  tp++ )
//			*tp = *sp + *ap;
//
//	return( im );
//}


image_type *
imagef_sub_c( image_type *im1, image_type *im0, image_type *im )
{
	float	*tp;
	u_char	*sp0,	*sp1;
	int	i,	j;


	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_F, 1 );

	sp1 = im1->data;
	sp0 = im0->data;

	tp = (float*)im->data;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = *sp1++ - (float)*sp0++;
		}

		return( im );
}

image_type *
imageF_divide( image_type *im1, image_type *im0, image_type *im )
{
float	*tp;
float	*sp0,	*sp1;
int	i,	j;


	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_F, 1 );

	sp1 = (float *)im1->data;
	sp0 = (float *)im0->data;

	tp = (float*)im->data;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, tp++, sp0++, sp1++ ){
			*tp = (*sp0==0 )? 0 : *sp1 / *sp0;
		}

	return( im );
}




void
imageF_minmax( image_type *im, float *min,	float *max )
{
float	*sp;
int	i,	j,	fSet;
//int	a;
	sp = (float *)im->data;


	*min = *max = -1;
	fSet = 0;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++ ){
//-32767.000
			if( *sp == -32767 )	continue;

			if( fSet == 0 ){
				*min = *max = *sp;
				fSet = 1;
				continue;
			}

			if( *sp < *min )
				*min = *sp;
			else	if( *sp > *max )
						*max = *sp;
		}
}



void
imageFM_minmax( image_type *im, image_type *mim, float *min, float *max )
{
float	*sp;
u_char	*mp;

int	i,	j,	n;

	sp = im->data_f;
	mp = mim->data;



	for( i = 0, n = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++ ){
			if( *mp == 0 )	continue;

			if( n == 0 )
				*min = *max = *sp;

			if( *sp < *min )
				*min = *sp;
			else	if( *sp > *max )
				*max = *sp;

			n++;
		}
}

void
imageF_minmaxP( image_type *im, float percent, float *min, float *max )
{
	int	h[4096];
	float	m0,	m1,	r0,	r1;

	imageF_minmax( im, &m0, &m1 );

	imageF_histogram( im, m0, m1, h );

	histogram_range( h, 4096, percent, percent, &r0, &r1 );


	*min = r0 * ( m1 - m0)/4095 + m0;
	*max = r1 * ( m1 - m0)/4095 + m0;
}


void
imageFM_minmaxP( image_type *im, image_type *mim, float percent, float *min, float *max )
{
int	h[4096];
float	m0,	m1,	r0,	r1;

	imageFM_minmax( im, mim, &m0, &m1 );

	imageFM_histogram( im, mim, m0, m1, h );

	histogram_range( h, 4096, percent, percent, &r0, &r1 );

	
	*min = r0 * ( m1 - m0)/4095 + m0;
	*max = r1 * ( m1 - m0)/4095 + m0;
}


void
imageF_max( image_type *im, float *max )
{
float	*sp;
int	i,	j;

	sp = (float *)im->data;

	*max = *sp;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++ ){

			if( *sp > *max )
				*max = *sp;
		}
}




void
imageFB_variance( image_type *im, box2i *b, float *av, float *var )
{
float	*sp;
double	sum,	sum2;
int	i,	j,	n;



	sum = sum2 = 0;

	for( i = b->y0, n = 0 ; i < b->y1 ; i++ ){
		sp = (float *)IMAGE_PIXEL( im, i, b->x0 );
		for( j = b->x0 ; j < b->x1 ; j++, sp++ ){
			sum += *sp;

			sum2 += *sp * *sp;

			n++;
		}
	}

	*av = sum / n;
	*var = sum2/ n - (*av) * (*av);
}



void
imageFM_variance( image_type *sim, image_type *mim, float *av, float *var )
{
float	*sp;
u_char	*mp;
double	sum,	sum2;
int	i,	j,	n;

	sum = sum2 = 0;

	sp = sim->data_f;
	mp = mim->data;
	for( i = 0, n = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp++, mp++ ){
			if( *mp == 0 )	continue;
			n++;
			sum += *sp;

			sum2 += *sp * *sp;
		}
	}


	*av = sum / n;
	*var = sum2/ n - (*av) * (*av);
}


image_type *
imageF_stretch( image_type *sim, image_type *im )
{
float	*sp,	*tp;
float	min,	max,	factor;
int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	imageF_minmax( sim, &min, &max );

//	if( max > 255 && sim == im  )	return;

	factor = 255/max;

	sp = (float *)sim->data;
	tp = (float *)im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, tp++ )
			*tp = *sp * factor;


	return( im );
}

image_type *
imageF_contrast( image_type *sim, float a, float b, image_type *im )
{
float	*sp,	*tp;
int	i,	j;




	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );


	sp = sim->data_f;
	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			*tp = a* ( (*sp) - b ) + b;
		}
	}

	return( im );
}

image_type *
imageF_linearAB( image_type *sim, float a, float b, image_type *im )
{
float	*sp,	*tp;
int	i,	j;



	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );


	sp = sim->data_f;
	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			*tp = a* *sp + b;
		}
	}

	return( im );
}

void
imageF_const( image_type *im, float val )
{
float	*sp;
int	i,	j,	k,	channle;

	sp = (float *)im->data;

	channle = im->depth/4;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
			for( k = 0; k < channle ; k++ ){
				*sp++ = val;
			}
}


image_type *
imageF_add_const( image_type *sim, float val, image_type *im )
{
float	*sp,	*tp;
int	i,	j;


	im = image_reallocL( sim, im );

	sp = sim->data_f;
	tp = im->data_f;

	for( i = 0 ; i < sim->row ; i++ )
		for( j = 0 ; j < sim->column ; j++, sp++, tp++ )
			*tp = *sp + val;

	return( im );

}



float
imageF_compare( image_type *im1, image_type *im0 )
{
float	*sp0,	*sp1;
int	i,	j;
float	tmp,	peak;

	sp1 = (float *)im1->data;
	sp0 = (float *)im0->data;


	peak = 0;

	for( i = 0 ; i < im0->row ; i++ )
		for( j = 0 ; j < im0->column ; j++ ){

			tmp = *sp1++ - (float)*sp0++;
			if( tmp < 0 )	tmp = - tmp;
			if( peak < tmp )	peak = tmp;
		}

	return( peak );
}


image_type *
imageF_to_y( image_type *sim, image_type *im )
{
float	*sp;
float	*p;
int	i,	j;
float	R,	G,	B;



	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	sp = sim->data_f;
	p = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;


			*p++ =  0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;

		}
	}

	return( im );
}

image_type *
imageF_limited( image_type *sim, float min, float max, image_type *im )
{
	float	*sp,	*tp;

	int	i,	j,	n;

	sp = sim->data_f;
	tp = im->data_f;


	for( i = 0, n = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){

			*tp = PUSH_TO_RANGE( *sp, min, max );
		}

		return( im );
}


image_type *
imageFM_limited( image_type *sim, image_type *mim, float min, float max, image_type *im )
{
float	*sp,	*tp;
u_char	*mp;

int	i,	j,	n;

	sp = sim->data_f;
	mp = mim->data;
	tp = im->data_f;


	for( i = 0, n = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++, tp++ ){
			if( *mp == 0 ){
				*tp = *sp;
				continue;
			}

			*tp = PUSH_TO_RANGE( *sp, min, max );
		}

	return( im );
}


image_type *
imageF_transform_matrix( image_type *sim, matrix3_type *m, image_type *im )
{
	u_char	*sp;
	float	*tp;
	int	i,	j;

	int	R,	G,	B;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	sp = sim->data;
	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			*tp++ = m->a00 * R + m->a10 * G + m->a20*B;

			*tp++ = m->a01 * R + m->a11 * G + m->a21*B;

			*tp++ = m->a02 * R + m->a12 * G + m->a22*B;
		}
	}


	return( im );
}


float   
imageF_mean( image_type *im )
{
int i,j;
float	*ip = im->data_f;
int channels = im->depth/4;

	float sum = 0;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < channels*im->column ; j++, ip++ ){
			sum += (*ip);
		}
	}

	return( sum/(im->row*im->column*channels) );
}



void   
imageF_normal( image_type *im )
{
	int i,j;
	float	*sp;
	double	t;



	double sum = 0;

	sp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){
			sum += (*sp) * (*sp);
		}
	}
	
	t = 1.0 / sqrt(sum);


	sp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){
			(*sp) *= t;
		}
	}




}



image_type *
imageF_binary( image_type *sim, int T, image_type *im )
{
	float	*sp;
	u_char	*tp;
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data_f;
	tp = im->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++, tp++ ){
			*tp = ( *sp <= T )? 0 : 255;

		}
	}

	return( im );
}