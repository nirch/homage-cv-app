/************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"

#define         PIXEL_12_TO_8( x )   \
	      		( ( (x) <= 0 )? 0 : (( (x) > 4096 )? 255 : ((x)>>4) ) )


image_type *
image2_to_image1( image_type *sim, image_type *im )
{
short	*sp;
u_char	*tp;
int	i,	j;
int	tmp;

	//im = image_recreate( im, sim->row, sim->column, 1 , 1 );
	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = (short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );
		}
	}

	return( im );
}

image_type *
image2_to_image1_F( image_type *sim, float f, image_type *im )
{
short	*sp;
u_char	*tp;
int	i,	j;
int	tmp;

//	im = image_recreate( im, sim->row, sim->column, 1 , 1 );
	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	sp = (short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *sp * f + 0.5;
			tmp = tmp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );
		}
	}

	return( im );
}



image_type *
image2_from_image1( image_type *sim, image_type *im )
{
u_short	*tp;
u_char	*sp;
int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_S12, 1 );

	sp = sim->data;
	tp = (short *)im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = *(sp++) << 4;
		}
	}

	return( im );
}


image_type *
image2_from_imageF( image_type *sim )
{
image_type	*im;
short	*tp;
float	*sp;
int	i,	j;

	im = image_create( sim->row, sim->column, 2 , 1, NULL );

	sp = (float*)sim->data;
	tp = (short *)im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = (int)(*(sp++) * 16);
		}
	}

	return( im );
}


void
image2_const( image_type *im, int r )
{
u_short	*tp;
int	i,	j;

	tp = (u_short *)im->data;


	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = r;
		}
	}
}

//void
//image2_dynamic_range( image_type *im, float h0, float h1, float maxD )
//{
//short	*tp;
//int	i,	j;
//
//int	h[4096];
//int	sum,	max,	tmp;
//float	a0,	a1,	a,	b;
//
//
//
//	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;
//
//	tp = (short *)im->data;
//	for( i = 0 ; i < im->row ; i++ ){
//		for( j = 0 ; j < im->column ; j++ ){
//			tmp = *tp++;
//			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
//			h[tmp]++;
//		}
//	}
//
//
//
//	max = im->row *im->column * h0/100;
//	for( i = 0, sum = 0 ; i < 4096 ; i++ ){
//		sum += h[i];
//		if( sum > max )	break;
//	}
//		
//	a0 = i -1;
//	if( a0 < 0 )	a0 = 0;
//	if( maxD > 0 && a0 > maxD*16 )	a0 = maxD*16;
//
//
//
//	max = im->row *im->column* h1/100;
//	for( i = 4096-1, sum = 0 ; i > 0 ; i-- ){
//		sum += h[i];
//		if( sum > max )	break;
//	}
//
//	a1 = i + 1;
//	if( maxD > 0 && a1 < 4096 - maxD*16 )	a1 = 4096 - maxD*16;
//	
//
//
//	a = 4095/(a1-a0);
//	b = - a* a0;
//
//	
//
//	tp = (short *)im->data;
//	for( i = 0 ; i < im->row ; i++ ){
//		for( j = 0 ; j < im->column ; j++ ){
//
//			tmp = a * (*tp) + b;
//			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );
//		}
//	}
//}
//

image_type *
image2_subtruct( image_type *im1, image_type *im0, image_type *im )
{
short	*sp1,	*sp0,	*tp;
int	i,	j;

	if( im == NULL )
		im = image_create( im1->row, im1->column, 2, 1, NULL );

	sp1 = (short *)im1->data;
	sp0 = (short *)im0->data;
	tp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

//			*tp++ = (1<<11) + *sp1 - *sp0;
			*tp++ = *sp1 - *sp0;
			sp1++; sp0++;
		}

	return( im );
}



image_type *
image2_copy( image_type *sim, image_type *im )
{
u_short	*tp;
u_short	*sp;
int	i,	j;

	if( im == NULL )
		im = image_create( sim->row, sim->column, 2 , 1, NULL );

	sp = (u_short *)sim->data;
	tp = (u_short *)im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = *sp++;
		}
	}

	return( im );
}


image_type *
image2_copy_image1( image_type *sim, image_type *im )
{
u_short	*tp;
u_char	*sp;
int	i,	j;

	if( im == NULL )
		im = image_create( sim->row, sim->column, 2 , 1, NULL );

	sp = sim->data;
	tp = (u_short *)im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = *(sp++) << 4;
		}
	}

	return( im );
}


void
image2_minus( image_type *sim )
{
short	*sp;
int	i,	j;

	sp = (short *)sim->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++ )
			*sp = - *sp;
	}
}


void
image2_binary_down( image_type *sim, int T )
{
	short	*sp;
	int	i,	j;


	sp = sim->data_s;
	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++ ){

			*sp = ( *sp < T )? 1 : 0;
		}
	}
}



image_type *
image2_abs_diff( image_type *im0, image_type *im1, image_type *im )
{
short	*sp0,	*sp1,	*tp;
int	i,	j;
int	tmp;

//	im = image_recreate( im, im0->height, im0->width, 2, 1 );
	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_S12, 1 );

	sp0 = im0->data_s;
	sp1 = im1->data_s;

	tp = im->data_s;


	for( i = 0; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp0++, sp1++, tp++ ){
			tmp = *sp1 - *sp0;
			if( tmp < 0 )	tmp = -tmp;

			*tp = tmp;
		}
	}



	return( im );
}