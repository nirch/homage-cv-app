/**********************
 ***	VecTool.c   ***
 **********************/
#include	<math.h>

#include	"Uigp/igp.h"

#include	"Uvec/Vec2d.h"
#include	"Ucamera/Pt2dType.h"

#include "Uln/LnType.h"
#include "Uln/PlnType.h"

#include "Uvl/IntType.h"
#include "Uvl/floatType.h"




static int	pt2dA_bounding_box( pt2dA_type *apt, int a[], box2d	*b );

pln_type *	pt2dA_to_pl( pt2dA_type *apt, intA_type *a );

static int	pt2dA_force_convex1( pt2dA_type *apt, pt2d_type	*pt0, pt2d_type	*pt1, int *k );

static int	pt2dA_force_convex_to( pt2dA_type *apt, intA_type *a, int i1 );

static int	pt2dA_concave( pt2dA_type *apt, int i0, int i1, float dT, int *k );

static int	pt2dA_concave( pt2dA_type *apt, int i0, int i1, float dT, int *k );


pln_type *
pt2dA_bounding_contourNN( pt2dA_type *apt )
{
pln_type	*pl;
box2d	b;
int i;

	if( apt->nP == 1 ){
		pt2d_type	*pt = &apt->p[0];
		vec2f_type p;
		p.x = pt->p.y;
		p.y = pt->p.x;
		pl = pln_from_circle( &p, pt->r, 1.0 );
		return( pl );
	}


	intA_type *a = intA_alloc( 1000 );
	int	ab[4];
	pt2dA_bounding_box( apt, ab, &b );


	a->a[0] = ab[0];
	a->nA = 1;
	for( i = 1 ; i < 4 ; i++ ){
		if( a->a[a->nA-1] == ab[i] )	continue;

		pt2dA_force_convex_to( apt, a, ab[i] );
	}
	if( ab[0] != ab[3] )
		pt2dA_force_convex_to( apt, a, ab[0] );


	pl = pt2dA_to_pl( apt, a );
	
	intA_destroy( a );

	return( pl );
}



static int
pt2dA_force_convex_to( pt2dA_type *apt, intA_type *a, int i1 )
{
int	k;
	int i0 = a->a[a->nA-1];

	if( pt2dA_force_convex1( apt, &apt->p[i0], &apt->p[i1], &k ) < 0 ){
		int	c;
		if( pt2dA_concave( apt, i0, i1, 100, &c ) > 0 ){

			pt2dA_force_convex_to( apt, a, c );

			pt2dA_force_convex_to( apt, a, i1 );

			return( 1 );
		}

		a->a[a->nA++] = i1;
		return( 1 );
	}

	pt2dA_force_convex_to( apt, a, k );

	pt2dA_force_convex_to( apt, a, i1 );




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

	dp.x = p1.x - p0.x;
	dp.y = p1.y - p0.y;
	float len = hypot( dp.x, dp.y );


	iMin = -1;
	tMin = 100000000;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];
		if( pt == pt0 || pt == pt1 )
			continue;



		dp.x = pt->p.x - p0.x;
		dp.y = pt->p.y - p0.y;

		t = VEC2D_INNER( v, dp );
		if( t < 0 || t > len )	continue;

		t = dp.x * u.x + dp.y*u.y - pt->r;
		if( iMin < 0 || t < tMin ){
			iMin = i;
			tMin = t;
		}
	}

	*k = iMin;

	if( tMin < -0.001 )
		return( 1 );

	return( -1 );
}




