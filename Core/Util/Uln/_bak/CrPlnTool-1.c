
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


int	pt2dA_segement( pt2dA_type *apt, float a, float b, float r1, si_type as[], int *nS );


int	siA_max( si_type as[], int nS );




int
crPln_create( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, crPln_type *cr )
{
vec2f_type	ctr,	v;
ln_type	*l;
float	t,	gt;
dPln_type	*ad,	*d;
int	nD,	n;
float	dt;
pt2dA_type *apt;
float	a,	b,	a1,	b1;
si_type	as[100];
int nS;
int	i0;


	n = (pl->len + 2.0 )/4;
	dt = (pl->len-0.125) /n;


	ad = (dPln_type *)malloc( (n+2) *sizeof(dPln_type));


	apt = pt2dA_alloc( n+2 );




	nD = n = 0;
	for( t = 0, gt = 0, l = pl->link, ctr = pl->ctr ; gt < pl->len ; t += dt, gt += dt ){
		for( ;  t > l->len ; l = LN_NEXT(l) ){
			ctr.x += l->v.x;
			ctr.y += l->v.y;

			t -= l->len;
		}


		ln_t2xy( &ctr, l, t, &v );



		d = &ad[nD++];
		if( ln_distance( &bpl->ctr, bpl->link, NULL, &v, &d->l, &d->u, &d->t, &d->gt ) < 0 ||  d->gt < 0 || d->gt > bpl->len ){
			d->l = NULL;
			d->u = 1000;
		}
		else	n++;

		d->sgt = gt;


		apt->p[apt->nP].p.x = gt;
		apt->p[apt->nP].p.y = d->u;
		apt->nP++;

	}

	if( n < 2 ){
		free( ad );
		pt2dA_destroy( apt );
		return( -1 );
	}



	a = b = 0;
	if( pt2dA_app_b( apt, 0, apt->nP, a, b, r1, &b ) < 2 || 
				pt2dA_app_b( apt, 0, apt->nP, a, b, r2, &b ) < 2 ){
		free( ad );
		pt2dA_destroy( apt );
		return( -1 );
	}



	pt2dA_app_ab(  apt, 0, apt->nP, a, b, r2, &a1, &b1 );

	if( ABS(a1) > 0.1 ){
		free( ad );
		pt2dA_destroy( apt );
		return( -1 );
	}



	pt2dA_segement( apt, a1, b1, 3, as, &nS );



	if( nS == 0 ){
		free( ad );
		pt2dA_destroy( apt );
		return( -1 );
	}

	i0 = siA_max( as, nS );
	if( i0 < 0 )	return( -1 );


	if( as[i0].i1 - as[i0].i0 < 4 ){
		free( ad );
		pt2dA_destroy( apt );
		return( -1 );
	}

	
	cr->type = 0;
	cr->d[0] = ad[as[i0].i0];
	cr->d[1] = ad[as[i0].i1];

	cr->type = 0;
	if( as[i0].i0 == 0 ) cr->type |= 0x1;
	if( as[i0].i1 == apt->nP-1 ) cr->type |= 0x2;



	cr->du = cr->d[0].u - cr->d[1].u;

	cr->u = 0.5*(cr->d[0].u + cr->d[1].u);

	cr->pl = pl;
	cr->iPl = iPl;

	free( ad );
	pt2dA_destroy( apt );


	return( 1 );
}


