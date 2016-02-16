/**********************
 ***	VecTool.c   ***
 **********************/
#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"

#include	"Uvec/Vec2d.h"
#include	"Ucamera/Pt2dType.h"
#include "Uvl/IntType.h"

#include "Uln/LnType.h"
#include "Uln/PlnType.h"




static int	pt2dA_bounding_next( pt2dA_type *apt, int i0, float r, vec2f_type *v );

static int	pt2dA_bounding_minx( pt2dA_type *apt );


pln_type *	pt2dA_bounding_pl( pt2dA_type  *apt, intA_type *a, float r );


pln_type *
pt2dA_bounding_contourN( pt2dA_type *apt, int r0, float r1 )
{
	pln_type	*pl;


	int i;

	if( apt->nP == 1 ){
		pt2d_type	*pt = &apt->p[0];
		vec2f_type p;
		p.x = pt->p.y;
		p.y = pt->p.x;
		pl = pln_from_circle( &p, pt->r, 1.0 );
		return( pl );
	}

	int i0 = pt2dA_bounding_minx( apt );

	intA_type *a = intA_alloc( 2*apt->nA );

	a->a[a->nA++] = i0;

	apt->a[i0].n.x = 0;
	apt->a[i0].n.y = 1;

	i = i0;
	vec2f_type	v;
	while( ( i = pt2dA_bounding_next( apt, i, r1, &v) ) != i0 ){
			apt->a[i].n = v;

			if( a->nA >= a->NA ){
				intA_destroy( a );
				return( NULL );
			}

			a->a[a->nA++] = i;
//			fprintf( stdout, "%d  ", i );
	}


//	fprintf( stdout, "%d %d\n", a->NA, a->nA );



	pl = pt2dA_bounding_pl( apt, a, r0 );


	intA_destroy( a );


	return( pl );
}


static int
	pt2dA_bounding_minx( pt2dA_type *apt )
{
	pt2d_type	*pt;
	int	i;


	int i0 = 0;

	for( i = 1 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];
		if( pt->p.x < apt->a[i0].p.x )
			i0 = i;
	}

	return( i0 );
}



static int
	pt2dA_bounding_next( pt2dA_type *apt, int i0, float r, vec2f_type *v )
{
	pt2d_type	*pt;
	int	i;


	float r2 = r*r;



	int i1 = -1;
	vec2f_type	bu;

	float	t0 = -1;



	pt2d_type	*bpt = &apt->a[i0];

	for( i = 0 ; i < apt->nP ; i++ ){
	
		pt = &apt->p[i];
		if( pt == bpt )	continue;


		vec2f_type	dp;
		dp.x = pt->p.x - bpt->p.x;
		dp.y = pt->p.y - bpt->p.y;


		if( dp.x*dp.x + dp.y*dp.y > r2 )	continue;
		float t = VEC2D_INNER( bpt->n, dp );

		if( i1 < 0 ){
			i1 = i;
			t0 = t;
			VEC2D_RIGHT(dp, bu );
			*v = dp;
			continue;
		}


		float u = VEC2D_INNER( dp, bu );
		if( t > 0 && t0 < 0 || u < 0 && ( t > 0 || t0 < 0 ) ){
			i1 = i;
			t0 = t;
			VEC2D_RIGHT( dp, bu );
			*v = dp;
		}
	}

	return( i1 );
}


pln_type *	pln_append_cycle( pln_type *pl, vec2f_type *p0, vec2f_type *p, float r );

pln_type *	pln_append_line( pln_type *pl, vec2f_type *p );


pln_type *
pt2dA_bounding_pl( pt2dA_type  *apt, intA_type *a, float r )
{
	int	i;
	pln_type *pl;
	vec2f_type p;

	pl = NULL;
	for( i = 0 ; i < a->nA ; i++ ){
		if( pl != NULL && pl->link != NULL )
			PLN_DUMP( pl, "PL", i , NULL );

		int	i0 = a->a[i];
		int	i1 = (i < a->nA-1)? a->a[i+1] : a->a[0];

		vec2f_type *p0 = &apt->a[i0].p;
		vec2f_type *p1 = &apt->a[i1].p;

		vec2f_type	v,	bu;
		v.x = p1->x - p0->x;
		v.y = p1->y - p0->y;

		float t = hypot( v.x, v.y );
		v.x /= t;
		v.y /= t;
		
		VEC2D_LEFT(v, bu );

		if( t < r ){
			float h = sqrt( r*r - 0.25*t*t );
			p.y = p0->x + 0.5*t*v.x + h *bu.x;
			p.x = p0->y + 0.5*t*v.y + h *bu.y;

			pl = pln_append_cycle( pl, p0, &p, r );
			continue;
		}


		//p.y = p0->x + 0.5*t*v.x + r *bu.x;
		//p.x = p0->y + 0.5*t*v.y + r *bu.y;
		p.y = p0->x + r *bu.x;
		p.x = p0->y + r *bu.y;

		pl = pln_append_cycle( pl, p0, &p, r );

		//if( pl != NULL && pl->link != NULL )
		//	PLN_DUMP( pl, "PL", i , "a" );

		p.y = p1->x + r *bu.x;
		p.x = p1->y + r *bu.y;
		pl = pln_append_line( pl, &p );

	}


	int	i0 = a->a[0];
	vec2f_type *p0 = &apt->a[i0].p;	
	pl = pln_append_cycle( pl, p0, &pl->ctr, r );

	pln_close( pl, 0.1 );

	PLN_DUMP( pl, "PL", i , NULL );

	return( pl );
}


void	pln_appendA( pln_type *pl, ln_type *l );

pln_type *
pln_append_cycle( pln_type *pl, vec2f_type *p0, vec2f_type *p, float r )
{
	if( pl == NULL ){
		pl = pln_alloc( 0 );
		pl->ctr.x = p->x;
		pl->ctr.y = p->y;
		return( pl );
	}

	vec2f_type	p1;
	pln_end_point( pl, &p1 );

	ln_type *l = ln_alloc();
	l->v.x = p->x - p1.x;
	l->v.y = p->y - p1.y;
	l->a = 0; 
	ln_set_aux( l );

	ln_type *l0 = lnL_last( pl->link );
	if( l0 != NULL  ){
		float t = -l0->u.y * l->v.x + l0->u.x *l->v.y;//VEC2D_INNER( l0->v, l->v );
		if( t < 0 && -t < l0->len ){
			l0->v.x += -t* l0->u.y;
			l0->v.y += t* l0->u.x;
			ln_set_aux( l0 );
			pln_set_length( pl );

			ln_destroy( l );
			return( pl );
		}
	}

	if( l->len > r ){
		l->a = 0.125*l->len;
		ln_set_aux( l );
	}


	pln_appendA( pl, l );

	return( pl );
}




pln_type *
	pln_append_line( pln_type *pl, vec2f_type *p )
{
	if( pl == NULL ){
		pl = pln_alloc( 0 );
		pl->ctr.x = p->y;
		pl->ctr.y = p->x;
		return( pl );
	}

	vec2f_type	p1;
	pln_end_point( pl, &p1 );

	ln_type *l = ln_alloc();
	l->v.x = p->x - p1.x;
	l->v.y = p->y - p1.y;
	l->a = 0;
	ln_set_aux( l );


	pln_appendA( pl, l );

	return( pl );
}



void
pln_appendA( pln_type *pl, ln_type *l )
{
	if( pl->link == NULL ){
		pl->link = l;
		pln_set_length( pl );
		return;
	}

	ln_type *l0 = lnL_last( pl->link );

	l0->p[1] = l;
	l->p[0] = l0;
	pln_set_length( pl );
}