static int
pt2dA_concave( pt2dA_type *apt, int i0, int i1, float dT, int *k )
{
	pt2d_type	*pt;
	vec2f_type	p0,	p1,	v,	dp,	bu;
	float	t;
	int	i;

	pt2d_type *pt0 = &apt->a[i0];
	pt2d_type *pt1 = &apt->a[i1];


	pt2d_tanget_line( pt0, pt1, &p0, &p1, &v );

	dp.x = p1.x - p0.x;
	dp.y = p1.y - p0.y;
	float len = hypot( dp.x, dp.y );

	if( len < dT )
		return( -1 );

	intA_type *ai = intA_alloc( len + 2 );
	floatA_type *af = floatA_alloc( len + 2 );
	ai->nA = ai->NA;
	for( i = 0 ; i < ai->nA ; i++ )
		ai->a[i] = -1;



	bu.x = -v.y;
	bu.y =  v.x;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];
		if( pt == pt0 || pt == pt1 )
			continue;


		dp.x = pt->p.x - p0.x;
		dp.y = pt->p.y - p0.y;

		t = VEC2D_INNER( v, dp );
		if( t < 0 || t > len )	continue;

		float u = dp.x * bu.x + dp.y*bu.y - pt->r;

		int	k = t/8;
		if( ai->a[k] < 0 || af->a[k] > u ){
			af->a[k] = u;
			ai->a[k] = i;
		}

	}

	int	iMax = 0;
	float	tMax = 0;

	int I = len / 8;
	for( i = 20 ; i < ai->nA-20 ; i++ ){
		if( ai->a[i] < 0 )	continue;
		if(iMax <0 ||  af->a[i] > tMax ){
			iMax = ai->a[i];
			tMax = af->a[i];
		}
	}

	intA_destroy( ai );
	floatA_destroy( af );

	*k = iMax;

	if( iMax < 0 || tMax < 100 )
		return( -1 );

	return( 1 );
}



static void	ln_smooth_a( ln_type *link, ln_type *pl, ln_type *nl );


pln_type *
pt2dA_to_pl( pt2dA_type *apt, intA_type *a )
{
int	i,	n;
vec2d	*p,	v,	p0,	p1;
ln_type	*l,	*pl,	*nl;

pln_type	*pln;


	p = (  vec2f_type *)malloc( 2*a->nA * sizeof(vec2f_type) );

	for( i = 0, n = 0 ; i < a->nA-1 ; i++ ){
		pt2d_tanget_line( &apt->p[a->a[i]], &apt->p[a->a[i+1]], &p0, &p1, &v );

		p[n].x = p0.y;
		p[n].y = p0.x;

		p[n+1].x = p1.y;
		p[n+1].y = p1.x;

		n += 2;
	}


	pln = pln_from_point( p, n, 1 );

	vec2f_type	ctr = pln->ctr;
	for( pl = pln->link, l = LN_NEXT(pl) ;  l != NULL ; pl = nl, l = LN_NEXT(pl) ){
		if( (nl = LN_NEXT(l)) == NULL ){
			ln_smooth_a(  l, pl, pln->link );
			break;
		}

		ctr.x += pl->v.x + l->v.x;
		ctr.y += pl->v.y + l->v.y;

		ln_smooth_a(  l, pl, nl );

	}

	free( p );

	pln_set_length( pln );

	return( pln );
}



static void
ln_smooth_a( ln_type *link, ln_type *pl, ln_type *nl )
{
vec2d	u;
float	a,	maxa;

	a = VEC2D_INNER( pl->u, nl->v );

	if( a < 0 ){
		vec2f_type u,	p;
		u.x = -nl->u.y;
		u.y =  nl->u.x;
		p.x = pl->v.x + link->v.x;
		p.y = pl->v.y + link->v.y;
		float	t = - VEC2D_INNER( p, pl->u ) / VEC2D_INNER( u, pl->u );

		vec2f_type	p0,	p1;
		p1.x = p.x + nl->v.x;
		p1.y = p.y + nl->v.y;

		p0.x = p.x + 2*t*-nl->u.y;
		p0.y = p.y + 2*t*nl->u.x;

		pl->v.x -= 2*t * -pl->u.y;
		pl->v.y -= 2*t *  pl->u.x;
		pl->len -= 2*t;


		link->v.x = p0.x - pl->v.x;
		link->v.y = p0.y - pl->v.y;
		ln_set_aux( link );

		nl->v.x = p1.x - p0.x;
		nl->v.y = p1.y - p0.y;
		ln_set_aux( nl );

		return;
	}



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






	

