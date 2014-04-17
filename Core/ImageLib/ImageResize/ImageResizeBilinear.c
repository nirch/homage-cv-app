/**********************************
 ***	Image1ResizeBilinear.c	***
 *********************************/

#include	"ImageType/ImageType.h"
#include	"Umath/LT2Type.h"


#include "ImageResize.h"

image_type *image4_resize_bilinear( image_type *sim, float scale, image_type *im );


image_type * 
image_resize_bilinear( image_type *sim, float scale, image_type *im )
{

	if ( sim->depth == 1 ){
		im = image1_resize_bilinear( sim, scale, im );
		return( im );
	}


	if ( sim->depth == 3 ){
		im = image3_resize_bilinear( sim, scale, im );
		return( im );
	}


	if ( sim->depth == 4 ){
		im = image4_resize_bilinear( sim, scale, im );
		return( im );
	}


	if( im != NULL )
		image_destroy( im, 1 );
	
	return( NULL );
}




image_type *
image1_resize_bilinear( image_type *sim, float scale, image_type *im )
{
	u_char	*sp0,	*sp,	*tp;
	int	i,	j;
	float	x,	y,	x0,	y0;
	int	ix,	iy,	dx,	dy;
	int	s1,	s2;
	int	s;
	int width,	height;
	float	iscale;
	int	px,	py;


	width = sim->width * scale;
	height = sim->height * scale;

	iscale = 1.0 / scale;

	im = image_realloc( im, width, height, 1, IMAGE_TYPE_U8, 1 );





	tp = (u_char *)im->data;

	y0 = (0.5 - 1)*iscale;
	for( i = 0 ; i < im->height ; i++ ){
		y0 += iscale;
		y = PUSH_TO_RANGE( y0, 0.5, sim->height-1 );

		py = y * 256;
		iy = ( py - 128)>>8;
		dy = py - (iy<<8) - 128;

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		x0 = (0.5 - 1)* iscale;
		for( j = 0 ; j < im->width ; j++ ){
			x0 += iscale;
			x = PUSH_TO_RANGE( x0, 0.5, sim->width-1 );


			px = x * 256;
			ix = ( px - 128)>>8;
			dx = px - (ix<<8) - 128;


			sp = sp0 + ix;


			s1 = (*sp <<8) + dx * (  *(sp+1) - *sp );
			sp += sim->column;
			s2 = (*sp<<8) + dx * (*(sp+1) - *sp );


			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( im );
}



image_type *
image3_resize_bilinear( image_type *sim, float scale, image_type *im )
{
	u_char	*sp0,	*sp,	*sp1,	*tp;
	int	i,	j;
	float	x,	y,	x0,	y0;
	int	ix,	iy,	dx,	dy;
	int	s1,	s2;
	int	s;
	int width,	height;
	float	iscale;
	int	px,	py;


	width = sim->width * scale;
	height = sim->height * scale;

	iscale = 1.0 / scale;

	im = image_realloc( im, width, height, 3, IMAGE_TYPE_U8, 1 );





	tp = (u_char *)im->data;

	y0 = (0.5 - 1)*iscale;
	for( i = 0 ; i < im->height ; i++ ){
		y0 += iscale;
		y = PUSH_TO_RANGE( y0, 0.5, sim->height-1 );

		py = y * 256;
		iy = ( py - 128)>>8;
		dy = py - (iy<<8) - 128;

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		x0 = (0.5 - 1)* iscale;
		for( j = 0 ; j < im->width ; j++ ){
			x0 += iscale;
			x = PUSH_TO_RANGE( x0, 0.5, sim->width-1 );


			px = x * 256;
			ix = ( px - 128)>>8;
			dx = px - (ix<<8) - 128;


			sp = sp0 + 3*ix;
			sp1 = sp + 3*sim->column;


			s1 = (*sp <<8) + dx * (  *(sp+3) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+3) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );

			sp++;
			sp1++;
			s1 = (*sp <<8) + dx * (  *(sp+3) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+3) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );


			sp++;
			sp1++;
			s1 = (*sp <<8) + dx * (  *(sp+3) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+3) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );


		}
	}

	return( im );
}




image_type *
image4_resize_bilinear( image_type *sim, float scale, image_type *im )
{
	u_char	*sp0,	*sp,	*sp1,	*tp;
	int	i,	j;
	float	x,	y,	x0,	y0;
	int	ix,	iy,	dx,	dy;
	int	s1,	s2;
	int	s;
	int width,	height;
	float	iscale;
	int	px,	py;


	width = sim->width * scale;
	height = sim->height * scale;

	iscale = 1.0 / scale;


	im = image_realloc( im, width, height, 4, IMAGE_TYPE_U8, 1 );





	tp = (u_char *)im->data;

	y0 = (0.5 - 1)*iscale;
	for( i = 0 ; i < im->height ; i++ ){
		y0 += iscale;
		y = PUSH_TO_RANGE( y0, 0.5, sim->height-1 );

		py = y * 256;
		iy = ( py - 128)>>8;
		dy = py - (iy<<8) - 128;

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		x0 = (0.5 - 1)* iscale;
		for( j = 0 ; j < im->width ; j++ ){
			x0 += iscale;
			x = PUSH_TO_RANGE( x0, 0.5, sim->width-1 );


			px = x * 256;
			ix = ( px - 128)>>8;
			dx = px - (ix<<8) - 128;


			sp = sp0 + 4*ix;
			sp1 = sp + 4*sim->column;

			//sp++;
			//sp1++;
			//*tp++ = 0;

			s1 = (*sp <<8) + dx * (  *(sp+4) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+4) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );

			sp++;
			sp1++;
			s1 = (*sp <<8) + dx * (  *(sp+4) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+4) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );


			sp++;
			sp1++;
			s1 = (*sp <<8) + dx * (  *(sp+4) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+4) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );


			sp++;
			sp1++;
			*tp++ = 0;


		}
	}

	return( im );
}


