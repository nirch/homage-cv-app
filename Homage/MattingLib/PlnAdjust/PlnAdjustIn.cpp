
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif


#include	"Uigp/igp.h"
#include	"Ulog/Log.h"



#include "Uln/PlnType.h"
#include "Umath/Ellipse/EllipseType.h"





static void	pln_ellipse( pln_type *pl );

static int	plnA_is_in( plnA_type *apl, pln_type *pl );


static float	pln_is_in_apl( pln_type *bpl, plnA_type *apl, float T );



int
plnA_adjust_in( plnA_type *apl, plnA_type *bapl, plnA_type *eapl, float dT, int iFrame )
{
int	i,	j,	ret;

	if( apl->nA < 1 )
		return( 1 );
	
	ret = 0;

	PLNA_DUMP( apl, "pln", iFrame, "in" );

	for( i = 1 ; i < apl->nA ; i++ )
		pln_ellipse( apl->a[i] );

//	if( iFrame < 1 )	return( 0 );


	for( i = 1, j = 1 ; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];

		dPln_type	md;
		if( pln_distance_pln( pl, apl->a[0], &md ) < 0 )
			md.u = 0;


		if( md.u != 0 && ABS(md.u) < 5 ){
			pln_destroy( pl );
			continue;
		}


		if( bapl != NULL ){
			float pT = pln_is_in_apl( pl, bapl, 4.0 );
			if( pT > 0.6 ){
				apl->a[j++] = apl->a[i];
				continue;
			}
		}





		if( ABS(md.u) > dT || pln_inside( pl, apl->a[0] ) == 0 ){
			
			pln_destroy( pl );
			continue;
		}

		if( pl->e->ry *1.5 < pl->e->rx && ABS(pl->e->v.x) > 0.9 ){
			pln_destroy( pl );
			continue;
		}



		if( eapl != NULL ){
			float t = pln_is_in_apl( pl, eapl, 2.5 );

	
			if( t > 0.55 ){
				apl->a[j++] = apl->a[i];
				continue;
			}

			if( t < 0.1 ){
				pln_destroy( pl );
				continue;
			}
		}






		dPln_type	d;
		pln_distance( apl->a[0], &pl->e->p, &d );

		if( ABS(d.u) > 40 ){
			pln_destroy( pl );
			continue;
		}





		if( bapl != NULL ){
			if( plnA_is_in( bapl, pl ) < 0 ){
				pln_destroy( pl );
				continue;
			}
		}


		apl->a[j++] = apl->a[i];

	}

	apl->nA = j;

	return( ret );
}


static int
plnA_is_in( plnA_type *apl, pln_type *pl )
{
	int	i;

	for( i = 1 ; i < apl->nA ; i++ ){
		if(  ellipse_distance( apl->a[i]->e, pl->e ) < 32 )
			return( 1 );
	}

	return( -1 );
}



static void
pln_ellipse( pln_type *pl )
{
	
	pt2dA_type *apt = pt2dA_alloc( pl->len + 10 );

	pln_sampleN( pl, 1, 1, apt );

	pl->e = ellipse_alloc();
	pt2dA_ellipse( apt, 0, apt->nA, pl->e );

	pt2dA_destroy( apt );
}







static float
pln_is_in_apl( pln_type *bpl, plnA_type *apl, float T )
{
	int	n;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt,	s;


	n = 0;

	pln_type *spl;

	dt = 2.0;
	s = 0;
	for( gt = 0 ; gt < bpl->len ; gt += dt, n++ ){

		pln_gt2p( bpl, gt, &p );

		if( plnA_distance( apl, &p, 2*T, &spl, &d ) < 0 )
			continue;


		if( ABS(d.u) < T )
			s += dt;
	}


	return( s / bpl->len );

}


static float	pln_distance_line( pln_type *pl, vec2f_type *p0, vec2f_type *lv );


int
plnA_adjust_in_head( plnA_type *apl, vec2f_type *hp, float r, int iFrame )
{
	int	i,	j;

	if( apl->nA < 1 )
		return( 1 );


	vec2f_type	hv;
	hv.x = 1.0;
	hv.y = 0;

	for( i = 1, j = 1 ; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];


		float t = pln_distance_line( pl, hp, &hv );

		if( ABS(t) < r ){
			pln_destroy( pl );
			continue;
		}

		apl->a[j++] = apl->a[i];
	}

	apl->nA = j;

	return( 1 );

}


static float
pln_distance_line( pln_type *pl, vec2f_type *p0, vec2f_type *lv )
{
	float gt,	dt,	t,	tMax;
	vec2f_type p;
	dt = 2.0;
	vec2f_type	u;

	tMax  = 0;

	VEC2D_RIGHT( *lv, u );

	for( gt = 0 ; gt < pl->len ; gt += dt ){

		pln_gt2p( pl, gt, &p );

		p.x -= p0->x;
		p.y -= p0->y;

		t = VEC2D_INNER( p, u );
		if( gt == 0 || ABS(t) < ABS(tMax) )
			tMax = t;

	}

	return( tMax );
}
