

#include	<math.h>

#include	"Ubox/Box2d.h"
#include	"Uln/LnType.h"
#include	"Uln/PlnType.h"
#include	"Uln/Dp/DpType.h"

#include	"Gpw.h"




void CGpw::Pln( plnA_type *apl, int color0, int color1 )
{
int	i;

	for( i = 0 ; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];
		if( pl->link == NULL )	continue;

		LnL( &pl->ctr, pl->link, color0, color1, 0 );//RED, BLUE );
	}
}


void CGpw::Pln( pln_type *pl, int color0, int color1, int flag )
{
	if( pl->link != NULL )
		LnL( &pl->ctr, pl->link, color0, color1, flag );//RED, BLUE );
}



void CGpw::LnL( vec2d *ctr, ln_type *link,
					int color0, int color1, int flag )
{
ln_type	*l;
vec2f_type	v;

	Marker_0( ctr->x, ctr->y, 3, color0 );

	v = *ctr;
	for( l = link ; l != NULL ; l = LN_NEXT(l) ){
		Ln( &v, l, color1, flag );
		v.x += l->v.x;
		v.y += l->v.y;
	}
}


//void
//ln_draw1( gpw_type *gpw, vec2d *ctr, ln_type *link, int color, int flag )
//{
//
//	ln_draw( gpw, ctr, link, color, flag );
//}


void
CGpw::Ln( vec2d *ctr, ln_type *link, int color, int flag )
{
vec2d   v,      w;
float   t,      h,      dt;
 
	v.x = ctr->x + link->v.x;
	v.y = ctr->y + link->v.y;

//	if( GPW_IS_IN( gpw, 5, ctr->x, ctr->y ) == 0  && GPW_IS_IN( gpw, 5, v.x, v.y ) == 0 )
//		return;


	dt = (float)(0.5*m_unit);


	if (link->a != 0)
		for( t = 0.0 ; t <= link->len ; t += dt ){
			h = link->c_prb * t * (t - link->len);
			w.x = ctr->x + t * -link->u.y  +  h * link->u.x;
			w.y = ctr->y + t *  link->u.x  +  h * link->u.y;


			Point( w.x, w.y, color );


		}
	else
		Line( ctr->x, ctr->y, v.x, v.y, color );


//	lnk_draw_direction( gpw, ctr, link, WHITE );
//	ln_draw_direction( gpw, ctr, link, color );


	if( flag ){
		Marker_0( ctr->x, ctr->y, 1,  BLACK );

		Marker_0( v.x, v.y, 1,  BLACK );
	}

}



#ifdef _AA_

void
ln_draw_direction( gpw_type *gpw, vec2d *ctr, ln_type *link, int color )
{
vec2d   w,      w1,	w2;
float   t,      h,      dt;
 



	dt = (float)(0.5*gpw->Unit);

	if( link->len < 12*dt )	return;


	t = 0.5*link->len - 6*dt;

	h = link->c_prb * t * (t - link->len);
	w1.x = ctr->x + t * -link->u.y  +  (h+4*dt) * link->u.x;
	w1.y = ctr->y + t *  link->u.x  +  (h+4*dt) * link->u.y;



	w2.x = ctr->x + t * -link->u.y  +  (h-4*dt) * link->u.x;
	w2.y = ctr->y + t *  link->u.x  +  (h-4*dt) * link->u.y;


	t += 12*dt;

	h = link->c_prb * t * (t - link->len);
	w.x = ctr->x + t * -link->u.y  +  h * link->u.x;
	w.y = ctr->y + t *  link->u.x  +  h * link->u.y;

		
	gpw_line( gpw, w.x, w.y, w1.x, w1.y, color );
	gpw_line( gpw, w1.x, w1.y, w2.x, w2.y, color );
	gpw_line( gpw, w2.x, w2.y, w.x, w.y, color );

}



void
dpA_draw( gpw_type *gpw, dpA_type *ad )
{
	dp_type	*dr;
	int	i,	j;
	vec2f_type	p1;


	for( i = 0; i < ad->Ni ; i++ ){
		dr = DPA( ad, i, 0 );
		for( j = 0 ; j < ad->Nj ; j++, dr++ ){
			if( dr->no <= 1 )	continue;

			p1.x = dr->p.x + 20 * dr->v.x;
			p1.y = dr->p.y + 20 * dr->v.y;


			gpw_line( gpw, dr->p.x, dr->p.y, p1.x, p1.y, GREEN ); 
			gpw_marker_rectangle( gpw, dr->p.x, dr->p.y, 1, BLUE ); 

		}
	}
}
#endif