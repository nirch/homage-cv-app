/**************************
 ***	LnFromGapp.c	***
 **************************/

#include <math.h>
#include	"UGeoApp/GeoApp.h"



#include "LnType.h"

#include "PlnType.h"

#define IS_NAN( f )	 (!(f <= 1.0 ) && !(f>1.0))


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

		//if( IS_NAN( l->c_prb) )
		//	fprintf( stdout, "PaP" );

		l->a = - l->c_prb * l->len * l->len*0.25 ;	

		//if( IS_NAN( l->a) )
		//	fprintf( stdout, "NaN" );

	}


	return( gapp->ino );
}



pln_type *
pln_from_gappA( gapp_type *gapp, int i0, int i1 )
{
	ln_type	*l,	*l0;

	vec2d	p,	v;
	int	i,	iseg;


	pln_type *pl = pln_alloc( 0 );
	pl->ctr = gapp->v[i0];




	p = gapp->v[i0];
	for(i = i0, l0 = NULL ; i < i1; i = iseg, l0 = l ){

		iseg = gapp_next( gapp, i, GEOM_DIVISION);

		v = gapp->v[iseg];

		l = LN_ALLOC();

		l->p[1] = NULL;

		if( l0 != NULL ){
			l0->p[1] = l;
			l->p[0] = l0;
		}
		else
			pl->link = l;

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

	pln_set_length( pl );

	return( pl );
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