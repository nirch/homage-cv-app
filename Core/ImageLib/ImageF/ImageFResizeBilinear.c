/**********************************
 ***	ImageFResizeBilinear.c	***
 *********************************/

#include	"ImageType/ImageType.h"
#include	"Umath/LT2Type.h"



image_type *
imageF_resize_bilinear( image_type *sim, float scale, image_type *im )
{
float	*sp0,	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
float	s;
int width,	height;
float	iscale;


	width = sim->width * scale;
	height = sim->height * scale;

	iscale = 1.0 / scale;

//	im = image_recreate( im, height, width, 4, 1 );
	im = image_realloc( im, width, height, 1, IMAGE_TYPE_F, 1 );



	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		y = ( i + 0.5) *iscale;

		y = PUSH_TO_RANGE( y, 0.5, sim->height-1 );

		iy = y-0.5;
		dy = y - (iy+0.5);


		sp0 = (float *)IMAGE_PIXEL( sim, iy, 0 );

		for( j = 0 ; j < im->column ; j++ ){
			x = ( j + 0.5) *iscale;
			x = PUSH_TO_RANGE( x, 0.5, sim->width-1 );

			ix = x-0.5;
			dx = x - (ix+0.5);


			sp = sp0 + ix;

			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2;
			*tp++ = s;
		}
	}

	return( im );
}


image_type *
imageF2_resize_bilinear( image_type *sim, float scale, image_type *im )
{
float	*sp0,	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
float	s;
int width,	height;
float	iscale;


	width = sim->width * scale;
	height = sim->height * scale;

	iscale = 1.0 / scale;

//	im = image_recreate( im, height, width, 8, 1 );
	im = image_realloc( im, width, height, 2, IMAGE_TYPE_F, 1 );



	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		y = ( i + 0.5) *iscale;

		y = PUSH_TO_RANGE( y, 0.5, sim->height-1 );

		iy = y-0.5;
		dy = y - (iy+0.5);


		sp0 = (float *)IMAGE_PIXEL( sim, iy, 0 );

		for( j = 0 ; j < im->column ; j++ ){
			x = ( j + 0.5) *iscale;
			x = PUSH_TO_RANGE( x, 0.5, sim->width-1 );

			ix = x-0.5;
			dx = x - (ix+0.5);


			sp = sp0 + 2*ix;

			s1 = (1-dx)* (*sp) + dx * (*(sp+2));
			sp += 2*sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+2));

			s = (1-dy)*s1 + dy * s2;
			*tp++ = s;


			sp = sp0 + 2*ix+1;

			s1 = (1-dx)* (*sp) + dx * (*(sp+2));
			sp += 2*sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+2));

			s = (1-dy)*s1 + dy * s2;
			*tp++ = s;
		}
	}

	return( im );
}