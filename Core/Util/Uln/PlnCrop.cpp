// WndInner.cpp : implementation file
//

#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Uigp/igp.h"


#include "PlnType.h"


static int	pln_crop_box( pln_type *pl, box2f_type *b, plnA_type *apl );




plnA_type *
plnA_crop_box( plnA_type *apl, box2f_type *b, plnA_type *tapl )
{
int	i;


	tapl = plnA_realloc( tapl, 4*apl->nA );


	for( i = 0; i < apl->nA ; i++ ){
		//if( (pl = pln_crop_box( apl->a[i], b )) != NULL )
		//	tapl->a[tapl->nA++] = pl;

		pln_crop_box( apl->a[i], b, tapl );
	}

	
		

	return( tapl );
}



pln_type *
pln_crop_box( pln_type *pl, box2f_type *b )
{
ln_type *l,	*l0,	*l1;
vec2f_type ctr0,	v0,	v;

	l0 = l1 = NULL;

	if( BOX2D_IS_IN( *b, pl->ctr.x, pl->ctr.y) ){
		l0 = l1 = pl->link;
		ctr0 = pl->ctr;
	}

	for( l = pl->link, v = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){
		v0 = v;
		v.x = v0.x + l->v.x;
		v.y = v0.y + l->v.y;

		if( BOX2D_IS_IN( *b, v.x, v.y) == 0  )
			continue;

		if( l0 == NULL ){
			l0 = l;
			ctr0 = v0;
		}

		l1 = l;
	}

	if( l0 == NULL )
		return( 0 );

	l = LN_NEXT(l1);
	if( l != NULL )
		l1 = l;


	pln_type *tpl = pln_alloc( 0 );
	tpl->ctr = ctr0;
	tpl->link = lnL_copy( l0, LN_NEXT(l1) );

	pln_set_length( tpl );

	return( tpl );
}



static int
	pln_crop_box( pln_type *pl, box2f_type *b, plnA_type *apl )
{
	ln_type *l,	*l0,	*l1;
	vec2f_type ctr0,	v0,	v;
	int	n,	gap;
	

	l0 = l1 = NULL;

	n = gap = 0;
	if( BOX2D_IS_IN( *b, pl->ctr.x, pl->ctr.y) ){
		l0 = l1 = pl->link;
		ctr0 = pl->ctr;
	}

	for( l = pl->link, v = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){
		v0 = v;
		v.x = v0.x + l->v.x;
		v.y = v0.y + l->v.y;

		if( BOX2D_IS_IN( *b, v.x, v.y) == 0  ){
			gap++;
			continue;
		}


		if( l0 != NULL && gap > 2 ){
			pln_type *tpl = pln_alloc( 0 );
			tpl->ctr = ctr0;
			tpl->link = lnL_copy( l0, LN_NEXT(l1) );

			pln_set_length( tpl );
			plnA_add( apl, tpl );
			n++;;
			
			l0 = NULL;
		}

		if( l0 == NULL ){
			l0 = l1 = l;
			ctr0 = v0;
			gap = 0;
			continue;
		}

		l1 = l;
		gap = 0;
	}

	if( l0 != NULL ){
		pln_type *tpl = pln_alloc( 0 );
		tpl->ctr = ctr0;
		tpl->link = lnL_copy( l0, LN_NEXT(l1) );

		pln_set_length( tpl );
		plnA_add( apl, tpl);
		n++;
	}

	return( n );
}