/*******************************
 ***   Pt2dApproximateAb.c   ***
 *******************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"
#include	"Umath/Matrix2Type.h"


#include	"Ucamera/Pt2dType.h"


// y = 0*x + b
int
pt2dA_app_b( pt2dA_type *apt, int i0, int i1, float a0, float b0, float maxY, float *b )
{
	pt2d_type	*pt;
	float	y,	dy;
	float	sum;
	int	i,	n;


	sum = 0;

	for( i = 0, n = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		y = b0;

		dy = y - pt->p.y;

		if( ABS(dy) > maxY )
			continue;

		sum += pt->p.y;// - b0;

		n++;
	}

	if( n < 2 )
		return( -1 );

	*b = sum/n;

	return( n );
}



// y = a*x + b
int
pt2dA_app_ab( pt2dA_type *apt, int i0, int i1, float a0, float b0, float maxY, float *a, float *b )
{
matrix2_type	m;
vec2d_type	D,	X;
pt2d_type	*pt;
float	y,	dy;
int	i,	n;

	m.a00 = m.a01 = m.a10 = m.a11 = 0;
	D.x = D.y = 0;

	for( i = i0, n = 0 ; i < i1 ; i++ ){
		pt = &apt->p[i];

		y = a0 *pt->p.x + b0;

		dy = y - pt->p.y;

		if( ABS(dy) > maxY )
			continue;

		m.a00 += pt->p.x*pt->p.x;
		m.a01 += pt->p.x;
		m.a11 += 1;

		D.x += pt->p.x*pt->p.y;
		D.y += pt->p.y;

		n++;
	}

	if( n < 2 )
		return( -1 );

	m.a10 = m.a01;

	matrix2S_solve( &m, &D, &X );

	*a = X.x;
	*b = X.y;

	return( n );
}



