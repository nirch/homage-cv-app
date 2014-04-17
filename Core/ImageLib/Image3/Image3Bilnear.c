/**************************
 ***   Image3Extend.c   ***
 **************************/


#include	"ImageType/ImageType.h"
#include	"Image3Tool.h"



int
image3_bilnear_pixel( image_type *sim, float x, float y,  u_char tp[] )
{
	u_char	*sp,	*sp1;
	float	dx,	dy;
	float	s1,	s2;
	int	s;
	int	iy,	ix;


	ix = x-0.5;
	iy = y-0.5;



	if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
		tp[0] = 0;
		tp[1] = 0;
		tp[2] = 0;
		return( -1 );
	}


	dx = x - (ix+0.5);
	dy = y - (iy+0.5);

	sp = IMAGE_PIXEL( sim, iy, ix );
	sp1 = sp + 3*sim->column;

	dx = x - (ix+0.5);
	dy = y - (iy+0.5);


	// Red
	s1 = (1-dx)* (*sp) + dx * (*(sp+3));
	s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
	s = (1-dy)*s1 + dy * s2 + 0.5;
	tp[0] = s;
	sp++; sp1++;

	//Green
	s1 = (1-dx)* (*sp) + dx * (*(sp+3));
	s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
	s = (1-dy)*s1 + dy * s2 + 0.5;
	tp[1] = s;
	sp++; sp1++;

	// Blue
	s1 = (1-dx)* (*sp) + dx * (*(sp+3));
	s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
	s = (1-dy)*s1 + dy * s2 + 0.5;
	tp[2] = s;
	sp++; sp1++;

	return( 1 );
}