
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"

#include "Uln/PlnType.h"
#include "Uln/CrPtLine.h"



typedef struct seg_type {
	int	i0;
	int	i1;
} seg_type;


int	pt2dA_segement( pt2dA_type *apt, float a, float b, float r1, seg_type as[], int *nS );

int	dPln_segement( dPln_type *ad, int nD, float r1, seg_type as[], int *nS );

int	segA_max( seg_type as[], int nS );

int	pt2dA_segement_expand( pt2dA_type *apt, float a, float b, float r1, seg_type *s );



int
ptLine_cr( pln_type *bpl, pln_type *pl, int iPl, float r, crPln_type *cr )
{
	vec2f_type	ctr,	v;
	ln_type	*l;
	float	t,	gt;
	dPln_type	d;
	int	i,	n;


	i = 0;

	n = 0;
	for( t = 0, gt = 0, l = pl->link, ctr = pl->ctr ; gt < pl->len ; t += 4, gt += 4 ){
		for( ;  t > l->len ; l = LN_NEXT(l) ){
			ctr.x += l->v.x;
			ctr.y += l->v.y;

			t -= l->len;
		}


		ln_t2xy( &ctr, l, t, &v );

		if( ln_distance( &bpl->ctr, bpl->link, NULL, &v, &d.l, &d.u, &d.t, &d.gt ) < 0 ){
			if( i == 0 )	continue;
			break;
		}


		if( d.gt < 0 || d.gt > bpl->len ){
			if( i == 0 )	continue;
			break;
		}

		if( ABS(d.u) > 3*r )	return( -1 );

		d.sgt = gt;
		cr->d[i] = d;

		n++;

		if( i == 0 ){
			i++;
		}
	}


	if( i == 0 )	return( -1 );



	if( gt >= pl->len ){
		if( l != NULL ){
			ctr.x += l->v.x;
			ctr.y += l->v.y;
		}
		ln_distance( &bpl->ctr, bpl->link, NULL, &ctr, &d.l, &d.u, &d.t, &d.gt );

		if( d.gt >= 0 && d.gt <=  bpl->len ){
			d.sgt = pl->len;
			cr->d[i] = d;

			n++;
		}
	}

	if( n <= 1 )	return( -1 );

	cr->du = cr->d[0].u - cr->d[1].u;
	if( ABS(cr->du) > 0.75*r )	return( -1 );

	cr->u = 0.5*(cr->d[0].u + cr->d[1].u);

	cr->pl = pl;
	cr->iPl = iPl;


	return( 1 );
}


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
	seg_type	as[100];
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



//	pt2dA_segement( apt, a, b, 3, as, &nS );
	pt2dA_segement( apt, a1, b1, 3, as, &nS );



	if( nS == 0 ){
		free( ad );
		pt2dA_destroy( apt );
		return( -1 );
	}

	i0 = segA_max( as, nS );
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
pt2dA_segement( pt2dA_type *apt, float a, float b, float r1, seg_type as[], int *nS )
{
seg_type	s;
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
			if( s.i1 - s.i0 > 4 && as[(*nS)-1].i1 - as[(*nS)-1].i0 > 4 && ABS(max - min) < 0.5 ){
				as[(*nS)-1].i1 = s.i1;
				continue;
			}

		}

		i0 = s.i1;

		as[*nS] = s;
		(*nS)++;
	}

	//if( *nS > 1 ){
	//	for( i = 0 ; i < *nS ; i++ )
	//		pt2dA_segement_expand( apt, a, b, r1,&as[i] );

	//}

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
pt2dA_segement_expand( pt2dA_type *apt, float a, float b, float r1, seg_type *s )
{
float		u,	au;
int	i,	n;

	for( i = s->i0, n = 0, au = 0 ; i <= s->i1 ; i++ ){
		u = a*apt->p[i].p.x + b - apt->p[i].p.y;
		au += u;
		n++;
	}

	au /= n;

	for( i = s->i0-1 ; i >= 0 ; i-- ){
		u = a*apt->p[i].p.x + b - au - apt->p[i].p.y;
		if( ABS(u) > r1 )	break;
		s->i0 = i;
	}

	for( i = s->i1+1 ; i < apt->nP ; i++ ){
		u = a*apt->p[i].p.x + b - au - apt->p[i].p.y;
		if( ABS(u) > r1 )	break;
		s->i1 = i;
	}

	return( 1 );

}



int
dPln_segement( dPln_type *ad, int nD, float r1, seg_type as[], int *nS )
{
seg_type	s;
int	i;

	*nS = 0;
	for( i = 0 ; i < nD ; i++ ){

		for( ; i < nD ; i++ )
			if( ABS(ad[i].u) < r1 )	break;

		if( i >= nD )	break;

		s.i0 = i;
		for( ; i < nD ; i++ )
			if( ABS(ad[nD].u) > r1 )	break;

		s.i1 = i-1;

		as[(*nS)++] = s;
	}

	return( *nS );
}

int
segA_max( seg_type as[], int nS )
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


	if( c->type & 0x01 )	type |= 0x2;
	if( c->type & 0x02 )	type |= 0x1;
	c->type = type;

}



