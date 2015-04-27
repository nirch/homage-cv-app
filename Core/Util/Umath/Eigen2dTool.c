#include	<string.h>
#include	<stdio.h>
#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif


#include	"Uigp/igp.h"
#include	"Ulog/Log.h"

#include "Umath/Matrix2Type.h"
#include "LT2Type.h"

#include "EigenType.h"


int
eige2d_swap( eigen2d_type  *se, eigen2d_type *te )
{


	*te = *se;

	te->v1 = se->v2;
	te->e1 = se->e2;

	te->v2 = se->v1;
	te->e2 = se->e1;

	return( 1 );
}




int 
eige2d_lt( eigen2d_type  *se, eigen2d_type *te, float d, lt2_type *lt )
{
	matrix2_type m;
	vec2d_type	D,	X;
	float	r;

	m.a00 = d*se->v1.x;
	m.a01 = d*se->v1.y;
	m.a10 = d*se->v1.y;
	m.a11 = -d*se->v1.x;

	D.x = te->v1.x;
	D.y = te->v1.y;

	matrix2_solve( &m, &D, &X );

	lt->a0 = X.x;
	lt->b0 = X.y;
	lt->a1 = -X.y;
	lt->b1 = X.x;

	// test
//	float a = lt->a0* se->v1.x + lt->b0 * se->v1.y;
//	float c = lt->a1* se->v1.x + lt->b1 * se->v1.y;
	// test

	r = sqrt(te->e1 / se->e1);
	lt->a0 *= r;
	lt->b0 *= r;
	lt->a1 *= r;
	lt->b1 *= r;

	lt->c0 = te->p.x - ( lt->a0 * se->p.x + lt->b0 * se->p.y );
	lt->c1 = te->p.y - ( lt->a1 * se->p.x + lt->b1 * se->p.y );

//	a = LT2_F1( *lt, se->p.x, se->p.y );
//	c = LT2_F2( *lt, se->p.x, se->p.y );


	return( 1 );
}


