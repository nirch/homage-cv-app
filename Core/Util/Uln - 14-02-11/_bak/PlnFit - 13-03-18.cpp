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


static int	pln_step_1( pln_type *bpl, float gt0, float wt, pln_type *pl, matrix4_type *A, double D[4] );


int
pln_fitC( pln_type *pl, float gt, float wt, pln_type *bpl0, int cycle, lt2_type *lt )
{
pln_type	*bpl;
int	i,	ret;
lt2_type	clt,	ct;
double	s;


	
	
	bpl = NULL;
	for( i = 0 ; i < cycle ; i++ ){

		bpl = pln_affine_lt( bpl0, lt, bpl );
		PLN_DUMP( bpl, "fit", i, NULL );
		
		s = hypot( lt->a0, lt->b0 );

		ret = pln_fit_step( pl, gt, s*wt, bpl, &clt );
		if( ret < 0 )	break;

		lt2_compose( &clt, lt, &ct );
		*lt = ct;
	}
	bpl = pln_affine_lt( bpl0, lt, bpl );
	PLN_DUMP( bpl, "fit", i, NULL );


	pln_destroy( bpl );

	return( ret );

}

int
pln_fit( pln_type *pl, float gt, float wt, pln_type *bpl, lt2_type *lt )
{
	int	ret;

	ret = pln_fit_step( pl, gt, wt, bpl, lt );

	return( ret );

}

int
pln_fit_step( pln_type *pl, float gt, float wt, pln_type *bpl, lt2_type *lt )
{
matrix4_type A;
double	D[4],	X[4];
int	n;

	
	matrix4_zero( &A );
	D[0] = D[1] = D[2] = D[3 ] = 0;


	n = 0;


	n += pln_step_1( bpl, gt, wt, pl, &A, D );

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
pln_step_1( pln_type *bpl, float gt0, float wt, pln_type *pl, matrix4_type *A, double D[4] )
{
float	gt,	v,	u;
vec2f_type	p,	m,	T;
float	H[5],	w;
int	n;
dPln_type	d;

	n = 0;


	for( gt = gt0 - wt ; gt < gt0 + wt ; gt += 2.0 ){
		if( gt < 0 || gt > pl->len )	continue;

		pln_gt2p( pl, gt, &p );

		if( pln_distance( bpl, &p, &d ) < 0 )
			continue;

		if( d.gt < 0 || d.gt > bpl->len )	continue;

		if( ABS(d.u) > 16 )	continue;


	


		pln_gt2p( bpl, d.gt, &m );
		
		pln_tanget( pl, gt, &T );
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
pln_fit_compare( pln_type *pl, pln_type *bpl, float gt0, float wt, float dT, float *c0, float *c1, float *dis )
{
	int	n;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt;

	*c0 = *c1 = *dis = 0;
	n = 0;

	dt = 2.0;
	for( gt = gt0 - wt ; gt < gt0 + wt ; gt += dt ){

		pln_gt2p( bpl, gt, &p );

		if( pln_distance( pl, &p, &d ) < 0 )
			continue;

		if( ABS(d.u) > dT )	continue;

		*dis += ABS(d.u);

		if( gt < gt0 )
			(*c0) += dt;
		else (*c1) += dt;

		n++;
	}

	if( n == 0 )
		return( -1 );

	*dis /= n;


	return( 1 );

}



int
pln_fit_compare_ltA( pln_type *pl, pln_type *bpl, float gt0, float wt, lt2_type *lt,  float dT, float *c0, float *c1, float *dis )
{
	int	n;
	vec2f_type	p,	p1;
	dPln_type	d;
	float	gt,	dt;


	*c0 = *c1 = *dis = 0;
	n = 0;

	pln_gt2p( bpl, gt0, &p );
	p1.x = LT2_F1( *lt, p.x, p.y );
	p1.y = LT2_F2( *lt, p.x, p.y );



	float scale = hypot( lt->a0, lt->b0 );


	dt = 2.0/scale;
	for( gt = gt0 - wt ; gt < gt0 + wt ; gt += dt ){

		pln_gt2p( bpl, gt, &p );


		p1.x = LT2_F1( *lt, p.x, p.y );
		p1.y = LT2_F2( *lt, p.x, p.y );


		if( pln_distance( pl, &p1, &d ) < 0 )
			continue;

		if( ABS(d.u) > dT )	continue;

		*dis += ABS(d.u);

		if( gt < gt0 )
			(*c0) += 2;
		else (*c1) += 2;

		n++;
	}

	if( n == 0 )
		return( -1 );

	*dis /= n;


	return( 1 );

}


int
pln_fit_compare_lt( pln_type *pl, pln_type *bpl, float gt0, float wt, lt2_type *lt,  float dT, float *c0, float *c1, float *dis )
{
	int	n;
	vec2f_type	p,	p1;
	dPln_type	d;
	float	gt;

	*c0 = *c1 = *dis = 0;
	n = 0;

	pln_gt2p( bpl, gt0, &p );
	p1.x = LT2_F1( *lt, p.x, p.y );
	p1.y = LT2_F2( *lt, p.x, p.y );



	float scale = hypot( lt->a0, lt->b0 );


//	dt = 2.0/scale;
	for( gt = gt0 - wt ; gt < gt0 + wt ; gt += 2.0 ){

		pln_gt2p( bpl, gt, &p );


		p1.x = LT2_F1( *lt, p.x, p.y );
		p1.y = LT2_F2( *lt, p.x, p.y );


		if( pln_distance( pl, &p1, &d ) < 0 )
			continue;

		if( ABS(d.u) > dT )	continue;

		*dis += ABS(d.u);

		if( gt < gt0 )
			(*c0) += 2.0;
		else (*c1) += 2.0;

		n++;
	}

//	*c0 /= scale;

	if( n == 0 )
		return( -1 );

	*dis /= n;


	return( 1 );

}