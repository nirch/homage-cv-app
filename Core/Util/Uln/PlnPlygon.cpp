// WndInner.cpp : implementation file
//

#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Uigp/igp.h"



#include "PlnType.h"




vec2fA_type *
pln_to_polygon( pln_type *pl, float DT )
{
ln_type	*l;
vec2f_type ctr,	p;
int	n;
float	 dt,	t;



	for( l = pl->link, n = 0 ; l != NULL ; l = LN_NEXT(l) ){

		if( l->a != 0 )

			n += l->len / DT + 0.5;

		n += 1;
	}
			



	vec2fA_type *av = vec2fA_alloc( n+32 );



	for( l = pl->link, ctr = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){

		av->a[av->nA++] = ctr;


		if( l->a != 0 ){

			n = l->len / DT + 0.5;
			dt = l->len / n;

			for( t = dt ; t < l->len - 0.5*dt ; t += dt ){
				ln_t2xy( &ctr, l, t, &p );
				av->a[av->nA++] = p;
			}
		}

		ctr.x += l->v.x;
		ctr.y += l->v.y;

	}


	av->a[av->nA++] = ctr;

	return( av );
}








