/****************************
 ***   ArDetectLine.cpp   ***
 ****************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
//#define _DUMP 
#endif



#include	"Uigp/igp.h"
#include "Ulog/Log.h"
#include "Uvec/Vec3d.h"
#include "Uln/PlnType.h"
#include "Umath/Matrix2Type.h"

#include "Ucamera/Pt2dType.h"
#include "ImageType/ImageType.h"
#include "ImageMark/ImageMark.h"
#include "ImageDump/ImageDump.h"


#include "pEdgeType.h"
#include "plEdgeType.h"




static int	image_pEdge_line_1( image_type *rim, pEdge_type *dp00, pEdgePrm_type *prm, plEdge_type **ple );

static int	pEdge_line_approximate( pEdge_type *ad[200], int nD, pEdgePrm_type *prm, plEdge_type **ple );


static int	pt2dA_line_approximate( pt2dA_type *apt,  pln_type **pl, int iLine );

static int	pt2dA_line_approximate( pt2dA_type *apt, int i0, int i1, ln_type lnA[] );


static pt2dA_type *	apt_from_pEdge( pEdge_type *ad[200], int nD, pt2dA_type *apt );


pln_type *pln_create_lnA( ln_type al[], int nAl );


static float	pt2d_approximate_line_pv_testT( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float *a, float D, int *nD );




static int iLine;



static int	ln_set( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, ln_type *l );


void	pln_set_lnA( pln_type *pl, ln_type al[], int nAl );



void	ln_update_end( ln_type *l0, ln_type *l1 );




int
pEdge_line( image_type *dim, int d, pEdgePrm_type *prm, plEdgeA_type *aPl )
{
	pEdge_type	*dp;
	int	i,	j,	n;

	iLine = 0;


	if( prm->straightLine == 1 ){
		int ret = pEdge_straight_line( dim, d, prm, aPl );
		return( ret );
	}



	dp = (pEdge_type *)dim->data;
	for( i = d ; i < dim->height-d ; i++ ){
		dp = (pEdge_type *)IMAGE_PIXEL( dim, i, d );
		for( j = d ; j < dim->width-d ; j++, dp++ ){
			if( dp->state != 1 )	continue;
			if( dp->ng[0] >= 0 )	continue;

			if( (n = image_pEdge_line_1( dim, dp, prm, &aPl->a[aPl->nA] )) > 0 ){
				aPl->nA++;
				
				plEdgeA_realloc( aPl, aPl->nA + 100 );
			}

		}
	}

//	plnA_dump( aPl, "aa", 1, "2" );


	return( 1 );
}




static int
image_pEdge_line_1( image_type *rim, pEdge_type *dp00, pEdgePrm_type *prm, plEdge_type **ple )
{
	pEdge_type	*ad[4096];
	int	nD;

	pEdge_type	*rp;
	int	arp[9];
	int	ret;


	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;




	for( rp = dp00, nD = 0 ; rp != NULL ;  ){
		ad[nD++] = rp;

		if( rp->ng[1] < 0 )	break;
		rp = rp + arp[ rp->ng[1] ];
	}


	if( nD < 3 )	return( -1 );


	ret = pEdge_line_approximate( ad, nD, prm, ple );


	return( ret );

}





static int
pEdge_line_approximate( pEdge_type *ad[200], int nD, pEdgePrm_type *prm, plEdge_type **ple )
{
pt2dA_type *apt;
pln_type	*pl;
int	ret,	i;


	apt = apt_from_pEdge( ad, nD, NULL );

	

	ret = pt2dA_line_approximate(  apt, &pl, iLine++ );

	pt2dA_destroy( apt );


	if( pl->len < prm->lenMin ){
		pln_destroy( pl );
		return( -1 );
	}

	float	sum = 0;
	int n = 0;
	for( i = 0 ; i < nD ; i++ ){
		sum += ad[i]->g;

		if( ad[i]->nStatic > 10 )
			n++;
	}


	if( n > 0.75* nD )
		pl->state |= PLN_WEEK;


	*ple = plEdge_alloc();
	(*ple)->grad = sum / nD;
	(*ple)->pl = pl;
	pl->qulity = (*ple)->grad;

//	fprintf( stdout, "%.2f\n", pl->qulity );




	return( ret );

}




static int
pt2dA_line_approximate( pt2dA_type *apt, pln_type **pl, int iLine )
{
	int	i,	n;
	ln_type	lnA[100];

	PT2DA_DUMP( apt, "line-pt", iLine, NULL );

	n = pt2dA_line_approximate( apt, 0, apt->nP-1, lnA );


	for( i = 0 ; i < n-1 ; i++ )
		ln_update_end( &lnA[i], &lnA[i+1] );


	*pl = pln_from_lnA( lnA, n );

	PLN_DUMP( *pl, "line-pt", iLine, NULL );

	return( n );

}

static int
pt2dA_line_approximate( pt2dA_type *apt, int i0, int i1, ln_type lnA[] )
{
	vec2f_type	p,	v;
	float	dMax,	d, e,	a;
	int	no,	i,	n;


	pt2d_approximate_line_pv( apt, i0, i1, &p, &v, &d, &e );

	dMax = pt2d_approximate_line_pv_testT( apt, i0, i1, &p, &v, &a, 2.5, &no );


	if( ABS(dMax) < 2.5 && ABS(a) < 0.5 ){//|| no < 3 ){
		ln_set( apt, i0, i1, &p, &v, &lnA[0] );

		return( 1 );
	}


	float	m,	sm;
	i = pt2d_approximate_line_pv_split( apt, i0, i1, &p, &v, &m, &sm );

	if(  i < i0+3 || i1 -i < 3 ){
		ln_set( apt, i0, i1, &p, &v, &lnA[0] );
		return( 1 );
	}


	n = pt2dA_line_approximate( apt, i0, i, lnA );

	n += pt2dA_line_approximate( apt, i, i1, &lnA[n] );

	return( n );
}



static int
ln_set( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, ln_type *l )
{
pt2d_type	*pt;
float	t,	t0,	t1;
int	i;


	pt = &apt->p[i0];
	t0 = t1 = (pt->p.x - p->x) * v->x + (pt->p.y - p->y ) * v->y;
	for( i = i0+1 ; i <= i1 ; i++ ){
		pt = &apt->p[i];
		t = (pt->p.x - p->x) * v->x + (pt->p.y - p->y ) * v->y;
		if( t < t0 )	t0 = t;
		else	if( t > t1 )	t1 = t;
	}



	l->ctr.y = p->x + t0 * v->x;
	l->ctr.x = p->y + t0 * v->y;
	l->len = t1 - t0;

	l->v.x = l->len*v->y;
	l->v.y = l->len*v->x;


	l->a = l->c_prb = 0;

	l->u.x = v->x;
	l->u.y = -v->y;


	return( 1 );
}


void
ln_update_end( ln_type *l0, ln_type *l1 )
{
	float	t;
	vec2f_type	dp,	p;

	dp.x = l1->ctr.x - l0->ctr.x;
	dp.y = l1->ctr.y - l0->ctr.y;


	t = -( dp.x * l0->u.x + dp.y*l0->u.y )/ (l1->v.x * l0->u.x + l1->v.y * l0->u.y );

	p.x = l1->ctr.x + t * l1->v.x;
	p.y = l1->ctr.y + t * l1->v.y;



	l0->v.x = p.x - l0->ctr.x;
	l0->v.y = p.y - l0->ctr.y;
	l0->len = l0->v.x * -l0->u.y + l0->v.y * l0->u.x;


	l1->v.x = l1->ctr.x + l1->v.x  - p.x;
	l1->v.y = l1->ctr.y + l1->v.y  - p.y;
	l1->len = l1->v.x * -l1->u.y + l1->v.y * l1->u.x;
	l1->ctr = p;
}



static pt2dA_type *
apt_from_pEdge( pEdge_type *ad[200], int nD, pt2dA_type *apt )
{
pEdge_type	*dp;
pt2d_type	*pt;
int	i;


	apt = pt2dA_realloc( apt, nD );

	for( i = 0 ; i < nD ; i++ ){
		dp = ad[i];

		pt = &apt->p[i];

		pt->p = dp->p;
		pt->n = dp->v;
	}

	apt->nP = nD;
	return( apt );
}





static float
pt2d_approximate_line_pv_testT( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float *a, float TD, int *nD )
{
	vec2d_type	u;

	int	i;

	float	d,	dMax;

	matrix2_type m;
	vec2d_type D,	X;

	matrix2_zero( &m );
	D.x = D.y = 0;



	VEC2D_LEFT( *v, u );


	*nD = 0;
	dMax = 0;
	for( i = i0 ; i < i1 ; i++ ){
		pt2d_type	*pt = &apt->p[i];

		vec2f_type	dp;
		dp.x = pt->p.x - p->x;
		dp.y = pt->p.y - p->y;

		d = VEC2D_INNER( u, dp );
		float t = VEC2D_INNER( *v, dp );

		float t2 = t*t;

		m.a00 += t2*t2;
		m.a01 += t2;
		D.x += d*t2;

		m.a10 += t2;
		m.a11 += 1;
		D.y += d;

		pt->r = d;

		if( ABS(d) > TD )	(*nD)++;

		if( ABS(d) > ABS(dMax) ){
			dMax = d;
		}
	}

	*a = 0;
	if( i1 - i0 > 6 ){
		matrix2_solve( &m, &D, &X );

		*a = X.y;
		float b = X.y;
	}


	//fprintf( stdout, " %.2f ", *a );


	return( dMax );
}