// WndInner.cpp : implementation file
//

#include <stdio.h>
#include <math.h>

#define _GPLOG

#ifdef _DEBUG
#define _DUMP
#endif

#include "Uigp/igp.h"
#include "Ulog/Log.h"
#include "Umath/Matrix2Type.h"
#include "Umath/EigenType.h"


#include "PlnType.h"


static int	pln_eigen_update( pln_type *pl, float gt0, float gt1, float dt, eigen2dAux_type *ea );



int
pln_eigen( pln_type *pl, float dt, eigen2d_type *e )
{
	matrix2_type	m;
	vec2f_type	p;
	float	x,	y;
	double	sx,	sy,	sxx,	sxy,	syy;
	int	n;
	float	t;



	sx = sy = sxx = sxy = syy = 0;


	for( t = 0, n = 0 ; t < pl->len ; t += dt ){

		pln_gt2p( pl, t, &p );


		x = p.x;
		y = p.y;


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


	e->p.x = sx;
	e->p.y = sy;



	matrix2S_eigen( &m, &e->e1, &e->v1, &e->e2 );

	VEC2D_RIGHT( e->v1, e->v2 );


	e->r = e->e2/ e->e1;



	return( n );
}



int
pln_eigenS( pln_type *pl, float gt0, float gt1, float dt, eigen2d_type *e )
{
	matrix2_type	m;
	vec2f_type	p;
	float	x,	y;
	double	sx,	sy,	sxx,	sxy,	syy;
	int	n;
	float	t;



	sx = sy = sxx = sxy = syy = 0;


	if( gt0 < 0 )	gt0 = 0;
	if( gt1 > pl->len )	gt1 = pl->len;

	for( t = gt0, n = 0 ; t < gt1 ; t += dt ){

		pln_gt2p( pl, t, &p );


		x = p.x;
		y = p.y;


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


	e->p.x = sx;
	e->p.y = sy;



	matrix2S_eigen( &m, &e->e1, &e->v1, &e->e2 );

	VEC2D_RIGHT( e->v1, e->v2 );


	e->r = e->e2/ e->e1;



	return( n );
}



int
plnA_eigen( plnA_type *apl, eigen2d_type *e )
{
eigen2dAux_type ea;
int	i;

	eigen2dAux_init( &ea, 1 );


	for( i = 0; i < apl->nA ; i++ )
		pln_eigen_update( apl->a[i], 0, 0, 1.0, &ea );

	int ret = eigen2dAux_final( &ea, e );
		

	return( ret );
}


static int
pln_eigen_update( pln_type *pl, float gt0, float gt1, float dt, eigen2dAux_type *ea )
{

	vec2f_type	p;
	int	n;
	float	t;



	
	if( gt1 <= 0 )	gt1 = pl->len;


	for( t = gt0, n = 0 ; t < gt1 ; t += dt ){

		pln_gt2p( pl, t, &p );


		EIGEN2D_UPDATE( *ea, p.x, p.y );

		n++;
	}




	return( n );
}



pt2dA_type *
pln_apt( pln_type *pl, float dt )
{

	vec2f_type	p;
	float	t;



	pt2dA_type *apt = pt2dA_alloc( pl->len /dt + 1 );



	for( t = 0 ; t < pl->len ; t += dt ){

		pln_gt2p( pl, t, &p );




		pt2d_type *pt = &apt->a[apt->nA++];

		pt->p = p;

	}


	return( apt );


}




void
	plnA_eigen_axis( plnA_type *apl )
{

	eigen2d_type e;
	plnA_eigen( apl, &e );




	{  //dump
		pt2dA_type *apt = pt2dA_alloc( 4 );
		apt->axis = PT2D_AXIS_YX;
		apt->type = PT2D_4V;

		pt2d_type *pt = &apt->a[apt->nA++];
		pt->p = e.p;
		pt->n = e.v1;

		PT2DA_DUMP( apt, "F", 1, NULL );
		pt2dA_destroy( apt );
	}



	apl->p.x = e.p.x;
	apl->p.y = e.p.y;

	apl->angle = atan2( e.v1.y, e.v1.x );
	apl->angle = apl->angle;

	apl->scale = 1.0;

	box2f_type	b;
	plnA_boxV( apl, &apl->p, &e.v1, &b );

	float x = 0.5*(b.x1 - b.x0);
	float y = 0.5*(b.y1 - b.y0);

	float dx = 0.5*(b.x1 + b.x0);
	float dy = 0.5*(b.y1 + b.y0);


	apl->p.x += dx*e.v1.x - dy * e.v1.y;
	apl->p.y += dx*e.v1.y + dy * e.v1.x;
	//plnA_boxV( apl, &apl->p, &e.v1, &b );



//	GPLOG( ( "%.2f  %.2f\n", 2*x, 2*y ) );

}
