/**********************************
 ***	Image2Interpolation.c	***
 **********************************/

#include	"ImageType/ImageType.h"
#include	"Umath/LT2Type.h"


static void	imageS_interpolation_V( image_type *sim, vec2f_type *p, vec2f_type *v,
					   int x0, int y0,
					   int width,
					   image_type *im );


image_type *
imageAS_interpolation_V( imageA_type *aim, vec2f_type *p, vec2f_type *v, 
						 int x0, int width,
						 image_type *im )
{
int	i;

	im = image_realloc( im, 2*width+1, aim->nA, 1, IMAGE_TYPE_U16, 1 );


	for( i = 0 ; i < aim->nA ; i++ ){
		imageS_interpolation_V( aim->a[i], p, v,  
			x0, i, width, im );
	}

	return( im );
}


static void
imageS_interpolation_V( image_type *sim, vec2f_type *p, vec2f_type *v,
					int x0, int y0,
					int width,
					image_type *im )
{
u_short	*sp,	*tp;
int	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
int	s;





	tp = (u_short *)IMAGE_PIXEL( im, y0, x0 - width );
	for( j = -width ; j <= width ; j++ ){

		x = p->x + j *v->x;
		y = p->y + j *v->y;





		ix = x-0.5;
		iy = y-0.5;

		if( ix < 0 || ix >= sim->width-1 ||  iy < 0 || iy >= sim->height-1 ){
			*tp = 0xFFFF;
			continue;
		}






		sp = (u_short *)IMAGE_PIXEL( sim, iy, ix );

		dx = x - (ix+0.5);
		dy = y - (iy+0.5);

		dx = PUSH_TO_RANGE( dx, 0, 1 );
		dy = PUSH_TO_RANGE( dy, 0, 1 );


		s1 = (1-dx)* (*sp) + dx * (*(sp+1));
		sp += sim->column;
		s2 = (1-dx)* (*sp) + dx * (*(sp+1));

		s = (1-dy)*s1 + dy * s2 + 0.5;
		*tp++ = PUSH_TO_RANGE(s, 0, 0xFFFF );

	}

}
