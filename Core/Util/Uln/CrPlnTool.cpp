
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"
#include	"Ulog/Log.h"

#include "Uln/PlnType.h"
#include "Uln/CrPlnType.h"



typedef struct si_type {
	int	i0;
	int	i1;
} si_type;


int	pt2dA_segement( pt2dA_type *apt, float a, float b, float r1, si_type as[], int *nS );


int	siA_max( si_type as[], int nS );



crPlnA_type *
crPlnA_alloc ( int n )
{
	crPlnA_type *ac = ( crPlnA_type *)malloc( sizeof( crPlnA_type) );

	ac->NA = n;
	ac->nA = 0;
	ac->a = ( crPln_type *)malloc( ac->NA*sizeof( crPln_type) );

	return( ac );
}

void
crPlnA_destroy( crPlnA_type *ac )
{
	free( ac->a );

	free( ac );
}


void
crPlnA_destroy_cr( crPlnA_type *ac, int i0 )
{
int	i;

	ac->nA--;

	for( i = i0 ; i < ac->nA ; i++ )
		ac->a[i] = ac->a[i+1];
}


//int
//crPln_createA( pln_type *bpl, plnA_type *apl, int i0,  float r1, float r2, int affine, crPln_type acr[], int *nCr )
//{
//	int	i,	n;
//
//	for( i = 0, n = 0 ; i < apl->nA ; i++ ){
//
//		if( i == i0 )	continue;
//		if( apl->a[i] == NULL || apl->a[i]->link == NULL )
//			continue;
//
//		n += crPln_create( bpl, apl->a[i], i, r1, r2, affine, &acr[n] );
//	}
//
//	*nCr = n;
//
//	return( n );
//}



int
crPln_createA( pln_type *bpl, plnA_type *apl, int i0,  float r1, float r2, int affine, crPlnA_type *ac )
{
	int	i,	n;

	box2f_type	b0,	b1;
	pln_box( bpl, &b0 );

	for( i = 0, n = 0 ; i < apl->nA ; i++ ){

		if( i == i0 )	continue;
		if( apl->a[i] == NULL || apl->a[i]->link == NULL )
			continue;

		pln_box( apl->a[i], &b1 );
		if( box2f_distance( &b0, &b1 ) > 8 )
			continue;

		n += crPln_create( bpl, apl->a[i], i, r1, r2, affine, &ac->a[n] );
	}


	ac->nA = n;

	return( n );
}



