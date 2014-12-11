/***********************
 ***  PlnParallel.c  ***
 ***********************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"


#include "Uln/PlnType.h"
#include "Uvl/Vl2fType.h"


int
pln_parallel2(  pln_type *bpl, pln_type *pl, int i, float aT, float T0, float T1, pPln_type ap[] )
{
	int	n = 0;

	if( pln_parallel( bpl, pl, i, 0, aT, T0 , T1, &ap[n] ) > 0 )
		n++;

	if( pln_parallel( bpl, pl, i, pl->len, aT, T0 , T1, &ap[n] ) > 0 )
		n++;


	if( n == 0 )	return( 0 );

	if( ap[0].d < 0 ){
		pln_inverse( pl );
		pPln_inverse( &ap[0] );

		if( n == 2 )
			pPln_inverse( &ap[1] );

	}


	return( n );
}



int
pln_parallel( pln_type *bpl, pln_type *pl, int i, float gt, float aT, float T0, float T1, pPln_type *pp )
{
	vec2f_type	p0,	v0;
	vec2f_type	p1,	v1;
	float	d0,	d1;
	dPln_type	d;
	vec2f_type	p;

	float	t,	u;


	pln_gt2p( pl, gt, &p );

	if( pln_distance( bpl, &p, &d ) < 0 )
		return( -1 );

	if( d.gt < 0 || d.gt > bpl->len )
		return( -1 );

	if( ABS(d.u) < T0 || ABS(d.u) > T1 )
		return( -1 );


	pln_pv( pl, gt, 2.0, 10, &p0, &v0, &d0 );

	pln_pv( bpl, d.gt, 2.0, 10, &p1, &v1, &d1 );



	u = (p0.x - p1.x) * v1.y - (p0.y - p1.y) * v1.x;


	t = v0.x*v1.x + v0.y*v1.y;
	if( ABS(t) < aT )
		return( -1 );

	pp->pl = pl;
	pp->iPl = i;

	pp->sgt = gt;

	pp->gt = d.gt;
	pp->u = d.u;
	pp->u = u;
	pp->v = v0;

	pp->p = p;

	pp->d = t;

	return( 1 );
}



int
pln_parallel_distance(  pln_type *bpl, pln_type *pl, float dt, float aT, float D0, float D1, float *u )
{
float	gt;
pPln_type	p;
float	su;
float	n;



	su = 0;
	n = 0;
	for( gt = 0 ; gt < pl->len ; gt += dt ){
//		if( pln_parallel( bpl, pl, 0, gt, aT, 8 , 1.5*196, &p ) > 0 ){
		if( pln_parallel( bpl, pl, 0, gt, aT, D0 , D1, &p ) > 0 ){

			su += p.u;
			n++;
		}
	}

	if( n <= 0 )	return( -1 );

	*u = su / n;


	return( n );
}



void
pPln_reorder( pPln_type a[], int n )
{
	int	i,	j;

	for( i = 1 ; i < n ; i++ ){

		pPln_type p = a[i];
		for( j = i ; j > 0 ; j-- ){
			if( a[j-1].gt < p.gt )	break;
			a[j] = a[j-1];
		}

		a[j] = p;
	}
}


void
pPln_inverse( pPln_type *p )
{
	p->v.x = -p->v.x;
	p->v.y = -p->v.y;

	p->sgt = p->pl->len - p->sgt;

	p->d = -p->d;
}



float
pln_straightline_measure( pln_type *pl, float gt, float dt, int n )
{
pt2dA_type	*apt;
vl2f_type	vl;
vec2f_type	dv;
float	u,	su;
int	i;


	apt = pt2dA_alloc( 2*n+1 );

	pln_sample( pl, gt -n*dt, dt, 2*n + 1, 0, apt );


	pt2d_approximate_line_vl( apt, 0, apt->nP, &vl );



	su = 0;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		dv.x = pt->p.x - vl.p.x;
		dv.y = pt->p.y - vl.p.y;

		u = dv.x * vl.v.y - dv.y * vl.v.x;
		if( u < 0 )	u = -u;

		su += u;
	}

	su /= apt->nP;

	pt2dA_destroy( apt );

	return( su );
}


int
pln_straightline( pln_type *pl, float gt0, float gt1, float dt, vl2f_type *vl )
{
	pt2dA_type	*apt;
	

	apt = pln_sampleP( pl, gt0, gt1, dt, NULL );


	if( pt2d_approximate_line_vl( apt, 0, apt->nP, vl ) < 0 ){
		pt2dA_destroy( apt );
		return( -1 );
	}

	pt2dA_destroy( apt );

	if( ABS( vl->e) > 0.125 )
		return( -1 );

	return( 1 );
}