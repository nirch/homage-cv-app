/****************************
 ***    LnAttCmpStep.c   ***
 ****************************/
#include	<stdlib.h>
#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	 "Umath/Matrix4Type.h"

#include "Uln/PlnType.h"






static int	pln_step_1( pln_type *bpl, float gt0, float wt, pln_type *pl, int fside, matrix4_type *A, double D[4] );
static int	pln_step_2( pln_type *bpl, float gt0, float wt, pln_type *pl, int fside, matrix4_type *A, double D[4] );

int
pln_fit( pln_type *pl, pln_type *bpl0, float gt0, float gt1, int fside, int cycle, float T, lnFit_type *f )
{
pln_type	*bpl;
int	i,	ret;
lt2_type	clt,	ct;
double	s;

	f->cover = 0;
	ret = -1;

	bpl = NULL;
	for( i = 0 ; i < cycle ; i++ ){

		bpl = pln_affine_lt( bpl0, &f->lt, bpl );
		PLN_DUMP( bpl, "fit", i, NULL );
		
		s = hypot( f->lt.a0, f->lt.b0 );

		if( s > 10 ){
			ret = -1;
			break;
		}

		ret = pln_fit_step( pl, bpl, gt0*s, gt1*s, fside, &clt );
		if( ret < 0 )	break;

		lt2_compose( &clt, &f->lt, &ct );
		f->lt = ct;
	}

	if( ret < 0 ){
		pln_destroy( bpl );
		return( -1 );
	}


	bpl = pln_affine_lt( bpl0, &f->lt, bpl );
	PLN_DUMP( bpl, "fit", i, "FINAL" );


	pln_fit_compare( pl, bpl, gt0*s, gt1*s, T, &f->cover, &f->dis );


	pln_destroy( bpl );

	return( ret );

}




int
	pln_fit_local( pln_type *pl, pln_type *bpl, float gt, float dt, int side, int cycle, lnFit_type *f )
{
	int ret;

	vec2f_type p;
	pln_gt2p( bpl, gt, &p );
	dPln_type	d;
	if( pln_distance( pl, &p, &d ) < 0 )
		return( -1 );


	pln_type *bpl0 = pln_copy_subR( bpl, gt-dt, gt+dt );
	//	PLN_DUMP( bpl0, "tt", apt->nA, "sub" );

	pln_type *pl0 = pln_copy_subR( pl, d.gt-4*dt, d.gt+4*dt );
	//	PLN_DUMP( pl0, "tt", apt->nA, "sub" );



	ret = pln_fit( pl0, bpl0, 0, bpl->len, side, cycle,  0.5, f );
	f->gt = gt;


	pln_destroy( bpl0 );
	pln_destroy( pl0 );


	return( ret );
}



int
pln_fit_step( pln_type *pl, pln_type *bpl, float gt0, float gt1, int fside, lt2_type *lt  )
{
matrix4_type A;
double	D[4],	X[4];
int	n;

	
	matrix4_zero( &A );
	D[0] = D[1] = D[2] = D[3 ] = 0;


	n = 0;


	n += pln_step_1( bpl, gt0, gt1, pl, fside, &A, D );
//	n += pln_step_2( bpl, gt0, gt1, pl, fside, &A, D );

	if( n < 6 )
		return( -1 );


	if( matrix4_solve( &A, D, X ) < 0 )
		return( -1 );


	lt->c0 = X[0];
	lt->a0 = X[2];
	lt->b0 = X[3];
	lt->c1 = X[1];
	lt->a1 = -X[3];
	lt->b1 =  X[2];


	return( 1 ); 

}






static int
	pln_step_1( pln_type *bpl, float gt0, float gt1, pln_type *pl, int fside, matrix4_type *A, double D[4] )
{
	float	gt,	v,	u;
	vec2f_type	p,	m,	T;
	float	H[5],	w;
	int	n;
	dPln_type	d;

	n = 0;



//	for( gt = 0 ; gt < pl->len ; gt += 2.0 ){

//		pln_gt2p( pl, gt, &p );
	float dt = 2.0;
	float t;
	vec2f_type	ctr;
	ln_type	*l;
	for( gt = t = 0, l = pl->link, ctr = pl->ctr ; gt < pl->len-0.1 ; t += dt, gt += dt ){

		for( ; t > l->len ; l = LN_NEXT(l) ){
			t -= l->len;
			ctr.x += l->v.x;
			ctr.y += l->v.y;
		}


		ln_t2xy( &ctr, l, t, &p );
//		ln_tanget( l, t, &T );



		//		pln_tangetP( pl, gt, &p, &T );

		if( pln_distance( bpl, &p, &d ) < 0 )
			continue;

		//		if( d.gt < 0 || d.gt > bpl->len )	continue;
		if( d.gt < gt0 || d.gt > gt1 )	continue;

//		if( ABS(d.u) > 16 )	continue;
		if( ABS(d.u) > 48 )	continue;


		ln_tanget( l, t, &T );

		if( fside == 1 ){
			vec2f_type	bn;
			ln_tanget( d.l, d.t, &bn );
			float t = VEC2D_INNER( bn, T );
			if( t < 0.5 )
				continue;
		}




		pln_gt2p( bpl, d.gt, &m );

//		pln_tanget( pl, gt, &T );
		u = T.y;
		v = -T.x;

		H[0] = u;
		H[1] = v;
		H[2] = (m.x*u + m.y*v);
		H[3] = (m.y*u -m.x*v);
		H[4] = (p.x*u + p.y *v);


		// weight 
		if( (w = H[2] - H[4]) < 0 )
			w = -w;
		w = 1.0 / ( 1+ 0.25*w );

		H[0] *= w;
		H[1] *= w;
		H[2] *= w;
		H[3] *= w;
		H[4] *= w;




		A->a[0][0] += H[0] * H[0];
		A->a[0][1] += H[1] * H[0];
		A->a[0][2] += H[2] * H[0];
		A->a[0][3] += H[3] * H[0];
		D[0] += H[4] * H[0];


		A->a[1][0] += H[0] * H[1];
		A->a[1][1] += H[1] * H[1];
		A->a[1][2] += H[2] * H[1];
		A->a[1][3] += H[3] * H[1];
		D[1] += H[4] * H[1];


		A->a[2][0] += H[0] * H[2];
		A->a[2][1] += H[1] * H[2];
		A->a[2][2] += H[2] * H[2];
		A->a[2][3] += H[3] * H[2];
		D[2] += H[4] * H[2];

		A->a[3][0] += H[0] * H[3];
		A->a[3][1] += H[1] * H[3];
		A->a[3][2] += H[2] * H[3];
		A->a[3][3] += H[3] * H[3];
		D[3] += H[4] * H[3];


		n++;
	}



	return( n );
}