int
crPln_create( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, int affine, crPln_type cr[] )
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


	n = (pl->len + 2.0 )/2;
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
		if( lnL_distance( &bpl->ctr, bpl->link, NULL, &v, &d->l, &d->u, &d->t, &d->gt ) < 0 ||  d->gt < 0 || d->gt > bpl->len ){
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
	a1 = b1 = 0;
	if( affine == 1 ){
		if( pt2dA_app_b( apt, 0, apt->nP, a, b, r1, &b ) < 2 || 
			pt2dA_app_b( apt, 0, apt->nP, a, b, r2, &b ) < 2 ){
				free( ad );
				pt2dA_destroy( apt );
				return( 0 );
		}



		pt2dA_app_ab(  apt, 0, apt->nP, a, b, r2, &a1, &b1 );

		if( ABS(a1) > 0.15 ){
			free( ad );
			pt2dA_destroy( apt );
			return( 0 );
		}
	}



	pt2dA_segement( apt, a1, b1, r2, as, &nS );



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


//		cr[i]->end = 0;
//		if( i == 0 )	cr->end |= 0x01;
//		if( i == nS-1 )	cr->end |= 0x02;


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



int
	crPlnA_parallel( pln_type *bpl, plnA_type *apl, int i0,  float r1, float r2, float aT, crPlnA_type *ac )
{
	int	i,	n;

	box2f_type	b0,	b1;
	pln_box( bpl, &b0 );

	for( i = 0, n = 0 ; i < apl->nA ; i++ ){

		if( i == i0 )	continue;
		if( apl->a[i] == NULL || apl->a[i]->link == NULL )
			continue;

		pln_box( apl->a[i], &b1 );
		if( box2f_distance( &b0, &b1 ) > 8 )
			continue;


	
		n += crPln_create_parallel( bpl, apl->a[i], i, r1, r2, aT, &ac->a[n] );
	}


	ac->nA = n;

	return( n );
}

int
crPln_create_parallel( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, float aT, crPln_type cr[] )
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


	n = (pl->len + 2.0 )/2;
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
		if( lnL_distance( &bpl->ctr, bpl->link, NULL, &v, &d->l, &d->u, &d->t, &d->gt ) < 0 ||  d->gt < 0 || d->gt > bpl->len ){
			apt->p[apt->nP].p.x = gt;
			apt->p[apt->nP].p.y = 1000;
			apt->nP++;
			continue;
		}

		vec2f_type	v0,	v1;
		ln_tanget( l, t, &v0 );
		ln_tanget( d->l, d->t, &v1 );

		a = VEC2D_INNER( v0, v1 );

		if( ABS(a) < aT ){
			apt->p[apt->nP].p.x = gt;
			apt->p[apt->nP].p.y = 1000;
			apt->nP++;
			continue;

		}


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



	a = b = 0;
	a1 = b1 = 0;




	pt2dA_segement( apt, a1, b1, r2, as, &nS );



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
crPln_inverse( crPln_type *c, int fPl )
{
float	tmp;
ln_type *l;
int type;

	if( fPl == 1 )
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


	c->end = ((c->end<<1) & 0x02) |  ((c->end>>1) & 0x01);
}






int
crPln_duplicate( plnA_type *apl, int i0, float r, crPln_type acr[] )
{
	vec2f_type	ctr,	v;
	ln_type	*l;
	float	t,	gt;
	dPln_type	d;
	int	nD,	n;
	float	dt;
	int	i,	iPl;
	crPln_type *cr;

	pln_type	*pl = apl->a[i0];


	n = (pl->len + 2.0 )/4;
	dt = (pl->len-0.125) /n;





	cr = NULL;

	nD = n = 0;
	for( t = 0, gt = 0, l = pl->link, ctr = pl->ctr ; gt < pl->len ; t += dt, gt += dt ){

		for( ;  t > l->len ; l = LN_NEXT(l) ){
			ctr.x += l->v.x;
			ctr.y += l->v.y;

			t -= l->len;
		}


		ln_t2xy( &ctr, l, t, &v );



		iPl = plnA_selectI( apl, &v, i0, r, &d );
		if( iPl < 0 ){
			cr = NULL;
			continue;
		}



		d.sgt = gt;

		if( cr == NULL || cr->iPl != iPl ){
			cr = &acr[nD++];
			cr->iPl = iPl;
			cr->pl = apl->a[iPl];
			cr->d[0] = d;
			cr->u = d.u;
		}
		cr->d[1] = d;
		if( ABS(d.u) < ABS(cr->u) )	cr->u = d.u;
	}



	for( i = 0 ; i < nD ; i++ ){
		cr = &acr[i];
		

		if( cr->d[0].gt < cr->d[01].gt ){
			cr->d[0].sgt = cr->d[0].gt;
			cr->d[1].sgt = cr->d[1].gt;
		}
		else	{
			cr->d[0].sgt = cr->d[1].gt;
			cr->d[1].sgt = cr->d[0].gt;
		}

		pln_gt2p( cr->pl, cr->d[0].sgt, &v );
		lnL_distance( &pl->ctr, pl->link, NULL, &v, &cr->d[0].l, &cr->d[0].u, &cr->d[0].t, &cr->d[0].gt );



		pln_gt2p( cr->pl, cr->d[1].sgt, &v );
		lnL_distance( &pl->ctr, pl->link, NULL, &v, &cr->d[1].l, &cr->d[1].u, &cr->d[1].t, &cr->d[1].gt );


		cr->du = cr->d[0].u - cr->d[1].u;

//		cr[i].u = 0.5*(cr[i].d[0].u + cr[i].d[1].u);
	}

	return( nD );
}




int
crLineA_oreder_side( crPln_type acr[], int nAcr, int side, crPln_type *ad[], int *nD )
{
	int	i,	j,	n;


	for( i = 0, n = 0; i < nAcr ; i++ ){

		if( acr[i].pl == NULL || acr[i].pl->link == NULL )	continue;
		if( acr[i].u*side < 0 )	continue;


		// need  inverse
		if( acr[i].d[0].gt > acr[i].d[1].gt ){
			crPln_inverse( &acr[i], 1 );
			for( j = i+1 ; j < nAcr ; j++ ){
				if( acr[j].pl == acr[i].pl )
					crPln_inverse( &acr[j], 0 );
			}
		}


		for( j = n ; j > 0 ; j-- ){
			if( acr[i].d[0].gt > ad[j-1]->d[0].gt )
				break;

			ad[j] = ad[j-1];
		}


		ad[j] = &acr[i];
		n++;
	}

	*nD = n;

	return( *nD );
}



int
crPlnA_union( crPln_type acr[], int *nAcr )
{
	int	i,	j;

	if( *nAcr == 0 )
		return(0);

	for( i = 1, j = 1; i < *nAcr ; i++ ){

		if( acr[j-1].iPl != acr[i].iPl ){
			acr[j] = acr[i];
			j++;
			continue;
		}

		float t = acr[i].d[0].sgt - acr[j-1].d[1].sgt;
		if( t > 40 ){
			acr[j] = acr[i];
			j++;
			continue;
		}


		acr[j-1].d[1] = acr[i].d[1];
		acr[j-1].end |= acr[i].end & 0x02;
		acr[j-1].type |= acr[i].type & 0x02;

		if( ABS(acr[j-1].u) < ABS(acr[i].u) )	acr[j-1].u = acr[i].u;
	}

	*nAcr = j;


	return( *nAcr );
}




void
crPlnA_direction( crPlnA_type *ac )
{
	int	i;

	
	for( i = 0 ; i < ac->nA ; i++ ){
		crPln_type *cr = &ac->a[i];

		if( cr->d[0].gt < cr->d[1].gt ){
			cr->direction = 0;
			cr->gt0 = cr->d[0].gt;
			cr->gt1 = cr->d[1].gt;
		}

		else {
			cr->direction = 1;
			cr->gt0 = cr->d[1].gt;
			cr->gt1 = cr->d[0].gt;
		}
	}

}


int
crPlnA_order( crPlnA_type *ac )
{
	int	i,	j,	j0;


	for( i = 0 ; i < ac->nA-1 ; i++ ){

		j0 = i;
		for( j = i+1 ; j < ac->nA ; j++ ){
			if( ac->a[j].gt0 < ac->a[j0].gt0 )	j0 = j;
		}

		if( i != j0 ){
			crPln_type cr = ac->a[i];
			ac->a[i] = ac->a[j0];
			ac->a[j0] = cr;
		}
	}

	return( 1 );
}



void
crPlnA_print( crPlnA_type *ac, FILE *fp )
{
	int	i;


	for( i = 0 ; i < ac->nA ; i++ ){
		crPln_type *cr = &ac->a[i];
		fprintf( fp, "%.2f %.2f(%d)  ", cr->gt0, cr->gt1, cr->direction );
	}

	fprintf( fp, "\n" );
}