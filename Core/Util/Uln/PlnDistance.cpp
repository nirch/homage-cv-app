
#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#define _DUMP
#endif


#include "Uigp/igp.h"


#include "PlnType.h"



int
pln_distanceC( pln_type *pl, vec2f_type *p, dPln_type *d )
{
	ln_type	*l;
	int	i,	ret;
	vec2f_type	cv;
	float	cu,	ct,	dt;
	float	dis,	tmp,	len;


	d->l = NULL;
	d->u = 100;
	d->t = 100;
	ret = -1;

	dis = -1;

	cv.x = p->x - pl->ctr.x;
	cv.y = p->y - pl->ctr.y;

	d->gt = 0;
	for( l = pl->link, i = 0, len = 0 ; l != NULL ; len += l->len, l = l->p[F_END], i++ ){

		cu = l->u.x * cv.x + l->u.y * cv.y;

		ct = -l->u.y * cv.x + l->u.x * cv.y;

		cv.x -= l->v.x;
		cv.y -= l->v.y;



		if( ct < 0 ){
			tmp = cu*cu + 1.44*ct*ct;

			if( cu > 0 )	cu -= ct;
			else	cu += ct;
		}
		else	if( (dt = ct - l->len) > 0 ){

			tmp = cu*cu + 1.44*dt*dt;

			if( cu > 0 )	cu += dt;
			else	cu -= dt;
		}
		else	{
			cu -= l->c_prb*ct *(ct - l->len );
			tmp = cu*cu;
		}


		if( dis >= 0 && tmp >= dis )	continue;

		dis = tmp;


		d->u = cu;
		d->t = ct;
		d->l = l;
		d->gt = len;// + ct;
		ret = i;
	}

	if( d->l == NULL )	return( -1 );

	if( d->t < 0 ){
		if( d->l == pl->link && pl->state != PLN_CLOSE )	d->gt = d->t;
	}
	else	if( d->t > d->l->len )
		d->gt += ( LN_NEXT(d->l) == NULL && pl->state != PLN_CLOSE )? d->t : d->l->len;
	else	d->gt += d->t;


	return( ret );
}




int
pln_distance_plnC( pln_type *bpl, pln_type *pl, float *m0, float *m1 )
{
	float gt,	dt;
	vec2f_type p;
	dt = 2.0;

	dPln_type d;

	*m0 = *m1 = 0;
	int	m = -1;

	for( gt = 0 ; gt < pl->len ; gt += dt ){

		pln_gt2p( pl, gt, &p );

		if( pln_distanceC( bpl, &p, &d ) < 0 )
			continue;

		if( m < 0 ){
			*m0 = *m1 = d.u;
			m = 1;
			continue;
		}

		if( d.u < *m0 )	*m0 = d.u;
		else if( d.u > *m1 )	*m1 = d.u;
	}


	return( m );
}