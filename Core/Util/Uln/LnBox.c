/***************************
 ***   AgpCopyBranch.c   ***
 ***************************/

#include	<stdio.h>
#include	<math.h>

#include	"Uigp/igp.h"
#include "Ubox/Box2d.h"



#include	"LnType.h"



static void	ln_update_box( vec2d *ctr, ln_type *link, box2d *box );


void
lnL_box( vec2d *ctr, ln_type *link, ln_type *last, box2f_type *box )
{
ln_type	*l;
vec2d	v;

	BOX2D_INIT( *box, ctr->x, ctr->y );

	for( l = link, v = *ctr ; l != last ; l = LN_NEXT(l) ){
		ln_update_box( &v, l, box );

		v.x += l->v.x;
		v.y += l->v.y;
	}
}


void
ln_box( vec2d *ctr, ln_type *link, box2f_type *box )
{
	BOX2D_INIT( *box, ctr->x, ctr->y );

	ln_update_box( ctr, link, box );
}


static void
ln_update_box( vec2d *ctr, ln_type *link, box2d *box )
{
vec2d	v,	p;
float	t;



	v.x = ctr->x + link->v.x;
	v.y = ctr->y + link->v.y;
	BOX2D_UPDATE( *box, v.x, v.y );



	if( ln_tanget_point_10( link, ctr, &t, &p ) )
		BOX2D_UPDATE( *box, p.x, p.y );



	if( ln_tanget_point_01( link, ctr, &t, &p ) )
		BOX2D_UPDATE( *box, p.x, p.y );

}



#ifdef _AA_
void
ln_box( ln_type *l, box2d *b )
{
	float	tmp,	t,	h;
	vec2d	p;

	if( l->v.x < 0 ){
		b->x0 = l->v.x;
		b->x1 = 0;
	}
	else    {
		b->x0 = 0;
		b->x1 = l->v.x;
	}

	if( l->v.y < 0 ){
		b->y0 = l->v.y;
		b->y1 = 0;
	}
	else    {
		b->y0 = 0;
		b->y1 = l->v.y;
	}

	if( l->c_prb == 0.0 )   return;


	tmp = l->c_prb * l->u.y;
	if( ABS( l->u.x ) < ABS( tmp )*l->len ){

		t = 0.5*( -l->u.x / tmp + l->len);
		h = l->c_prb * t * (t - l->len);

		p.y = t *  l->u.x  +  h * l->u.y;

		if( p.y < b->y0 )	b->y0 = p.y;
		else	b->y1 = p.y;
	}


	tmp = l->c_prb * l->u.x;
	if( ABS( l->u.y ) < ABS( tmp )*l->len ){

		t = 0.5*( +l->u.y / tmp + l->len);

		h = l->c_prb * t * (t - l->len);

		p.x = t * -l->u.y  +  h * l->u.x;

		if( p.x < b->x0 )	b->x0 = p.x;
		else	b->x1 = p.x;
	}
}
#endif