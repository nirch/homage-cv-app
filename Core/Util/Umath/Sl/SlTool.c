/*********************
 ***   SlTool.h   ***
 *********************/
#include <stdio.h>
#include <math.h>

#include "Uigp/igp.h"
#include "SlType.h"

#include "Umath/Matrix2Type.h"



void
sl_set( sl_type *sl, vec2f_type *p0, vec2f_type *p1 )
{
	vec2f_type	v;

	v.x = p1->x - p0->x;
	v.y = p1->y - p0->y;
	VEC2D_NORMALIZE( v );

	VEC2D_RIGHT( v, sl->n );

	sl->d = - VEC2D_INNER( sl->n, *p0 );
}



void
sl_cutpoint( sl_type *sl0, sl_type *sl1, vec2f_type *p )
{
	matrix2_type	m;
	vec2d_type D,	X;

	m.a00 = sl0->n.x;
	m.a01 = sl0->n.y;
	D.x = -sl0->d;

	m.a10 = sl1->n.x;
	m.a11 = sl1->n.y;
	D.y = -sl1->d;

	matrix2_solve( &m, &D, &X );

	p->x = X.x;
	p->y = X.y;
}