static int
	pln_step_2( pln_type *bpl, float gt0, float gt1, pln_type *pl, int fside, matrix4_type *A, double D[4] )
{
	float	gt,	v,	u;
	vec2f_type	p,	m,	T;
	float	H[5],	w;
	int	n;
	dPln_type	d;

	n = 0;



	//	for( gt = 0 ; gt < pl->len ; gt += 2.0 ){

	//		pln_gt2p( pl, gt, &p );
	float dt = 2.0;
	float t;
	vec2f_type	ctr;
	ln_type	*l;
	for( gt = t = 0, l = bpl->link, ctr = bpl->ctr ; gt < bpl->len-0.1 ; t += dt, gt += dt ){

		for( ; t > l->len ; l = LN_NEXT(l) ){
			t -= l->len;
			ctr.x += l->v.x;
			ctr.y += l->v.y;
		}

		if( gt < gt0 || gt > gt1 )	continue;


		ln_t2xy( &ctr, l, t, &m );
	
		if( pln_distance( pl, &m, &d ) < 0 )
			continue;

	

		if( ABS(d.u) > 16 )	continue;



		ln_tanget( d.l, d.t, &T );
		if( fside == 1 ){
			vec2f_type	bn;
			ln_tanget( l, t, &bn );
			float t = VEC2D_INNER( bn, T );
			if( t < 0.5 )
				continue;
		}




		pln_gt2p( pl, d.gt, &p );
	


		u = T.y;
		v = -T.x;

		H[0] = u;
		H[1] = v;
		H[2] = (m.x*u + m.y*v);
		H[3] = (m.y*u -m.x*v);
		H[4] = (p.x*u + p.y *v);


		// weight 
		if( (w = H[2] - H[4]) < 0 )
			w = -w;
		w = 1.0 / ( 1+ w );

		H[0] *= w;
		H[1] *= w;
		H[2] *= w;
		H[3] *= w;
		H[4] *= w;




		A->a[0][0] += H[0] * H[0];
		A->a[0][1] += H[1] * H[0];
		A->a[0][2] += H[2] * H[0];
		A->a[0][3] += H[3] * H[0];
		D[0] += H[4] * H[0];


		A->a[1][0] += H[0] * H[1];
		A->a[1][1] += H[1] * H[1];
		A->a[1][2] += H[2] * H[1];
		A->a[1][3] += H[3] * H[1];
		D[1] += H[4] * H[1];


		A->a[2][0] += H[0] * H[2];
		A->a[2][1] += H[1] * H[2];
		A->a[2][2] += H[2] * H[2];
		A->a[2][3] += H[3] * H[2];
		D[2] += H[4] * H[2];

		A->a[3][0] += H[0] * H[3];
		A->a[3][1] += H[1] * H[3];
		A->a[3][2] += H[2] * H[3];
		A->a[3][3] += H[3] * H[3];
		D[3] += H[4] * H[3];


		n++;
	}



	return( n );
}

int
pln_fit_compare( pln_type *pl, pln_type *bpl, float gt0, float gt1, float dT, float *cover, float *dis )
{
	int	n;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt;

	*cover = *dis = 0;
	n = 0;

	dt = 1.0;
	for( gt = gt0 ; gt < gt1 ; gt += dt, n++ ){

		pln_gt2p( bpl, gt, &p );

		if( pln_distance( pl, &p, &d ) < 0 )
			continue;


		if( d.gt < 0 || d.gt > pl->len )	continue;
	
		if( ABS(d.u) > dT )	continue;

		*dis += ABS(d.u);

		*cover += dt;
	}

	if( n == 0 )
		return( -1 );

	*dis /= n;

	*cover /= (n*dt);

	return( 1 );
}

