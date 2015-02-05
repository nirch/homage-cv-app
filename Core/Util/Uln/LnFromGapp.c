/**************************
 ***	LnFromGapp.c	***
 **************************/

#include <math.h>
#include	"UGeoApp/GeoApp.h"



#include "LnType.h"

#include "PlnType.h"



pln_type *
pln_from_gapp( gapp_type *gapp )
{
pln_type *pl;

	pl = pln_alloc( 0 );

	lnL_from_gapp( gapp, &pl->ctr, &pl->link );

	pln_set_length( pl );

	return( pl );
}


int 
lnL_from_gapp( gapp_type *gapp, vec2f_type *ctr, ln_type **lnL )
{
ln_type	*l;
	
vec2d	p,	v;
int	i,	iseg;
  
	*ctr = gapp->v[0];


	*lnL = lnL_alloc( gapp->ino );


	p = gapp->v[0];
	for(i = 0, l = *lnL, iseg = 0; i < gapp->ino; i++, l = LN_NEXT(l) ){

		iseg = gapp_next( gapp, iseg, GEOM_DIVISION);

		v = gapp->v[iseg];


		l->v.x = v.x - p.x;
		l->v.y = v.y - p.y;

		p.x += l->v.x;
		p.y += l->v.y;



		/* set AUX */
		if( l->v.x == 0.0 && l->v.y == 0 ){
			l->len = l->u.x = l->u.y = l->c_prb = l->a = 0.0;
			continue;
		}


		l->len = hypot( l->v.x, l->v.y );
 
		l->u.x = l->v.y/l->len;
		l->u.y = -l->v.x/l->len;

		l->c_prb = gapp->a[iseg];
		l->a = - l->c_prb * l->len * l->len*0.25 ;	
	}


	return( gapp->ino );
}



void
pln_to_gapp( pln_type *pl, float dt, gapp_type *gapp )
{
ln_type	*l;
vec2d	v;
float	t,	t0;//,	len;//,	a;
int	no;

	no = pl->len + 10;

	gapp_realloc( gapp, no );


	gapp->no = 0;
	t0 = 0;

	for( l = pl->link, v = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){

		for( t = t0 ; t < l->len ; t += dt ){
			ln_t2xy( &v, l, t, &gapp->v[gapp->no] );
			ln_tanget( l, t, &gapp->tang[gapp->no] );

			gapp->flag[gapp->no] = 0;

			gapp->no++;
		}

		t0 = t - l->len;

		v.x += l->v.x;
		v.y += l->v.y;
	}
}