static image_type *image3_resize_bilinearB( image_type *sim, box2i_type *b, int width, int height, image_type *im );
static image_type *image4_resize_bilinearB( image_type *sim, box2i_type *b, int width, int height, image_type *im );



image_type * 
image_resize_bilinearB( image_type *sim, box2i_type *b, int width, int height, image_type *im )
{

	//if ( sim->depth == 1 ){
	//	im = image1_resize_bilinearB( sim, b, width, height, im );
	//	return( im );
	//}


	if ( sim->depth == 3 ){
		im = image3_resize_bilinearB( sim, b, width, height, im );
		return( im );
	}


	if ( sim->depth == 4 ){
		im = image4_resize_bilinearB( sim, b, width, height, im );
		return( im );
	}


	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}


static image_type *
image3_resize_bilinearB( image_type *sim, box2i_type *b, int width, int height, image_type *im )
{
	u_char	*sp0,	*sp,	*sp1,	*tp;
	int	i,	j;
	float	x,	y,	x0,	y0;
	int	ix,	iy,	dx,	dy;
	int	s1,	s2;
	int	s;
	float	scale,	iscale;
	int	px,	py;





	im = image_realloc( im, width, height, 3, IMAGE_TYPE_U8, 1 );


	scale = width /(float)( b->x1 - b->x0 );
	
	iscale = 1.0 / scale;


	tp = (u_char *)im->data;

	y0 = b->y0 + (0.5 - 1)*iscale;
	for( i = 0 ; i < im->height ; i++ ){
		y0 += iscale;
		y = PUSH_TO_RANGE( y0, 0.5, sim->height-1 );

		py = y * 256;
		iy = ( py - 128)>>8;
		dy = py - (iy<<8) - 128;

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		x0 = b->x0 + (0.5 -1 )* iscale;
		for( j = 0 ; j < im->width ; j++ ){
			x0 += iscale;
			x = PUSH_TO_RANGE( x0, 0.5, sim->width-1 );


			px = x * 256;
			ix = ( px - 128)>>8;
			dx = px - (ix<<8) - 128;


			sp = sp0 + 3*ix;
			sp1 = sp + 3*sim->column;



			s1 = (*sp <<8) + dx * (  *(sp+3) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+3) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );

			sp++;
			sp1++;
			s1 = (*sp <<8) + dx * (  *(sp+3) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+3) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );


			sp++;
			sp1++;
			s1 = (*sp <<8) + dx * (  *(sp+3) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+3) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );

		}
	}

	return( im );
}


image_type *
image4_resize_bilinearB( image_type *sim, box2i_type *b, int width, int height, image_type *im )
{
	u_char	*sp0,	*sp,	*sp1,	*tp;
	int	i,	j;
	float	x,	y,	x0,	y0;
	int	ix,	iy,	dx,	dy;
	int	s1,	s2;
	int	s;
	float	scale,	iscale;
	int	px,	py;





	im = image_realloc( im, width, height, 4, IMAGE_TYPE_U8, 1 );


	scale = width /(float)( b->x1 - b->x0 );
	
	iscale = 1.0 / scale;


	tp = (u_char *)im->data;

	y0 = b->y0 + (0.5 - 1)*iscale;
	for( i = 0 ; i < im->height ; i++ ){
		y0 += iscale;
		y = PUSH_TO_RANGE( y0, 0.5, sim->height-1 );

		py = y * 256;
		iy = ( py - 128)>>8;
		dy = py - (iy<<8) - 128;

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		x0 = b->x0 + (0.5 -1 )* iscale;
		for( j = 0 ; j < im->width ; j++ ){
			x0 += iscale;
			x = PUSH_TO_RANGE( x0, 0.5, sim->width-1 );


			px = x * 256;
			ix = ( px - 128)>>8;
			dx = px - (ix<<8) - 128;


			sp = sp0 + 4*ix;
			sp1 = sp + 4*sim->column;

			//sp++;
			//sp1++;
			//*tp++ = 0;

			s1 = (*sp <<8) + dx * (  *(sp+4) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+4) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );

			sp++;
			sp1++;
			s1 = (*sp <<8) + dx * (  *(sp+4) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+4) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );


			sp++;
			sp1++;
			s1 = (*sp <<8) + dx * (  *(sp+4) - *sp );
			s2 = (*sp1<<8) + dx * (*(sp1+4) - *sp1 );
			s = ((s1<<8) + dy * (s2 - s1 ))>>16;

			*tp++ = PUSH_TO_RANGE(s, 0, 255 );


			sp++;
			sp1++;
			*tp++ = 0;


		}
	}

	return( im );
}
