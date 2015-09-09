/*************************
 ***   Poly2d3Type.c   ***
 *************************/

#include	"Poly3d3Type.h"



void	
poly3d3_zero( poly3d3_type *f )
{
int	i;

	for( i = 0 ; i < POLY3D3_COEFF ; i++ )
		f->a[0] = 0;
}



double	
poly3d3_value( poly3d3_type *f, double x, double y, double z )
{
	double L = x;
	double P = y;
	double H = z;

	double val = 0;

	val = f->a[0];
	val +=  f->a[1]*L;
	val +=  f->a[2]*P;
	val +=  f->a[3]*H;
	val +=  f->a[4]*L*P;
	val +=  f->a[5]*L*H;
	val +=  f->a[6]*P*H;
	val +=  f->a[7]*L*L;
	val +=  f->a[8]*P*P;
	val +=  f->a[9]*H*H;
	val +=  f->a[10]*P*L*H;
	val +=  f->a[11]*L*L*L;
	val +=  f->a[12]*L*P*P;
	val +=  f->a[13]*L*H*H;
	val +=  f->a[14]*L*L*P;
	val +=  f->a[15]*P*P*P;
	val +=  f->a[16]*P*H*H;
	val +=  f->a[17]*L*L*H;
	val +=  f->a[18]*P*P*H;
	val +=  f->a[19]*H*H*H;

	return( val );
}


#ifdef _AA_
void	
poly3d3_derivative_x( poly3d3_type *f, poly3d3_type *fx )
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
poly3d3_derivative_y( poly3d3_type *f, poly3d3_type *fy )
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
#endif

