/*************************
 ***   Poly1d4Type.c   ***
 *************************/
#include	<stdio.h>
#include	<math.h>

#include "Uigp/igp.h"
#include	"Poly1d4Type.h"



void	
poly1d4_zero( poly1d4_type *f )
{
	f->a0 = 0;

	f->a1 = 0;

	f->a2 = 0;

	f->a3 = 0;

	f->a4 = 0;
}





void	
poly1d4_derivative( poly1d4_type *f, poly1d4_type *fx )
{
	fx->a0 = f->a1;

	fx->a1 = 2*f->a2;
	fx->a2 = 3*f->a3;

	fx->a3 = 4*f->a4;

	fx->a4 = 0;
}




void
poly1d4_shift( poly1d4_type *f, double x, poly1d4_type *g )
{
}



void	
poly1d4_max_diff( poly1d4_type *f0, poly1d4_type *f1, poly1d4_type *mf  )
{
double	tmp;
int	i;

	for( i = 0 ; i < POLY1D4_COEFF ; i++ ){
		tmp = f1->a[i] - f0->a[i];
		if( tmp < 0 )	tmp = -tmp;
		if( tmp > mf->a[i] )	mf->a[i] = tmp;
	}
}


void	
poly1d4_print( poly1d4_type *f, FILE *fp )
{
int	i;

	for( i = 0 ; i < POLY1D4_COEFF ; i++ ){
		fprintf( fp, "   %f", f->a[i] );
	}

	fprintf( fp, "\n" );
}



double	
poly1d4_curvtur( poly1d4_type *f, double x0 )
{
poly1d4_type f1;
vec2d_type	v;
double	t,	b;

	// move to the origin 
	f1.a0 = 0;
	f1.a1 = f->a1 + 2*f->a2*x0;
	f1.a2 = f->a2;


	v.x = 1;
	v.y = f1.a1;

	t = hypot( v.x, v.y );
	v.x /= t;
	v.y /= t;

//	a = f1.a1 * v.y - v.x;
	b = f1.a2 * v.x*v.x;

	return( b );
}