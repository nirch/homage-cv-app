/**********************************
 ***	Image1ResizeBilinear.c	***
 *********************************/

#include	"ImageType/ImageType.h"
#include	"Umath/LT2Type.h"



image_type *
image1_resize_bilinear( image_type *sim, float scale, image_type *im )
{
u_char	*sp0,	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
int	s;
int width,	height;
float	iscale;


	width = sim->width * scale;
	height = sim->height * scale;

	iscale = 1.0 / scale;

	im = image_recreate( im, height, width, 1, 1 );





	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		y = ( i + 0.5) *iscale;

		y = PUSH_TO_RANGE( y, 0.5, sim->height-1 );

		iy = y-0.5;
		dy = y - (iy+0.5);


		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		for( j = 0 ; j < im->column ; j++ ){
			x = ( j + 0.5) *iscale;
			x = PUSH_TO_RANGE( x, 0.5, sim->width-1 );

			ix = x-0.5;
			dx = x - (ix+0.5);


			sp = sp0 + ix;

			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( im );
}



image_type *
image1_resize_bilinearI( image_type *sim, float scale, image_type *im )
{
	u_char	*sp0,	*sp,	*tp;
	int	i,	j;
	float	x,	y;
	int	ix,	iy,	dx,	dy;
	int	s1,	s2;
	int	s;
	int width,	height;
	float	iscale;
	int	px,	py;


	width = sim->width * scale;
	height = sim->height * scale;

	iscale = 1.0 / scale;

	im = image_recreate( im, height, width, 1, 1 );





	tp = (u_char *)im->data;

	y = (0.5 - 1)*iscale;
	for( i = 0 ; i < im->height ; i++ ){
		y += iscale;
		y = PUSH_TO_RANGE( y, 0.5, sim->height-1 );

		py = y * 256;
		iy = ( py - 128)>>8;
		dy = py - (iy<<8) - 128;

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		x = (0.5 - 1)* iscale;
		for( j = 0 ; j < im->width ; j++ ){
			x += iscale;
			x = PUSH_TO_RANGE( x, 0.5, sim->width-1 );


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
