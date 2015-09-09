/*************************
 ***   Poly2d3Type.c   ***
 *************************/

#include	"Poly2d3Type.h"



void	
poly2d3_zero( poly2d3_type *f )
{
	f->a00 = 0;

	f->a10 = 0;
	f->a01 = 0;

	f->a20 = 0;
	f->a11 = 0;
	f->a02 = 0;

	f->a30 = 0;
	f->a21 = 0;
	f->a12 = 0;
	f->a03 = 0;
}


void	
poly2d3_derivative_x( poly2d3_type *f, poly2d3_type *fx )
{
	fx->a00 = f->a10;

	fx->a10 = 2*f->a20;
	fx->a01 =   f->a11;

	fx->a20 = 3*f->a30;
	fx->a11 = 2*f->a21;
	fx->a02 =   f->a12;

	fx->a30 = 0;
	fx->a21 = 0;
	fx->a12 = 0;
	fx->a03 = 0;
}

void	
poly2d3_derivative_y( poly2d3_type *f, poly2d3_type *fy )
{
	fy->a00 = f->a01;

	fy->a10 =   f->a11;
	fy->a01 = 2*f->a02;

	fy->a20 =   f->a21;
	fy->a11 = 2*f->a12;
	fy->a02 = 3*f->a03;

	fy->a30 = 0;
	fy->a21 = 0;
	fy->a12 = 0;
	fy->a03 = 0;
}


