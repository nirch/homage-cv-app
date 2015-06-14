#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"

#include	"Umath/Matrix2Type.h"
#include	"Umath/Matrix3Type.h"


#include	"Ucamera/Pt2dType.h"





static int	pt2d_approximate_line_direction( pt2dA_type *apt, int i0, int di, float dis, vec2f_type *p, vec2f_type *v );




// order 1
int
pt2d_approximate_line( pt2dA_type *apt, int i0, int di, vec2d *p0, float dis, vec2f_type *p, vec2f_type *v )
{
matrix2_type	m;
vec2d_type	D,	X;
float	sx,	sy,	sxx,	sxy,	syy,	s,	x,	y;
float	dis2,	t;
float	A,	B,	C;
vec2f_type	u;
int	i;


	dis2 = dis*dis;
	sx = sy = sxx = sxy = syy = s = 0;

	for( i = i0-di ; i <= i0+di ; i++ ){
		if( i < 0 || i > apt->nP )	continue;


		x = apt->p[i].p.x - p0->x;
		y = apt->p[i].p.y - p0->y;

		if( x*x + y*y > dis2 )
			continue;


		sx += x;
		sxx += x*x;
		sxy += x*y;
		syy += y*y;
		sy += y;
		s += 1;
	}


	B = 1;
	m.a00 = sxx;
	m.a01 = sx;
	m.a10 = sx;
	m.a11 = s;

	D.x = -sxy;
	D.y = -sy;

	matrix2S_solve( &m, &D, &X  );
	A = X.x;
	C = X.y;

	if( ABS(A) > 2 ){
		A = 1;
		m.a00 = syy;
		m.a01 = sy;
		m.a10 = sy;
		m.a11 = s;

		D.x = -sxy;
		D.y = -sx;

		matrix2S_solve( &m, &D, &X  );
		B = X.x;
		C = X.y;
	}




	t = hypot( A,B );

	A /= t;
	B /= t;
	C /= t;

	u.x = A;
	u.y = B;

	t = -C;// - (p0->x*v->x + p0->y*v->y );

	p->x = p0->x + t *u.x;
	p->y = p0->y + t *u.y;

	v->x =  u.y;
	v->y = -u.x;


	pt2d_approximate_line_direction(  apt, i0, di, dis, p,  v );

	return( s );
}





static int
pt2d_approximate_line_direction( pt2dA_type *apt, int i0, int di, float dis, vec2f_type *p, vec2f_type *v )
{
float	dis2,	t;
float	x,	y,	tmin,	tmax;
int	i;
int	n,	s;


	dis2 = dis*dis;
	
	s = n = 0;
	tmin = tmax = 0;
	for( i = i0-di ; i <= i0+di ; i++ ){
		if( i < 0 || i > apt->nP )	continue;


		x = apt->p[i].p.x - p->x;
		y = apt->p[i].p.y - p->y;

		if( x*x + y*y > dis2 )
			continue;

		t = x * v->x + y * v->y;

		if( t < tmin )	tmin = t;
		if( t < tmax )	tmax = t;


		n++;
		if( i > i0 && t > 0 || i < i0 && t < 0 )
			s++;
	}


	if( s < 0.5*n ){
		v->x = -v->x;
		v->y = -v->y;
	}

	return( n );
}




// order 3
int
pt2d_approximate_line3( pt2dA_type *apt, int i0, int di, vec2d *p0, vec2d *u0, float dis, vec2f_type *p, vec2f_type *v )
{
matrix3_type	m;
vec3d_type	D,	X;
float	x,	y;
float	t,	u,	t2;
float	A,	B,	C;
int	i;

vec2d	v0;


	v0.x = u0->y;
	v0.y = -u0->x;


	

	matrix3_zero( &m );
	D.x = D.y = D.z = 0;


	for( i = i0-di ; i <= i0+di ; i++ ){
		if( i < 0 || i > apt->nP )	continue;


		x = apt->p[i].p.x - p0->x;
		y = apt->p[i].p.y - p0->y;


		t = v0.x * x + v0.y * y;

		if( ABS(t) > dis )
			continue;

		u = u0->x * x + u0->y * y;

		if( ABS(u) > 20 )
			continue;


		t2 = t*t;

		m.a00 += t2*t2;
		m.a01 += t*t2;
		m.a02 += t2;

		m.a12 += t;

		m.a22 += 1;


		D.x += u*t2;
		D.y += u*t;
		D.z += u;
	}

	m.a10 = m.a01;
	m.a11 = m.a02;
	m.a20 = m.a02;
	m.a21 = m.a12;


	if( m.a22 <= 4 )
		return( m.a22 );



	matrix3S_solve( &m, &D, &X  );

	A = X.x;
	B = X.y;
	C = X.z;


	p->x = p0->x + C *u0->x;
	p->y = p0->y + C *u0->y;


	v->x = v0.x + B*u0->x;
	v->y = v0.y + B*u0->y;

	t = hypot( v->x, v->y );
	v->x /= t;
	v->y /= t;





	pt2d_approximate_line_direction(  apt, i0, di, dis, p,  v );

	return( m.a22 );
}



