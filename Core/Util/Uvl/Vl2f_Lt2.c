

#include	<math.h>



#ifdef _DEBUG
#define _DUMP
#endif


#include "Uigp/igp.h"
#include "Ulog/Log.h"
#include "Umath/LT2Type.h"
#include "Umath/Matrix2Type.h"


#include "Vl2fType.h"



	// transform s0 to t0 &&  s1 to t1
int
vl2f_lt2( vl2f_type *s0, vl2f_type *s1, vl2f_type *t0, vl2f_type *t1, lt2_type *lt )
{
	matrix2_type	A,	B,	C;
	vec2f_type	p,	p1;
	float	d;
	vec2f_type	u;
	matrix2_type	m;
	vec2d_type	D,	X;
	float	t;

	// transcform s0->v   to (1,0 )		and s10>v to ( 0, 1 )
	d = -s1->v.y * s0->v.x + s1->v.x * s0->v.y;
	A.a00 = -s1->v.y / d;
	A.a01 = s1->v.x / d;

	d = -s0->v.y * s1->v.x + s0->v.x * s1->v.y;
	A.a10 = -s0->v.y / d;
	A.a11 = s0->v.x / d;


	// test1
	matrix2_multV( &A, &s0->v, &p );
	matrix2_multV( &A, &s1->v, &p );




	// transform  (1,0)	to  t->v   and   (0,1)  to  t1->v
	t = t0->d / s0->d;
	//t = 1.0;
	B.a00 = t0->v.x * t;
	B.a10 = t0->v.y * t;
	t = t1->d / s1->d;
	//t = 1.0;
	B.a01 = t1->v.x;
	B.a11 = t1->v.y;


	// test 2
	p1.x = 1; p1.y = 0;
	matrix2_multV( &B, &p1, &p );

	p1.x = 0; p1.y = 1;
	matrix2_multV( &B, &p1, &p );




	matrix2_mult( &B, &A, &C );
	lt->a0 = C.a00;
	lt->b0 = C.a01;

	lt->a1 = C.a10;
	lt->b1 = C.a11;



	VEC2D_LEFT( t0->v, u );
	p.x = t0->p.x - ( s0->p.x * lt->a0 + s0->p.y * lt->b0 );
	p.y = t0->p.y - ( s0->p.x * lt->a1 + s0->p.y * lt->b1 );

	m.a00 = u.x;
	m.a01 = u.y;
	D.x = p.x*u.x + p.y*u.y;


	VEC2D_LEFT( t1->v, u );
	p.x = t1->p.x - ( s1->p.x * lt->a0 + s1->p.y * lt->b0 );
	p.y = t1->p.y - ( s1->p.x * lt->a1 + s1->p.y * lt->b1 );

	m.a10 = u.x;
	m.a11 = u.y;
	D.y = p.x*u.x + p.y*u.y;

	matrix2_solve( &m, &D, &X );

	lt->c0 = X.x;
	lt->c1 = X.y;



	// test 3
	p.x = LT2_F1( *lt, s0->p.x, s0->p.y );
	p.y = LT2_F2( *lt, s0->p.x, s0->p.y );


	d  = vl2f_distance( t0, &p );

	p.x = LT2_F1( *lt, s1->p.x, s1->p.y );
	p.y = LT2_F2( *lt, s1->p.x, s1->p.y );


	d  = vl2f_distance( t1, &p );


	return( 1 );
}






void
vl2f_compute_lt2( vl2f_type *vl, lt2_type *lt )
{
	vec2f_type	p0,	p1,	q0,	q1;

	vl2f_points( vl, &p0, &p1 );

	LT2_F( *lt, p0, q0 );
	LT2_F( *lt, p1, q1 );

	vl2f_set( vl, &q0, &q1 );
}