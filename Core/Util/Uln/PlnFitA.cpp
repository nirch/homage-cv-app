/*********************
 ***    PlnFitA.c   ***
 *********************/
#include	<stdlib.h>
#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	 "Umath/Matrix4Type.h"
#include	 "Umath/Matrix2Type.h"

#include "Uln/PlnType.h"


int	plnA_fit_step( plnA_type *apl, pln_type *bpl, float gt0, float gt1, lt2_type *lt );

static int	pln_step_1( pln_type *bpl, float gt0, float gt1, pln_type *pl, matrix4_type *A, double D[4] );

int	plnA_fitT_step( plnA_type *apl, pln_type *bpl, float gt0, float gt1, lt2_type *lt );

int	plnA_fitV_step( plnA_type *apl, pln_type *bpl, float gt0, float gt1, vec2f_type *V, lt2_type *lt );


static int GGG = 0;


int
plnA_fitT( plnA_type *apl, pln_type *bpl0, float gt0, float gt1, int cycle, float T, lnFit_type *f )
{
	pln_type	*bpl;
	int	i,	ret;
	lt2_type	clt,	ct;
	double	s;


	bpl = NULL;


	for( i = 0 ; i < cycle ; i++ ){

		bpl = pln_affine_lt( bpl0, &f->lt, bpl );
		PLN_DUMP( bpl, "fit", i, NULL );

		s = hypot( f->lt.a0, f->lt.b0 );

		ret = plnA_fitT_step( apl, bpl, gt0*s, gt1*s, &clt );
		if( ret < 0 )	break;

		lt2_compose( &clt, &f->lt, &ct );
		f->lt = ct;
	}

	bpl = pln_affine_lt( bpl0, &f->lt, bpl );
	PLN_DUMP( bpl, "fit", i, NULL );


	plnA_fit_compare( apl, bpl, gt0*s, gt1*s, T*s, &f->cover, &f->dis );

	float dis;
	plnA_fit_compareN( apl, bpl, gt0*s, gt1*s, T*s, &f->cover1, &dis );


	pln_destroy( bpl );

	return( ret );

}



int
	plnA_fitV( plnA_type *apl, pln_type *bpl0, float gt0, float gt1, int cycle, vec2f_type *V, float T, lnFit_type *f )
{
	pln_type	*bpl;
	int	i,	ret;
	lt2_type	clt,	ct;
	double	s;


	bpl = NULL;


	for( i = 0 ; i < cycle ; i++ ){

		bpl = pln_affine_lt( bpl0, &f->lt, bpl );
		PLN_DUMP( bpl, "fit", i, NULL );

		s = hypot( f->lt.a0, f->lt.b0 );

		ret = plnA_fitV_step( apl, bpl, gt0*s, gt1*s, V, &clt );
		if( ret < 0 )	break;

		lt2_compose( &clt, &f->lt, &ct );
		f->lt = ct;
	}

	bpl = pln_affine_lt( bpl0, &f->lt, bpl );
	PLN_DUMP( bpl, "fit", i, NULL );


	plnA_fit_compare( apl, bpl, gt0*s, gt1*s, T*s, &f->cover, &f->dis );

	float dis;
	plnA_fit_compareN( apl, bpl, gt0*s, gt1*s, T*s, &f->cover1, &dis );


	pln_destroy( bpl );

	return( ret );

}




