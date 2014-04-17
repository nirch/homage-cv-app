/*********************
 ***   LkSplit.h   ***
 ********************/

#include	<stdio.h>   
#include	<math.h>
#include	<stdlib.h>


#include	"Uigp/igp.h"

#include	"LnType.h"



ln_type *
ln_split( ln_type *link, float t, float dt )
{
	ln_type	*l;


	if( t > link->len - dt )
		return( LN_NEXT(link) );


	if( t < dt ){

		return( link );
	}


	l = ln_split_t( link, t );
	return( l );
}



ln_type *
ln_split_t( ln_type *link, float t0 )
{
	ln_type	*l,*l1;
	vec2d	dv,	pv,	pv1;
	vec2d	v,	u;
	float	t, a;
	float	h;



	l = LN_ALLOC();




	h = link->c_prb * t0 * (t0 - link->len);

	dv.x = t0 * -link->u.y  +  h * link->u.x;
	dv.y = t0 *  link->u.x  +  h * link->u.y;

	*l = *link;


	/* profile */
	t = t0 / link->len;




	l->v.x = link->v.x - dv.x;
	l->v.y = link->v.y - dv.y;


	link->v.x = dv.x;
	link->v.y = dv.y;



	/* link->a */
	if( link->c_prb != 0 ){

		a = link->a;

		pv.x = -link->u.y * dv.x + link->u.x*dv.y;
		pv.y =  link->u.x * dv.x + link->u.y*dv.y;


		t = hypot( pv.x , pv.y );
		u.x = -pv.y/t;
		u.y = pv.x/t;
		t = (-u.x  + u.y* link->c_prb* link->len )/
			( 2*link->c_prb*u.y );
		v.x = t;
		v.y = link->c_prb*t*( t - link->len );
		link->a = v.x *u.x + v.y * u.y;

		if( ABS(link->a) > ABS(a) )	link->a = 0;



		pv1.x = link->len - pv.x;
		pv1.y = 0  - pv.y;
		t = hypot( pv1.x , pv1.y );
		u.x = -pv1.y/t;
		u.y = pv1.x/t;
		t = (-u.x  + u.y* link->c_prb* link->len )/
			( 2*link->c_prb*u.y );
		v.x = t - pv.x;
		v.y = link->c_prb*t*( t - link->len ) - pv.y;
		l->a = v.x *u.x + v.y * u.y;

		if( ABS(l->a) > ABS(a) )	l->a = 0;
	}


	ln_set_aux( link );
	ln_set_aux( l );



	/* POINTER */

	link->p[1] = l;
	l->p[0] = link;
	l1 = LN_NEXT(l);
	if ( l1 != NULL )
		l1->p[0] = l;


	return( l );
}