int
crPln_createN( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, crPln_type cr[] )
{
vec2f_type	ctr,	v;
ln_type	*l;
float	t,	gt;
dPln_type	*ad,	*d;
int	nD,	n;
float	dt;
pt2dA_type *apt;
float	a,	b,	a1,	b1;
si_type	as[100];
int nS;
int	i;


	n = (pl->len + 2.0 )/4;
	dt = (pl->len-0.125) /n;


	ad = (dPln_type *)malloc( (n+2) *sizeof(dPln_type));


	apt = pt2dA_alloc( n+2 );




	nD = n = 0;
	for( t = 0, gt = 0, l = pl->link, ctr = pl->ctr ; gt < pl->len ; t += dt, gt += dt ){
		for( ;  t > l->len ; l = LN_NEXT(l) ){
			ctr.x += l->v.x;
			ctr.y += l->v.y;

			t -= l->len;
		}


		ln_t2xy( &ctr, l, t, &v );



		d = &ad[nD++];
		if( ln_distance( &bpl->ctr, bpl->link, NULL, &v, &d->l, &d->u, &d->t, &d->gt ) < 0 ||  d->gt < 0 || d->gt > bpl->len ){
			d->l = NULL;
			d->u = 1000;
		}
		else	n++;

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



	a = b = 0;
	if( pt2dA_app_b( apt, 0, apt->nP, a, b, r1, &b ) < 2 || 
		pt2dA_app_b( apt, 0, apt->nP, a, b, r2, &b ) < 2 ){
			free( ad );
			pt2dA_destroy( apt );
			return( 0 );
	}



	pt2dA_app_ab(  apt, 0, apt->nP, a, b, r2, &a1, &b1 );

	if( ABS(a1) > 0.1 ){
		free( ad );
		pt2dA_destroy( apt );
		return( 0 );
	}



	pt2dA_segement( apt, a1, b1, 3, as, &nS );



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

	pt2dA_destroy( apt );
	free( ad );


	return( n );
}



int
pt2dA_segement( pt2dA_type *apt, float a, float b, float r1, si_type as[], int *nS )
{
si_type	s;
float		u,	min,	max;
int	i,	n,	i0;
	
	i0 = 0;
	*nS = 0;
	for( i = 0 ; i < apt->nP ; i++ ){

		min = max = a*apt->p[i0].p.x + b - apt->p[i0].p.y;;
		n = 0;
		for( n = 0 ; i < apt->nP ; i++, n++ ){
			u = a*apt->p[i].p.x + b - apt->p[i].p.y;

			if( u < min )	min = u;
			if( u > max )	max = u;

			if( ABS(u) < r1 )	break;
		}

		if( i >= apt->nP )	break;

		s.i0 = i;
		for( ; i < apt->nP ; i++ ){
			u = a*apt->p[i].p.x + b - apt->p[i].p.y;
			if( ABS(u) > r1 )	break;
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





typedef struct kmean1_type {
	float	m;
	float sum;
	float	n;
} kmean1_type ;

int
pt2dA_kmin_y( pt2dA_type *apt, float r1, float r2 )
{
	int	i;

	kmean1_type ak[2];

	float	m,	d,	dm;
	int	k,	km,	n,	nK;

	for( i = 0, m = 0, n = 0 ; i < apt->nP ; i++ ){
		if( ABS(apt->p[i].p.y) > r1 || ABS(apt->p[i].p.y) > r2 )
			continue;
		m += apt->p[i].p.y;
		n++;
	}


	m /= n;

	ak[0].m = m - r2;
	ak[1].m = m + r2;
	nK = 2;

	ak[0].n = ak[1].n = 0;
	ak[0].sum = ak[1].sum = 0;
	for( i = 0, n ; i < apt->nP ; i++ ){
		if( ABS(apt->p[i].p.y) > r1 || ABS(apt->p[i].p.y) > r2 )
			continue;


		km = 0;
		if( (dm = apt->p[i].p.y - ak[km].m) < 0 )	dm = -dm;

		for( k = 1; k < nK ; k++ ){
			if( (d = apt->p[i].p.y - ak[k].m) < 0 )	d = -d;
			if( d < dm ){
				dm = d;
				km = k;
			}
		}
		
		ak[km].sum += apt->p[i].p.y;
		ak[km].n++;
	}

	ak[0].m = ak[0].sum / ak[0].n;
	ak[1].m = ak[1].sum / ak[1].n;

	return( 1 );
}



int
siA_max( si_type as[], int nS )
{
int	i,	i0;

	if( nS <= 0 )	return( -1 );
	i0 = 0;
	for( i = 1; i < nS ; i++ ){
		if( as[i].i1 - as[i].i0 > as[i0].i1 - as[i0].i0 )
			i0 = i;
	}

	return( i0 );
}





void
crPln_inverse( crPln_type *c )
{
float	tmp;
ln_type *l;
int type;

	pln_inverse( c->pl );

	tmp = c->d[0].u;
	c->d[0].u = c->d[1].u;
	c->d[1].u = tmp;

	tmp = c->d[0].gt;
	c->d[0].gt = c->d[1].gt;
	c->d[1].gt = tmp;

	tmp = c->d[0].t;
	c->d[0].t = c->d[1].t;
	c->d[1].t = tmp;

	l = c->d[0].l;
	c->d[0].l = c->d[1].l;
	c->d[1].l = l;

	tmp = c->d[0].sgt;
	c->d[0].sgt = c->pl->len - c->d[1].sgt;
	c->d[1].sgt = c->pl->len - tmp;


	type = 0;
	if( c->type & 0x01 )	type |= 0x2;
	if( c->type & 0x02 )	type |= 0x1;
	c->type = type;

}



