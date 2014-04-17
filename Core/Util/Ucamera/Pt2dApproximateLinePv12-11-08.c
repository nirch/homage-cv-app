/***********************************
 ***    Pt2ApproximateLinePv.c   ***
 ***********************************/


#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"

#include	"Umath/Matrix2Type.h"

#include "Uvl/Vl2fType.h"

#include	"Ucamera/Pt2dType.h"


int
pt2d_approximate_line_vl( pt2dA_type *apt, int i0, int i1, vl2f_type *vl )
{
int	ret;

	ret = pt2d_approximate_line_pv( apt, i0, i1, &vl->p, &vl->v, &vl->d );

	return( ret );
}



int
pt2d_approximate_line_pv( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float *d )
{
matrix2_type	m;
vec2d_type	D;
float	x,	y;
double	sx,	sy,	sxx,	sxy,	syy;
int	i,	n;

float	e1,	e2;
float	t0,	t1;


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


	// direction
	t0 = (apt->p[i0].p.x - p->x)*v->x + (apt->p[i0].p.y - p->y)*v->y;
	t1 = (apt->p[i1-1].p.x - p->x)*v->x + (apt->p[i1-1].p.y - p->y)*v->y;

	*d = 0.5*(t1 - t0);

	if( t1 < t0 ){
		v->x = -v->x;
		v->y = -v->y;

		*d = - *d;
	}


	return( n );
}



float
pt2d_approximate_line_pv_test( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float D, int *nD )
{
vec2d_type	u;
int	i;

float	d0,	d,	dMax;



	VEC2D_LEFT( *v, u );

	d0 = p->x * u.x + p->y * u.y;

	*nD = 0;
	dMax = 0;
	for( i = i0 ; i < i1 ; i++ ){
		pt2d_type	*pt = &apt->p[i];

		d = pt->p.x * u.x + pt->p.y * u.y - d0;
	
		pt->r = d;

		if( ABS(d) > D )	(*nD)++;

		if( ABS(d) > ABS(dMax) ){
			dMax = d;
		}
	}

	return( dMax );
}






typedef struct slT_type {
	int	i0;
	int	i1;

	float m;
	float sm;
	int	iM;
} slT_type;


int
pt2d_approximate_line_vl_split( pt2dA_type *apt, int i0, int i1, vl2f_type *vl, float *m, float *sm )
{
int	ret;

	ret = pt2d_approximate_line_pv_split( apt, i0, i1, &vl->p, &vl->v, m, sm );

	return( ret );

}


static int	pt2d_approximate_line_pv_split0( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float *m, float *sm );

int
pt2d_approximate_line_pv_split( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float *m, float *sm )
{
	pt2d_type	*pt;
	vec2d_type	u;
	int	i;

	float	d0;
	float	d;
	int		iMax;

	slT_type	aS[100],	*sl;
	int	nS;
	int	ret;


	
	ret = pt2d_approximate_line_pv_split0( apt, i0, i1, p,  v, m, sm );
	if( ret >= 0 )	return( ret );


	VEC2D_LEFT( *v, u );

	d0 = p->x * u.x + p->y * u.y;


	nS = 0;
	sl = &aS[nS++];

	pt = &apt->p[i0];
	sl->m = pt->p.x * u.x + pt->p.y * u.y - d0;
	sl->sm = sl->m;
	sl->i0 = sl->i1 = sl->iM = i0;


	for( i = i0+2 ; i < i1-1 ; i++ ){
		pt2d_type	*pt = &apt->p[i];

		d = pt->p.x * u.x + pt->p.y * u.y - d0 ;

		if( (sl->m < 0) == ( d < 0 ) ){

			if( ABS(d) > ABS(sl->m) ){
				sl->m = d;
				sl->iM = i;
			}

			sl->sm += d;
			sl->i1 = i;
			continue;
		}

	
		sl = &aS[nS++];
		sl->m = d;
		sl->sm = d;
		sl->i0 = sl->i1 = sl->iM = i;
	}


	if( nS == 1 ){
		*sm =  aS[0].sm;
		*m =  aS[0].m;
		return( aS[0].iM );
	}

	if( nS == 2 ){
		iMax = (  ABS(aS[0].m) > ABS(aS[1].m) )? 0 : 1;
		
		*sm =  aS[iMax].sm;
		*m =  aS[iMax].m;
		return( aS[iMax].iM );
	}
	
	iMax = 1;
	for( i = 2 ; i < nS -1 ; i++ ){
		if( ABS(aS[iMax].m) > ABS(aS[iMax].m) )
			iMax = i;
	}

	*sm =  aS[iMax].sm;
	*m =  aS[iMax].m;
	return( aS[iMax].iM );
}



static int
pt2d_approximate_line_pv_split0( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float *m, float *sm )
{
pt2d_type	*pt;
float	t0,	t1,	t;
int	i,	j0,	j1,	d0,	d1;

	pt = &apt->p[i0];
	t0 = ( pt->p.x - p->x )*v->x + ( pt->p.y - p->y )*v->y;

	pt = &apt->p[i1];
	t1 = ( pt->p.x - p->x )*v->x + ( pt->p.y - p->y )*v->y;

	if( (t0 < 0) != (t1<0) )
		return( -1 );


	t0 = t1 = 0;
	j0 = j1 = 0;
	for( i = i0 ; i < i1 ; i++ ){
		pt = &apt->p[i];
		t = ( pt->p.x - p->x )*v->x + ( pt->p.y - p->y )*v->y;

		if( t < t0 ){
			t0 = t;
			j0 = i;
			continue;
		}

		if( t > t1 ){
			t1 = t;
			j1 = i;
			continue;
		}
	}


	d0 = MIN( j0, i1-j0 );
	d1 = MIN( j1, i1-j1 );

	if( d0 > d1 )	return( j0 );
	return( j1 );
}

