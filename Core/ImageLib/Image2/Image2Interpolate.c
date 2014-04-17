/**********************************
 ***	Image2Interpolation.c	***
 **********************************/

#include	"ImageType/ImageType.h"
#include	"Umath/LT2Type.h"



image_type *
imageS_interpolation_lt2( image_type *sim, lt2_type *lt,
					int x0, int y0,
					int width, int height,
					image_type *im )
{
short	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;//,	ds;
int	ix,	iy;
float	s1,	s2;
int	s;


	im = image_recreate( im, height, width, 2, 1 );



	tp = im->data_s;
	for( i = 0 ; i < im->row ; i++ ){

		for( j = 0 ; j < im->column ; j++ ){


			x = LT2_F1( *lt, j+0.5, i+0.5 ) + x0;
			y = LT2_F2( *lt, j+0.5, i+0.5 ) + y0;



			ix = x-0.5;
			iy = y-0.5;


			ix = PUSH_TO_RANGE( ix, 0, sim->width-2 );
			iy = PUSH_TO_RANGE( iy, 0, sim->height-2 );




			sp = (short *)IMAGE_PIXEL( sim, iy, ix );

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			dx = PUSH_TO_RANGE( dx, 0, 1 );
			dy = PUSH_TO_RANGE( dy, 0, 1 );


			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 4096 );
		}
	}

	return( im );
}
