/**********************
 ***	VecTool.c   ***
 **********************/
#include	<math.h>

#include	"Uigp/igp.h"

#include	"Uvec/Vec2d.h"
#include	"Ucamera/Pt2dType.h"

#include "Uln/LnType.h"
#include "Uln/PlnType.h"









//static pln_type *	pt2dA_bounding_contour( pt2dA_type *apt );


static int	pt2dA_bounding_box( pt2dA_type *apt, int a[], box2d	*b );

pln_type *	pt2dA_to_pl( pt2dA_type *apt, int a[], int nA );


static int	pt2dA_force_convex( pt2dA_type *apt, int a[], int *nA );

static int	pt2dA_force_convex1( pt2dA_type *apt, pt2d_type	*pt0, pt2d_type	*pt1, int *k );


pln_type *pt2dA_to_plN( pt2dA_type *apt, int a[], int nA );



plnA_type *
pt2dA_componnet( pt2dA_type *apt, float dT, int nT )
{
pt2dA_type *aptc;
plnA_type *apl;
pln_type	*pl;
int	i;
pt2dGroupA_type *ag;


	pt2dA_group_radius( apt, dT );

	ag = pt2dGroupA_set( NULL, apt );


	apl = plnA_alloc( ag->nG );

	apl->nA = 0;
	for( i = 0 ; i < ag->nG ; i++ ){
		if( ag->g[i].no < nT )
			continue;

		aptc = pt2dA_copy_group( apt, ag->g[i].group, ag->g[i].no );

		pl = pt2dA_bounding_contour( aptc );

		apl->a[apl->nA++] = pl;

		pt2dA_destroy( aptc );
	}


	pt2dGroupA_destroy( ag );


	return( apl );
}






pln_type *
pt2dA_bounding_contour( pt2dA_type *apt )
{
pln_type	*pl;
box2d	b;
int a[1000],	nA;
int i,	j;

	if( apt->nP == 1 ){
		pt2d_type	*pt = &apt->p[0];
		vec2f_type p;
		p.x = pt->p.y;
		p.y = pt->p.x;
		pl = pln_from_circle( &p, pt->r, 1.0 );
		return( pl );
	}

	pt2dA_bounding_box( apt, a, &b );



	for( i = 1, j = 0 ; i < 4 ; i++ ){
		if( a[i] == a[j] )	continue;
		j++;
		a[j] = a[i];
	}

	if( a[0] == a[j] )	j--;

	nA = j+1;


	pt2dA_force_convex( apt, a, &nA );

	pl = pt2dA_to_pl( apt, a, nA );
	

	return( pl );
}


static int
pt2dA_force_convex( pt2dA_type *apt, int a[], int *nA )
{
int	i,	j,	k;

	a[*nA] = a[0];

	for( i = 0 ; i < *nA ; ){
		if( pt2dA_force_convex1( apt, &apt->p[a[i]], &apt->p[a[i+1]], &k ) < 0 ){
			i++;
			continue;
		}

		*nA += 1;
		for( j = *nA ; j > i+1 ; j-- ){
			a[j] = a[j-1];
		}
		a[i+1] = k;
		
	}

	return( 1 );
}

static int
pt2dA_force_convex1( pt2dA_type *apt, pt2d_type	*pt0, pt2d_type	*pt1, int *k )
{
pt2d_type	*pt;
vec2f_type	p0,	p1,	v,	dp,	u;
float	tMin,	t;
int	iMin,	i;

	pt2d_tanget_line( pt0, pt1, &p0, &p1, &v );

	u.x = -v.y;
	u.y =  v.x;


	iMin = -1;
	tMin = 100000000;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];
		if( pt == pt0 || pt == pt1 )
			continue;


		dp.x = pt->p.x - p0.x;
		dp.y = pt->p.y - p0.y;

		t = dp.x * u.x + dp.y*u.y - pt->r;
		if( t < tMin ){
			iMin = i;
			tMin = t;
		}
	}

	*k = iMin;

	if( tMin < -0.001 )
		return( 1 );

	return( -1 );
}

