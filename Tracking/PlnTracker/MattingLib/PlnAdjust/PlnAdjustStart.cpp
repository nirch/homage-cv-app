
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



static void	pln_adjust_start( pln_type *pl, float height );


int	pln_rangeH( pln_type *pl, float h, float ag[] );


int
plnA_adjust_start( plnA_type *apl, float height )
{
	int	i;

	if( apl->nA == 0 )
		return( -1 );

	for( i = 0 ; i < apl->nA ; i++ )
		pln_adjust_start( apl->a[i], height );


	pln_type	*pl = apl->a[0];

	float ag[100];
	int n =	pln_rangeH(  apl->a[0], height, ag );

	if( n > 2 && ag[n-1] - ag[n-2] < 10 )
		n -= 2;

	ln_type *l0;
	vec2f_type p0;
	float	t;
	pln_gt2lt( apl->a[0], ag[n-1], &p0, &l0, &t );

	if( t+0.1 >= l0->len && LN_NEXT(l0) == NULL )
		return( 1 );


	if( l0 == pl->link )	return( 1 );

	ln_type *el = lnL_last( pl->link );


	el->p[1] = pl->link;
	pl->link->p[0] = el;

	pl->ctr = p0;
	pl->link = l0;

	ln_type *l = LN_PREV( l0 );
	l->p[1] = NULL;
	l0->p[0] = NULL;


	PLN_DUMP( pl, "BB", 1, NULL );

	return( 1 );
}

static void
pln_adjust_start( pln_type *pl, float height )
{

	vec2f_type ctr;
	ln_type *l,	*el;

	vec2f_type p0 = pl->ctr;
	ln_type *l0 = pl->link;

	for( l = pl->link, ctr = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){
		if( ctr.x > p0.x && p0.x < height -1  || ( ctr.x == p0.x || ctr.x == height ) && ctr.y < p0.y ){
			p0 = ctr;
			l0 = l;
		}

		ctr.x += l->v.x;
		ctr.y += l->v.y;

		el = l;
	}


	if( l0 == pl->link )	return;

	el->p[1] = pl->link;
	pl->link->p[0] = el;

	pl->ctr = p0;
	pl->link = l0;

	l = LN_PREV( l0 );
	l->p[1] = NULL;
	l0->p[0] = NULL;
}




static int
pln_range( pln_type *pl, float h, float *gt0, float *gt1 )
{

	vec2f_type v;
	ln_type *l;

	*gt0 = 0;
	*gt1 = 0;

	for( l = pl->link, v = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){

		*gt1 += l->len;
		v.x += l->v.x;
		v.y += l->v.y;

		if( v.x > h - 0.1 && *gt1 > 10.0 )
			break;
	}


	return( 1 );
}



int
pln_rangeH( pln_type *pl, float h, float ag[] )
{
float	gt0,	gt1,	gt;
int	n;

	vec2f_type v;
	ln_type *l;

	gt0 = gt1 = -1;

	

	for( l = pl->link, v = pl->ctr, gt = 0, n = 0 ; l != NULL ; l = LN_NEXT(l) ){

		if( v.x > h - 0.1 ){
			if( gt0 < 0 )
				gt0 = gt;

			gt1 = gt;
		}
		else {
			if( gt0 > 0 ){
				ag[n++] = gt0;
				ag[n++] = gt1;
			}

			gt0 = gt1 = -1;
		}



		gt += l->len;
		v.x += l->v.x;
		v.y += l->v.y;
	}

	if( v.x > h - 0.1 ){
		if( gt0 < 0 )
			gt0 = gt;

		gt1 = gt;
	}

	if( gt0 > 0 ){
		ag[n++] = gt0;
		ag[n++] = gt1;
		}

	return( n );
}


