
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"

#include "Uln/PlnType.h"
#include "Uln/CrPlnType.h"



typedef struct si_type {
	int	i0;
	int	i1;
} si_type;


static int	pt2dA_segement_r( pt2dA_type *apt, float rT, si_type as[], int *nS );






int
crPln_createN( pln_type *bpl, pln_type *pl, int iPl, float r0, float r1, crPln_type cr[] )
{
vec2f_type	ctr,	v;
ln_type	*l;
float	t,	gt;
dPln_type	*ad,	*d;
int	nD,	n;
float	dt;
pt2dA_type *apt;
si_type	as[100];
int nS;
int	i;
float	T;


	n = (pl->len + 2.0 )/4;
	dt = (pl->len-0.125) /n;


	ad = (dPln_type *)malloc( (n+2) *sizeof(dPln_type));


	apt = pt2dA_alloc( n+2 );


	T = sin( ANGLE_D2R(10) );


	nD = n = 0;
	for( t = 0, gt = 0, l = pl->link, ctr = pl->ctr ; gt < pl->len ; t += dt, gt += dt ){
		for( ;  t > l->len ; l = LN_NEXT(l) ){
			ctr.x += l->v.x;
			ctr.y += l->v.y;

			t -= l->len;
		}


		ln_t2xy( &ctr, l, t, &v );



		d = &ad[nD++];
		if( lnL_distance( &bpl->ctr, bpl->link, NULL, &v, &d->l, &d->u, &d->t, &d->gt ) < 0 ||  d->gt < 0 || d->gt > bpl->len || ABS(d->u) > r1 ){
			d->l = NULL;
			d->u = 1000;
		}
		else
			n++;

		d->sgt = gt;


		apt->p[apt->nP].p.x = gt;
		apt->p[apt->nP].p.y = d->u;
		apt->nP++;

	}

	if( n < 2 ){
		free( ad );
		pt2dA_destroy( apt );
		return( 0 );
	}



	apt->p[0].r = 1000;
	if( apt->p[0].p.y < 1000 )
		apt->p[0].r = (apt->p[1].p.y - apt->p[0].p.y)/dt;

	for( i = 1 ; i < apt->nP-1 ; i++ ){
		apt->p[i].r = 1000;
		if( apt->p[i].p.y < 1000 )
			apt->p[i].r = (apt->p[i+1].p.y - apt->p[i-1].p.y)/(2*dt);
	}

	apt->p[apt->nP-1].r = 1000;
	if( apt->p[apt->nP-1].p.y < 1000 )
		apt->p[apt->nP-1].r = (apt->p[apt->nP-1].p.y - apt->p[apt->nP-2].p.y)/dt;




	pt2dA_segement_r( apt, T, as, &nS );



	for( i = 0, n = 0 ; i < nS ; i++ ){
		if( as[i].i1 - as[i].i0 < 4 )
			continue;

		cr[n].d[0] = ad[as[i].i0];
		cr[n].d[1] = ad[as[i].i1];


		cr[n].type = 0;
		if( as[i].i0 == 0 ) cr[n].type |= 0x1;
		if( as[i].i1 == apt->nP-1 ) cr[n].type |= 0x2;



		cr[n].du = cr[n].d[0].u - cr[n].d[1].u;

		cr[n].u = 0.5*(cr[n].d[0].u + cr[n].d[1].u);

		cr[n].pl = pl;
		cr[n].iPl = iPl;

		n++;
	}


	for( i = 0 ; i < n ; i++ ){
		cr[i].end = 0;
		if( i == 0 )	cr[i].end |= 0x01;
		if( i == n-1 )	cr[i].end |= 0x02;
	}


	pt2dA_destroy( apt );
	free( ad );


	return( n );
}



static int
pt2dA_segement_r( pt2dA_type *apt, float rT, si_type as[], int *nS )
{
	si_type	s;
	float		u,	min,	max;
	int	i,	n,	i0;

	i0 = 0;
	*nS = 0;
	for( i = 0 ; i < apt->nP ; i++ ){

		min = max = apt->p[i0].r;
		n = 0;
		for( n = 0 ; i < apt->nP ; i++, n++ ){
			u = apt->p[i].r;

			if( u < min )	min = u;
			if( u > max )	max = u;

			if( ABS(u) < rT )	break;
		}

		if( i >= apt->nP )	break;

		s.i0 = i;
		for( ; i < apt->nP ; i++ ){
			u = apt->p[i].r;
			if( ABS(u) > rT )	break;
		}

		s.i1 = i-1;

		if( *nS > 0 ){
			if( s.i1 - s.i0 > 4 && as[(*nS)-1].i1 - as[(*nS)-1].i0 > 4 && (ABS(max - min) < 0.5 || (s.i0 - as[(*nS)-1].i1 <= 2 && ABS(max - min) < 1.0 )) ){
				as[(*nS)-1].i1 = s.i1;
				continue;
			}

		}

		i0 = s.i1;

		as[*nS] = s;
		(*nS)++;
	}


	return( *nS );
}