int
plnA_fit( plnA_type *apl, pln_type *bpl0, float gt0, float gt1, int cycle, float T, lnFit_type *f )
{
	pln_type	*bpl;
	int	i,	ret;
	lt2_type	clt,	ct;
	double	s;


	bpl = NULL;


	// translate
	GGG = 1;
	if( GGG == 1 ){
		for( i = 0 ; i < 8 ; i++ ){
			bpl = pln_affine_lt( bpl0, &f->lt, bpl );
			PLN_DUMP( bpl, "fit", i, "T" );

			s = hypot( f->lt.a0, f->lt.b0 );
			ret = plnA_fitT_step( apl, bpl, gt0*s, gt1*s, &clt );
			if( ret < 0 )	break;

			lt2_compose( &clt, &f->lt, &ct );
			f->lt = ct;
		}
	}




	for( i = 0 ; i < cycle ; i++ ){

		bpl = pln_affine_lt( bpl0, &f->lt, bpl );
		PLN_DUMP( bpl, "fit", i, NULL );

		s = hypot( f->lt.a0, f->lt.b0 );

		ret = plnA_fit_step( apl, bpl, gt0*s, gt1*s, &clt );
		if( ret < 0 )	break;

		lt2_compose( &clt, &f->lt, &ct );
		f->lt = ct;
	}

	bpl = pln_affine_lt( bpl0, &f->lt, bpl );
	PLN_DUMP( bpl, "fit", i, NULL );

	if( ret < 0 ){
		pln_destroy( bpl );
		return( -1 );
	}

	PLNA_DUMP( apl, "fit", i, "apl" );
	plnA_fit_compare( apl, bpl, gt0*s, gt1*s, T*s, &f->cover, &f->dis );

	float dis;
//	plnA_fit_compareN( apl, bpl, gt0*s, gt1*s, T*s, &f->cover1, &dis );

	plnA_fit_compareG( apl, bpl, gt0*s, gt1*s, T*s, 1.0, &f->cover1, &dis );
	//if( f->cover1 > 0.5 )
	//	fprintf( stdout, "FIT: %f %f %f\n", f->cover, f->cover1, cover2 );

	pln_destroy( bpl );

	return( ret );

}

int
plnA_fit_step( plnA_type *apl, pln_type *bpl, float gt0, float gt1, lt2_type *lt )
{
	matrix4_type A;
	double	D[4],	X[4];
	int	i,	n;


	matrix4_zero( &A );
	D[0] = D[1] = D[2] = D[3 ] = 0;


	n = 0;

	for( i = 0 ; i < apl->nA ; i++ )
		n += pln_step_1( bpl, gt0, gt1, apl->a[i], &A, D );


	if( n < 10 )	
		return( -1 );

	if( matrix4_solve( &A, D, X ) < 0 )
		return( -1 );


	lt->c0 = X[0];
	lt->a0 = X[2];
	lt->b0 = X[3];
	lt->c1 = X[1];
	lt->a1 = -X[3];
	lt->b1 =  X[2];


	double t = X[2]*X[2] + X[3]*X[3];
	if( t < 0.5*0.5 || t > 2.0*2.0 )
		return( -1 );

	//if( ABS(X[0]) > 100 || ABS(X[1]) > 100 )
	//	return( -1 );


	return( 1 );

}




