
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"ImageUSTool.h"


void
imageUS_const( image_type *im, int color )
{
	int	i,	j;


	color = 0xFF & color;


	u_short	*sp = im->data_us;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*sp++ = color;

		}
}



void
imageUS_minmax( image_type *im, int *min,	int *max )
{
u_short	*sp;
int	i,	j;
int	channel;


	channel = im->depth / 2;

	sp = im->data_us;
	*min = *max = *sp;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < channel*im->column ; j++, sp++ ){

			if( *sp < *min )
				*min = *sp;
			else	if( *sp > *max )
				*max = *sp;
		}
}

void
imageUS_max( image_type *im, int *max )
{
u_short	*sp;
int	i,	j;
int	channel;



	channel = im->depth / 2;

	sp = im->data_us;

	*max = *sp;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < channel*im->column ; j++, sp++ ){

			if( *sp > *max )
				*max = *sp;
		}
}



image_type *
imageUS_to_image1( image_type *sim, image_type *im )
{
u_short	*sp;
u_char	*tp;
int	i,	j;



	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data_us;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){


			*tp++ = *sp++;
		}
	}

	return( im );
}


image_type *
	imageUS_to_image1_F( image_type *sim, float f, image_type *im )
{
	u_short	*sp;
	u_char	*tp;
	int	i,	j;



	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data_us;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){

			float t = f * *sp;

			*tp++ = PUSH_TO_RANGE( t, 0, 255 );
		}
	}

	return( im );
}





image_type *
	imageUS_from( image_type *sim, image_type *im )
{
	if( sim->format == IMAGE_FORMAT_US ){
		if( im == NULL )
			return( sim );

		im = image_make_copy( sim, im );
		return( im );
	}



	if( sim->format == IMAGE_FORMAT_UC ){

		im = image2_from_image1( sim, im );
		return( im );
	}




	if( sim->depth == 3 )
		return imageUS3_to_y( sim, im );


	//if( sim->depth == 4 )
	//	return image4_to_y( sim, im );


	return( NULL );
}


image_type *
imageUS3_to_y( image_type *sim, image_type *im )
{
u_short	*sp,	*tp;
int	i,	j;
int	R,	G,	B;

int	Y;


	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT_US, 1 );

	sp = sim->data_us;
	tp = im->data_us;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;
			*tp++ = PUSH_TO_RANGE( Y, 0, ((1<<16)-1) );
		}
	}

	return( im );
}




image_type *
imageUS3_from_imageF( image_type *sim, image_type *im )
{
	float	*sp;
	u_short	*tp;
	int	i,	j;
	float	m0,	m1;
double a,	b,	tmp;


//	imageF_minmaxP( sim, 0.01, &m0, &m1 );
	imageF_minmax( sim, &m0, &m1 );
//	m0 = 100;



	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );


	

	a = ((1<<16)-1)/ ( m1 - m0 );
	b = -a*m0;

	sp = (float *)sim->data_f;

	tp = im->data_us;

	for( i = 0; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++ ){
			tmp = a * *sp + b;
			*tp++ = PUSH_TO_RANGE( tmp, 0, (1<<16)-1 );
			
		}

	return( im );
}



//image_type *
//imageUS3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im )
//{
//	u_short	*sp;
//	u_short	*tp;
//	int	i,	j;
//
//	int	R,	G,	B;
//
//	int	Y;
//
//
//
//
//	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT_US, 1 );
//
//	sp = sim->data_us;
//	tp = im->data_us;
//	for( i = 0 ; i < im->row ; i++ ){
//		for( j = 0 ; j < im->column ; j++ ){
//			R = *sp++;
//			G = *sp++;
//			B = *sp++;
//
//
//			Y  =  a + fR * R + fG * G + fB * B + 0.5;
//			*tp++ = PUSH_TO_RANGE( Y, 0, ((1<<16)-1) );
//		}
//	}
//
//	return( im );
//}


int
imageUS_compare( image_type *im1, image_type *im0 )
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
imageUS_to_U8( image_type *sim, int min, int max, image_type *im )
{
	u_short	*sp;
	u_char	*tp;
	int	i,	j;
	double	t;
	int	tmp;


	im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE_U8, 1 );


	sp = sim->data_us;
	tp = im->data;

	t = 255.0 / ( max-min);

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < sim->channel*im->column ; j++ ){



			tmp = ((*sp++) - min )*t + 0.5;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}

	return( im );
}


image_type *
imageUS_to_U8_smart(  image_type *sim, float p0, float p1, image_type *im )
{

	int	a0,	a1;
	imageUS_dynamic_range( sim, NULL, p0, p1, &a0, &a1 );
	if( a1 - a0 < 256 ){
		float da = 0.5*( 256 - ( a1-a0 ) );
		a0 -= da;
		a1 += da;
	}


	im = imageUS_to_U8( sim, a0, a1, NULL );

	return( im );
}

image_type *
imageUS_to_imageU8( image_type *sim, image_type *im )
{
	int	min,	max;


	imageUS_minmax( sim, &min, &max );


	im = imageUS_to_U8( sim, min, max, im );
	return( im );
}




image_type *
imageUS_linearAB( image_type *sim, float a, float b, image_type *im )
{
	u_short	*sp,	*tp;
	int	i,	j,	tmp;



	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );


	sp = sim->data_us;
	tp = im->data_us;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = a* *sp + b;
			*tp = PUSH_TO_RANGE( tmp, 0, 65535 );
		}
	}

	return( im );
}



image_type *
imageUS_binary( image_type *sim, int T, int unit, image_type *im )
{

	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	u_short *sp = sim->data_us;
	u_char *tp = im->data;

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp++, tp++ ){

			*tp = ( *sp <= T )? 0 : unit;

		}
	}

	return( im );
}


image_type *
	imageUS_band( image_type *sim, int T0, float T1, image_type *im )
{
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );

	u_short *sp = sim->data_us;
	u_short *tp = im->data_us;

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp++, tp++ ){

			*tp = PUSH_TO_RANGE( *sp, T0, T1 );
		}
	}

	return( im );
}


//image_type *
//imageUS_to_imageU8( image_type *sim, int a0, int a1, image_type *im )
//{
//	u_short	*sp;
//	u_char	*tp;
//	int	i,	j,	tmp;
//
//	im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE_U8, 1 );
//
//	double	d = 255.0/( a1 - a0 );
//
//	sp = (u_short *)sim->data_s;
//	tp = im->data;
//	for( i = 0 ; i < im->row ; i++ ){
//		for( j = 0 ; j < im->column*im->channel ; j++, sp++, tp++ ){
//			tmp = (*sp++ - a0 )*d;
//			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );
//		}
//	}
//
//	return( im );
//}