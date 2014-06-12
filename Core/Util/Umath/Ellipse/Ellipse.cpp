#include <math.h>

#include "Uigp/igp.h"

#include "EllipseType.h"

#include "Umath/EigenType.h"
#include "Umath/Matrix2Type.h"


ellipse_type *
ellipse_alloc()
{
	ellipse_type *e = ( ellipse_type *)malloc( sizeof(ellipse_type) );

	return( e );
}

void
ellipse_destroy( ellipse_type *e )
{

	free( e );
}


void
ellipse_set( ellipse_type *e, eigen2d_type *eg )
{
	e->p = eg->p;

	e->v = eg->v1;


	e->rx = 2.0*sqrt(eg->e1);
	e->ry = 2.0*sqrt(eg->e2);

	e->s = M_PI * e->rx*e->ry;
}


void
ellipse_scale( ellipse_type *e, float scale )
{
	e->p.x *= scale;
	e->p.y *= scale;
	
	e->rx *= scale;
	e->ry *= scale;

	e->s *= scale*scale;
}


float
ellipse_distance( ellipse_type *e, vec2f_type *p )
{
	vec2f_type	dp,	u;

	VEC2D_RIGHT( e->v, u );
	dp.x = p->x - e->p.x;
	dp.y = p->y - e->p.y;

	float x = VEC2D_INNER( dp, e->v );
	float y = VEC2D_INNER( dp, u );

	float t = x*x /(e->rx*e->rx) + y*y/(e->ry*e->ry);

	return( t );
}


float
ellipse_distance( ellipse_type *be, ellipse_type *e )
{
	float	t,	tMin;
	vec2f_type	p;

	tMin = ellipse_distance( be, &e->p );


	p.x = e->p.x + e->rx * e->v.x;
	p.y = e->p.y + e->rx * e->v.y;
	t = ellipse_distance( be, &p );
	if( t < tMin )	tMin = t;

	p.x = e->p.x - e->rx * e->v.x;
	p.y = e->p.y - e->rx * e->v.y;
	t = ellipse_distance( be, &p );
	if( t < tMin )	tMin = t;


	vec2f_type	u;
	VEC2D_RIGHT( e->v, u );

	p.x = e->p.x + e->ry * u.x;
	p.y = e->p.y + e->ry * u.y;
	t = ellipse_distance( be, &p );
	if( t < tMin )	tMin = t;

	p.x = e->p.x - e->ry * u.x;
	p.y = e->p.y - e->ry * u.y;
	t = ellipse_distance( be, &p );
	if( t < tMin )	tMin = t;


	return( tMin );
}




float
ellipse_distanceN( ellipse_type *be, ellipse_type *e, int N )
{
	float	t,	tMin;
	vec2f_type	p,	p1;
//	int N = 16;

	tMin = ellipse_distance( be, &e->p );

	vec2f_type	u;
	VEC2D_RIGHT( e->v, u );



	int i;
	for( i = 0 ; i < N ; i++ ){
		float a = i*2*M_PI/(float)N;

		p1.x = e->rx*cos(a);
		p1.y = e->ry*sin(a);

		p.x = e->p.x + p1.x * e->v.x + p1.y* u.x;
		p.y = e->p.y + p1.x * e->v.y + p1.y* u.y;

		t = ellipse_distance( be, &p );

		if( t < tMin )	tMin = t;
	}

	if( tMin < 1 )
		return( 0 );

	t = (sqrt( tMin ) -1 )*be->rx;

	return( t );
}



int
ellipse_union( ellipse_type *e0, float w0, ellipse_type *e1, float w1, ellipse_type *e )
{


	matrix2_type	m0;
	matrix2S_eigen_inv( &m0, 0.25*e0->rx*e0->rx, &e0->v, 0.25*e0->ry*e0->ry );

	matrix2_type	m1;
	matrix2S_eigen_inv( &m1, 0.25*e1->rx*e1->rx, &e1->v, 0.25*e1->ry*e1->ry );


	e->p.x = (w0*e0->p.x + w1*e1->p.x )/( w0 + w1 );
	e->p.y = (w0*e0->p.y + w1*e1->p.y )/( w0 + w1 );

	matrix2_type	m;
	float a0 = m0.a00 + e0->p.x*e0->p.x;
	float a1 = m1.a00 + e1->p.x*e1->p.x;
	m.a00 = (w0* a0 + w1* a1 )/( w0 + w1 )  - e->p.x *e->p.x;

	a0 = m0.a01 + e0->p.x*e0->p.y;
	a1 = m1.a01 + e1->p.x*e1->p.y;
	m.a01 = (w0* a0 + w1* a1 )/( w0 + w1 )  - e->p.x *e->p.y;

	a0 = m0.a11 + e0->p.y*e0->p.y;
	a1 = m1.a11 + e1->p.y*e1->p.y;
	m.a11 = (w0* a0 + w1* a1 )/( w0 + w1 )  - e->p.y *e->p.y;

	m.a10 = m.a01;



	matrix2S_eigen( &m, &e->rx, &e->v, &e->ry );

	e->rx = 2.0*sqrt(e->rx);
	e->ry = 2.0*sqrt(e->ry);

	return( 1 );
}