static int
pln_step_1( pln_type *bpl, float gt0, float gt1, pln_type *pl, matrix4_type *A, double D[4] )
{
float	gt,	v,	u;
vec2f_type	p,	m,	T;
float	H[5],	w;
int	n;
dPln_type	d;

	n = 0;


	for( gt = 0 ; gt < pl->len ; gt += 2 ){

		pln_gt2p( pl, gt, &p );

		if( pln_distance( bpl, &p, &d ) < 0 )
			continue;

		if( d.gt < gt0 || d.gt > gt1 )	continue;

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

static int	pln_stepT_1( pln_type *bpl, float gt0, float gt1, pln_type *pl, matrix2_type *A, double D[2] );

int
plnA_fitT_step( plnA_type *apl, pln_type *bpl, float gt0, float gt1, lt2_type *lt )
{
	matrix2_type A;
	double	D[2],	X[2];
	int	i,	n;


	matrix2_zero( &A );
	D[0] = D[1] =  0;


	n = 0;

	for( i = 0 ; i < apl->nA ; i++ )
		n += pln_stepT_1( bpl, gt0, gt1, apl->a[i], &A, D );


	if( n < 10 )	
		return( -1 );

	if( matrix2_solve( &A, (vec2d_type *)D, (vec2d_type *)X ) < 0 )
		return( -1 );

	

	lt->c0 = X[0];
	lt->a0 = 1;
	lt->b0 = 0;
	lt->c1 = X[1];
	lt->a1 = 0;
	lt->b1 = 1;


	return( 1 );

}


static int
	pln_stepT_1( pln_type *bpl, float gt0, float gt1, pln_type *pl, matrix2_type *A, double D[2] )
{
	float	gt,	v,	u;
	vec2f_type	p,	m,	T;
	float	H[5],	w;
	int	n;
	dPln_type	d;

	n = 0;


	for( gt = 0 ; gt < pl->len ; gt += 2 ){

		pln_gt2p( pl, gt, &p );

		if( pln_distance( bpl, &p, &d ) < 0 )
			continue;

		if( d.gt < gt0 || d.gt > gt1 )	continue;

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
		D[0] += (H[4] - H[2]) * H[0];


		A->a[1][0] += H[0] * H[1];
		A->a[1][1] += H[1] * H[1];
		D[1] += (H[4] -H[2])* H[1];


		n++;
	}



	return( n );
}


static int	pln_stepV_1( pln_type *bpl, float gt0, float gt1, pln_type *pl, vec2f_type *V, double *A, double *D );


int
	plnA_fitV_step( plnA_type *apl, pln_type *bpl, float gt0, float gt1, vec2f_type *V, lt2_type *lt )
{
	double A;
	double	D,	X;
	int	i,	n;



	A = D = 0;


	n = 0;

	for( i = 0 ; i < apl->nA ; i++ )
		n += pln_stepV_1( bpl, gt0, gt1, apl->a[i], V, &A, &D );


	if( n < 10 )	
		return( -1 );

	X = D / A;


	lt->c0 = X * V->x;
	lt->a0 = 1;
	lt->b0 = 0;
	lt->c1 = X * V->y;
	lt->a1 = 0;
	lt->b1 = 1;


	return( 1 );

}


static int
	pln_stepV_1( pln_type *bpl, float gt0, float gt1, pln_type *pl, vec2f_type *V, double *A, double *D )
{
	float	gt,	v,	u;
	vec2f_type	p,	m,	T;
	float	H[5],	w;
	int	n;
	dPln_type	d;

	n = 0;


	for( gt = 0 ; gt < pl->len ; gt += 2 ){

		pln_gt2p( pl, gt, &p );

		if( pln_distance( bpl, &p, &d ) < 0 )
			continue;

		if( d.gt < gt0 || d.gt > gt1 )	continue;

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


		float H0 = V->x * H[0] + V->y * H[1];
		float H1 = H[4] - H[2];


		*A += H0*H0;
		*D += H1*H0;
	
		n++;
	}



	return( n );
}


int
plnA_fit_compare( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float *cover, float *dis )
{
	int	n,	i;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt;

	*cover = *dis = 0;
	n = 0;

	dt = 2.0;
	for( gt = gt0 ; gt < gt1 ; gt += dt, n++ ){

		pln_gt2p( bpl, gt, &p );

		for( i = 0 ; i < apl->nA ; i++ ){

			if( pln_distance( apl->a[i], &p, &d ) < 0 )
				continue;


			if( d.gt < 0 || d.gt > apl->a[i]->len )	continue;

			if( ABS(d.u) > dT )	continue;
			break;
		}

		if( i >= apl->nA )
			continue;




		*dis += ABS(d.u);


		*cover += dt;
	}

	if( n == 0 )
		return( -1 );

	*dis /= n;

	*cover /= (n*dt);

	return( 1 );
}


int
	plnA_fit_compareN( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float *cover, float *dis )
{
	int	n,	n1,	i;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt;

	*cover = *dis = 0;
	n = n1 = 0;

	dt = 2.0;
	for( gt = gt0 ; gt < gt1 ; gt += dt, n++ ){

		pln_gt2p( bpl, gt, &p );

		for( i = 0 ; i < apl->nA ; i++ ){

			if( pln_distance( apl->a[i], &p, &d ) < 0 )
				continue;


			if( d.gt < 0 || d.gt > apl->a[i]->len )	continue;

			if( ABS(d.u) > dT )	continue;
			break;
		}

		if( i >= apl->nA )
			continue;


		vec2f_type	m,	T,	T0;
		//pln_gt2p( apl->a[i], d.gt, &m );
		pln_tangetP( apl->a[i], d.gt, &m, &T0 );

		pln_tanget( bpl, gt, &T );

		float a = VEC2D_INNER( T, T0 );

		if( ABS(a) < 0.7660 )		// cos( 40 )
			continue;


		float u = T.y;
		float v = -T.x;

		vec2f_type	dp;
		dp.x = m.x - p.x;
		dp.y = m.y - p.y;

		float t = dp.x * u + dp.y*v;
		if( t < 0 )	t = -t;
//		if( ABS( t) < 0.25 )	continue;
		//if( t < 0.25 )
		//	continue;
		
		float w = VEC2D_INNER( dp, dp );

		if( t < 0.25 * w )
			continue;

		if( t > 0 )
			w /= t;

//		if( w < 0 )	w = -w;

		w -= 2;
		if( w < 0 )	w = 0;

		w = 1 / ( 1 + w );


		*dis += ABS(d.u);


		*cover += dt * w;
		n1++;
	}

	if( n1 == 0 )
		return( -1 );

	*dis /= n1;

	*cover /= (n*dt);

	return( 1 );
}


int
plnA_fit_compareG( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float W, float *cover, float *dis )
{
	int	n,	n1,	i;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt;

	*cover = *dis = 0;
	n = n1 = 0;

	dt = 2.0;
	for( gt = gt0 ; gt < gt1 ; gt += dt, n++ ){

		pln_gt2p( bpl, gt, &p );

		for( i = 0 ; i < apl->nA ; i++ ){

			if( pln_distance( apl->a[i], &p, &d ) < 0 )
				continue;


			if( d.gt < 0 || d.gt > apl->a[i]->len )	continue;

			if( ABS(d.u) > dT )	continue;
			break;
		}

		if( i >= apl->nA )
			continue;


		vec2f_type	m,	T,	T0;
		pln_tangetP( apl->a[i], d.gt, &m, &T0 );

		pln_tanget( bpl, gt, &T );

		float a = VEC2D_INNER( T, T0 );
		if( a < 0 )	a = -a;

		if( ABS(a) < 0.7660 )		// cos( 40 )
			continue;


		float u = T.y;
		float v = -T.x;

		vec2f_type	dp;
		dp.x = m.x - p.x;
		dp.y = m.y - p.y;


		float w = hypot( dp.x, dp.y );

		w -= W;
		if( w < 0 )	w = 0;

		w = a / ( 1 + w );


		*dis += ABS(d.u);


		*cover += dt * w;
		n1++;
	}

	if( n1 == 0 )
		return( -1 );

	*dis /= n1;

	*cover /= (n*dt);

	return( 1 );
}


int
	plnA_fit_compareW( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float W, float *cover, float *dis )
{
	int	n,	n1,	i;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt;

	*cover = *dis = 0;
	n = n1 = 0;

	dt = 2.0;
	for( gt = gt0 ; gt < gt1 ; gt += dt, n++ ){

		pln_gt2p( bpl, gt, &p );

		for( i = 0 ; i < apl->nA ; i++ ){

			if( pln_distance( apl->a[i], &p, &d ) < 0 )
				continue;


			if( d.gt < 0 || d.gt > apl->a[i]->len )	continue;

			if( ABS(d.u) > dT )	continue;
			break;
		}

		if( i >= apl->nA )
			continue;


		vec2f_type	m,	T;
		pln_gt2p( apl->a[i], d.gt, &m );

		pln_tanget( bpl, gt, &T );
		float u = T.y;
		float v = -T.x;

		vec2f_type	dp;
		dp.x = m.x - p.x;
		dp.y = m.y - p.y;

		float t = dp.x * u + dp.y*v;
		if( t < 0 )	t = -t;
		//		if( ABS( t) < 0.25 )	continue;
		if( t < 0.25 )	continue;

		float w = VEC2D_INNER( dp, dp );
		w /= t;

		//		if( w < 0 )	w = -w;

		w -= W;
		if( w < 0 )	w = 0;

		w = 1 / ( 1 + w );


		*dis += ABS(d.u);


		*cover += dt * w;
		n1++;
	}

	if( n1 == 0 )
		return( -1 );

	*dis /= n1;

	*cover /= (n*dt);

	return( 1 );
}


#ifdef _AA_
int
plnA_fit_compare( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float *cover, float *dis )
{
	int	i;
	float	len;

	*cover = *dis = len = 0;
	
	for( i = 0 ; i < apl->nA ; i++ )
		pln_fit_compare( apl->a[i], bpl, gt0, gt1, dT, cover, dis, &len );

	*dis /= len;
	*cover /= len;

	return( 1 );
}


static int
pln_fit_compare( pln_type *pl, pln_type *bpl, float gt0, float gt1, float dT, float *cover, float *dis, float *len )
{
	int	n;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt;

	*cover = *dis = 0;
	n = 0;

	dt = 2.0;
	for( gt = 0 ; gt < pl->len ; gt += dt, n++ ){

		pln_gt2p( pl, gt, &p );

		if( pln_distance( bpl, &p, &d ) < 0 )
			continue;

		if( d.gt < gt0 || d.gt > gt1 )	continue;


		if( ABS(d.u) > dT )	continue;

		*dis += ABS(d.u);

		*cover += dt;
	}

	*len = gt - dt;


	return( 1 );
}
#endif