#ifdef _AA_
pln_type *
pt2dA_to_plN( pt2dA_type *apt, int a[], int nA )
{
int	i,	n;
vec2d	v,	p0,	p1,	p00,	ctr;
ln_type	*l;
pt2d_type *pt0,	*pt1;

	pln_type	*pl;

	pl = pln_alloc( 0 );

	pt0 = &apt->p[a[0]];
	for( i = 0, n = 0 ; i < nA-1 ; i++ ){
		pt1 = &apt->p[a[i+1]];

		pt2d_tanget_line( &apt->p[a[i]], &apt->p[a[i+1]], &p0, &p1, &v );

		if( i > 0 ){
			lnL_from_sub_circle( &pt0->p, pt0->r, &p00, &p0, 1, &ctr, &l );
			pln_appendL( pl, &ctr, l );
		}

		l = LN_ALLOC();
		l->v.x = p1.x - p0.x;
		l->v.y = p1.y - p0.y;
		l->a = 0;
		ln_set_aux( l );
		l->p[0] = l->p[1] = NULL;

		pln_appendL( pl, &p0, l );


		p00 = p1;
		pt0 = pt1;

	}

	lnL_from_sub_circle( &pt0->p, pt0->r, &p00, &pl->ctr, 1, &ctr, &l );
	pln_appendL( pl, &ctr, l );

	pl->state = PLN_CLOSE;

	return( pl );
}
#endif

static void	ln_smooth_a( ln_type *link, ln_type *pl, ln_type *nl );


pln_type *
pt2dA_to_pl( pt2dA_type *apt, int a[], int nA )
{
int	i,	n;
vec2d	*p,	v,	p0,	p1;
ln_type	*l,	*pl,	*nl;

pln_type	*pln;


	p = (  vec2f_type *)malloc( 2*nA * sizeof(vec2f_type) );

	for( i = 0, n = 0 ; i < nA-1 ; i++ ){
		pt2d_tanget_line( &apt->p[a[i]], &apt->p[a[i+1]], &p0, &p1, &v );

		p[n].x = p0.y;
		p[n].y = p0.x;

		p[n+1].x = p1.y;
		p[n+1].y = p1.x;

		n += 2;
	}

	pt2d_tanget_line( &apt->p[a[nA-1]], &apt->p[a[0]], &p0, &p1, &v );


	p[n].x = p0.y;
	p[n].y = p0.x;

	p[n+1].x = p1.y;
	p[n+1].y = p1.x;
	n += 2;

	pln = pln_from_point( p, n, 1 );

	for( pl = pln->link, l = LN_NEXT(pl) ;  l != NULL ; pl = nl, l = LN_NEXT(pl) ){
		if( (nl = LN_NEXT(l)) == NULL ){
			ln_smooth_a(  l, pl, pln->link );
			break;
		}

		ln_smooth_a(  l, pl, nl );
	}

	free( p );

	return( pln );
}



static void
ln_smooth_a( ln_type *link, ln_type *pl, ln_type *nl )
{
vec2d	u;
float	a,	maxa;

	u.x = pl->u.x * -link->u.y + pl->u.y *link->u.x;
	u.y = pl->u.x * link->u.x + pl->u.y *link->u.y;

	a = -u.x *link->len / ( 4* u.y);

	maxa = 0.25*link->len;
	a = PUSH_TO_RANGE( a, -maxa, maxa );

	if( a > 0 )	a = -a;

	link->a = a;
	ln_set_aux( link );
}


static int
pt2dA_bounding_box( pt2dA_type *apt, int a[], box2d	*b )
{
pt2d_type	*pt,	*cpt;
int	i;

	a[0] = a[1] = a[2] = a[3] = 0;

	for( i = 1 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		cpt = &apt->p[a[0]];
		if( pt->p.x - pt->r <  cpt->p.x - cpt->r)
			a[0] = i;

		cpt = &apt->p[a[1]];
		if( pt->p.y - pt->r <  cpt->p.y - cpt->r)
			a[1] = i;


		cpt = &apt->p[a[2]];
		if( pt->p.x + pt->r >  cpt->p.x + cpt->r)
			a[2] = i;

		cpt = &apt->p[a[3]];
		if( pt->p.y + pt->r >  cpt->p.y + cpt->r)
			a[3] = i;
	}

	cpt = &apt->p[a[0]];
	b->y0 = cpt->p.x - cpt->r;


	cpt = &apt->p[a[1]];
	b->x0 = cpt->p.y - cpt->r;
	


	cpt = &apt->p[a[2]];
	b->y1 = cpt->p.x + cpt->r;
	

	cpt = &apt->p[a[3]];
	b->x1 = cpt->p.y + cpt->r;

	return( 1 );
}






	

