
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



static void	pln_adjust_start( pln_type *pl );

int
plnA_adjust_start( plnA_type *apl )
{
	int	i;

	for( i = 0 ; i < apl->nA ; i++ )
		pln_adjust_start( apl->a[i] );

	return( 1 );
}

static void
pln_adjust_start( pln_type *pl )
{

	vec2f_type ctr;
	ln_type *l,	*el;

	vec2f_type p0 = pl->ctr;
	ln_type *l0 = pl->link;

	for( l = pl->link, ctr = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){
		if( ctr.x > p0.x || ctr.x == p0.x && ctr.y < p0.y ){
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