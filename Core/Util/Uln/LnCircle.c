/********************
 ***   LkType1.h   ***
 ********************/

#include	<stdio.h>   
#include	<math.h>
#include	<stdlib.h>


#include	"Uigp/igp.h"
#include	"Umath/Lt2Type.h"

#include	"LnType.h"





int
lnL_from_circle( vec2f_type *ctr0, float R, float dr, vec2f_type *ctr, ln_type **lnL )
{
ln_type	*l;
vec2d	p,	v;
int	i,	nL;
float	angle,	a;

	if( dr * 4 > R )	dr = R / 4;


	nL = 4;

	a = 1 - dr/R;
	angle = 2*acos( a );

	nL = 2*M_PI/angle + 1.0;

	if( nL < 4 )	nL = 4;


	a = R * (1 - cos( M_PI / nL ) );


	*lnL = lnL_alloc( nL );

	ctr->x = ctr0->x + R * cos( 0 );
	ctr->y = ctr0->y + R * sin( 0 );

	for( i = 1,v = *ctr, l = *lnL ; i < nL ; i++, l = LN_NEXT(l) ){

		angle = i*2*M_PI /nL;


		p.x = ctr0->x + R * cos( angle );
		p.y = ctr0->y + R * sin( angle );


		l->v.x = p.x - v.x;
		l->v.y = p.y - v.y;

		l->a = a;

		ln_set_aux( l );

		v.x += l->v.x;
		v.y += l->v.y;
	}

	l->v.x = ctr->x - v.x;
	l->v.y = ctr->y - v.y;

	l->a = a;

	ln_set_aux( l );


	return( 1 );
}



int
lnL_from_sub_circle( vec2f_type *ctr0, float R, vec2f_type *p0, vec2f_type *p1, float dr, vec2f_type *ctr, ln_type **lnL )
{
	ln_type	*l;
	vec2d	p,	v;
	int	i,	nL;
	float	angle,	a,	a0,	a1,	da;

	a0 = atan2( p0->y - ctr0->y, p0->x - ctr0->x );
	a1 = atan2( p1->y - ctr0->y, p1->x - ctr0->x );

	if( a0 < 0 )	a0 += 2*M_PI;
	if( a1 < 0 )	a1 += 2*M_PI;
	da = a1 - a0;



	if( dr * 4 > R )	dr = R / 4;


	nL = 4;

	a = 1 - dr/R;
	angle = 2*acos( a );

	nL = da/angle + 1.0;


	if( nL < 4 )	nL = 4;


	a = R * (1 - cos( 0.5*da / nL ) );


	*lnL = lnL_alloc( nL );

//	ctr->x = ctr0->x + R * cos( a0 );
	ctr->y = ctr0->y + R * sin( a0 );

	*ctr = *p0;

	for( i = 1,v = *ctr, l = *lnL ; i < nL ; i++, l = LN_NEXT(l) ){

		angle = a0 + i*da /nL;


		p.x = ctr0->x + R * cos( angle );
		p.y = ctr0->y + R * sin( angle );


		l->v.x = p.x - v.x;
		l->v.y = p.y - v.y;

		l->a = a;

		ln_set_aux( l );

		v.x += l->v.x;
		v.y += l->v.y;
	}

	l->v.x = p1->x - v.x;
	l->v.y = p1->y - v.y;

	l->a = a;

	ln_set_aux( l );


	return( 1 );
}