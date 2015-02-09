
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


int	pln_adjust_intersect0( pln_type *pl );


static int	pln_tttt( pln_type *pl, float gt, float *d );

ln_type *	plnC_crop( pln_type *pl, float gt0, float gt1 );


int
plnA_adjust_intersect( plnA_type *apl )
{
int	i;

	for( i = 0 ; i < apl->nA ; i++ )
		pln_adjust_intersect0( apl->a[i] );


	return( 1 );

}


int
pln_adjust_intersect( pln_type *pl )
{

	pln_adjust_intersect0( pl );

	return( 1 );

}

int
pln_adjust_intersect0( pln_type *pl )
{
	ln_type *l,	*l1;

	float gt;

	float T = cos( ANGLE_D2R(12.5 ) );

	vec2f_type	ctr = pl->ctr;
	for( l = pl->link , gt = 0 ; l != NULL ; l = LN_NEXT(l) ){
		gt += l->len;
		ctr.x += l->v.x;
		ctr.y += l->v.y;
		l1 = LN_NEXT(l);
		if( l1 == NULL )
			break;

		vec2f_type v,	v1,	u;
		ln_tanget_end( l, &v );
		ln_tanget_beg( l1, &v1 );
		VEC2D_RIGHT( v, u );

		float t = VEC2D_INNER( v, v1 );
		//float u0 = VEC2D_INNER( u, l1->v );
		float u0 = VEC2D_INNER( u, v1 );

		if(   t > -T  )
			continue;

		vec2f_type p;
		pln_gt2p( pl, gt, &p );


		float dt;
		if( pln_tttt( pl, gt, &dt ) < 0 )
			continue;


		l = plnC_crop( pl, gt - dt, gt + dt );
		gt = lnL_lengthE( pl->link, LN_NEXT(l) );
	}

	return( 1 );
}



ln_type *
plnC_crop( pln_type *pl, float gt0, float gt1 )
{
	ln_type *l;

	if( gt0 < 0 ){
		pln_trim( pl, F_END, ABS(gt0) );
		pln_trim( pl, F_BEGIN, gt1 );
		pln_close( pl, 0.25 );

		l = LN_NEXT( pl->link );

	}
	else {
		pln_type *pl1 =	pln_split( pl, gt1, 0.25 );
		pln_trim( pl, F_END, pl->len - gt0 );

		l = pl1->link;

		if( pl->link == NULL ){
			free( pl );
			*pl = *pl1;

			pl1->link = NULL;
			pln_destroy( pl1 );

			pln_close( pl, 0.25 );
			l = LN_NEXT( pl->link );
		}
		else	pln_append( pl, pl1 );

		pl->state = PLN_CLOSE;
	}


	vec2f_type v,	v1;


	ln_tanget_beg( l, &v1 );
	

	l = LN_PREV( l );
	ln_tanget_end( l, &v );
	float t0 = VEC2D_INNER( v, v1 );

	l->a = 0.25*l->len;
	ln_set_aux( l );

	ln_tanget_end( l, &v );
	float t1 = VEC2D_INNER( v, v1 );
	if( t0 > t1 ){
		l->a = -0.25*l->len;
		ln_set_aux( l );
	}


	return( l );
}


static int
pln_tttt( pln_type *pl, float gt, float *d )
{
	vec2f_type	v,	v1,	u;

	float	dt;

	float T = cos( ANGLE_D2R(10 ) );

	for( dt = 1.0 ; dt < 10 ; dt++ ){
		pln_tanget( pl, gt - dt, &v );
		pln_tanget( pl, gt+dt, &v1 );

		VEC2D_RIGHT( v, u );

		float t = VEC2D_INNER( v, v1 );
		float u0 = VEC2D_INNER( u, v1 );

		vec2f_type p0,	p1;
		pln_gt2p( pl, gt-dt, &p0 );
		pln_gt2p( pl, gt+dt, &p1 );

		vec2f_type	dp;
		dp.x = p1.x - p0.x;
		dp.y = p1.y - p0.y;
		float s = hypot( dp.x, dp.y );

		if( t > -T && s > 0.75 ){
			*d = dt;
			//pln_type *pl1 =	pln_split( pl, gt + dt, 0.25 );
			//pln_trim( pl, F_END, pl->len - ( gt - dt) );

			//pln_append( pl, pl1 );
			//pl->state = PLN_CLOSE;
			return( 1 );
		}

	}

	return( -1 );
}











