#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"

#include	"Umath/Matrix2Type.h"

#include	"Ucamera/Pt2dType.h"



int
pt2d_approximate_Ax_B( pt2dA_type *apt, int i0, int i1, float *a, float *b )
{
matrix2_type	m;
vec2d_type	D,	X;
float	x,	y;
int	i;


	pt2dA_dump( apt, "axb", 1, NULL );

	matrix2_zero( &m );
	D.x = D.y = 0;


	if( i0 < 0 )
		i0 = 0;

	if( i1 > apt->nP )
		i1 = apt->nP;


	for( i = i0 ; i < i1 ; i++ ){


		x = apt->p[i].p.x;
		y = apt->p[i].p.y;

	
		m.a00 += x*x;
		m.a01 += x;

		m.a11 += 1;


		D.x += y*x;
		D.y += y;
	}

	m.a10 = m.a01;




	matrix2S_solve( &m, &D, &X  );

	*a = X.x;
	*b = X.y;

	return( m.a11 );
}



float
pt2d_approximate_Ax_B_test( pt2dA_type *apt, int i0, int i1, float a, float b )
{
	float	x,	y,	dy,	sum;
	int	i;


	if( i0 < 0 )
		i0 = 0;

	if( i1 > apt->nP )
		i1 = apt->nP;

	sum = 0;
	for( i = i0 ; i < i1 ; i++ ){


		x = apt->p[i].p.x;
		y = apt->p[i].p.y;

		dy = a*x + b - y;

		if( dy < 0 )
			dy = -dy;

		sum += dy;
	}

	sum /= ( i1-i0);

	return( sum );
}



int
pt2d_approximate_line_pn( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v )
{
	matrix2_type	m;
	vec2d_type	D;
	float	x,	y;
	double	sx,	sy,	sxx,	sxy,	syy;
	int	i,	n;

	float	e1,	e2;
//	vec2f_type	ev;
	float	t0,	t1;


//	pt2dA_dump( apt, "axb", 1, NULL );

	matrix2_zero( &m );
	D.x = D.y = 0;


	if( i0 < 0 )
		i0 = 0;

	if( i1 > apt->nP )
		i1 = apt->nP;

	sx = sy = sxx = sxy = syy = 0;


	for( i = i0, n = 0 ; i < i1 ; i++ ){


		x = apt->p[i].p.x;
		y = apt->p[i].p.y;


		sx += x;
		sy += y;

		sxx += x*x;
		sxy += x*y;
		syy += y*y;
		n++;
	}

	sx /= n;
	sy /= n;

	m.a00 = sxx / n - sx*sx;
	m.a01 = sxy / n - sx*sy;
	m.a11 = syy / n - sy*sy;
	m.a10 = m.a01;


	p->x = sx;
	p->y = sy;



	matrix2S_eigen( &m, &e1, v, &e2 );

//	v->x = ev.y;
//	v->y = -ev.x;



	t0 = (apt->p[i0].p.x - p->x)*v->x + (apt->p[i0].p.y - p->y)*v->y;
	t1 = (apt->p[i1-1].p.x - p->x)*v->x + (apt->p[i1-1].p.y - p->y)*v->y;

	if( t1 < t0 ){
		v->x = -v->x;
		v->y = -v->y;
	}



	return( n );
}

