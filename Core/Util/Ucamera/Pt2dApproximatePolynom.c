#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"

#include	"Umath/Matrix.h"
#include	"Umath/Matrix3Type.h"
#include	"Umath/Matrix2Type.h"

#include	"Ucamera/Pt2dType.h"


int
pt2d_approximate_polynom_1( pt2dA_type *apt, int i0, int di, vec2f_type *p0, vec2f_type *v0, float maxDv, poly1d4_type *pl )
{
	matrix2_type	m;
	vec2d_type	D,	X;
	float	x,	y;
	float	t,	u,	t2;
	float	A,	B;
	int	i;

	vec2f_type	u0;


	u0.x = -v0->y;
	u0.y =  v0->x;




	matrix2_zero( &m );
	D.x = D.y = 0;


	for( i = i0-di ; i <= i0+di ; i++ ){
		if( i < 0 || i > apt->nP )	continue;


		x = apt->p[i].p.x - p0->x;
		y = apt->p[i].p.y - p0->y;


		t = v0->x * x + v0->y * y;

		if( ABS(t) > maxDv )
			continue;

		u = u0.x * x + u0.y * y;

		if( ABS(u) > 20 )
			continue;


		t2 = t*t;

		m.a00 += t*t;
		m.a01 += t;

		m.a11 += 1;


		D.x += u*t;
		D.y += u;
	}

	m.a10 = m.a01;



	if( m.a11 <= 4 )
		return( -1 );



	matrix2S_solve( &m, &D, &X  );

	A = X.x;
	B = X.y;

	pl->a0 = B;
	pl->a1 = A;
	pl->a2 = pl->a3 = pl->a4 = 0;



	return( m.a11 );
}


int
pt2d_approximate_polynom_2( pt2dA_type *apt, vec2f_type *p0, vec2f_type *v0, int i0, int i1, poly1d4_type *pl )
{
matrix3_type	m;
vec3d_type	D,	X;
float	x,	y;
float	t,	u,	t2;
float	A,	B,	C;
int	i;

vec2f_type	u0;


	u0.x = -v0->y;
	u0.y =  v0->x;




	matrix3_zero( &m );
	D.x = D.y = D.z = 0;


	for( i = i0 ; i < i1 ; i++ ){

		x = apt->p[i].p.x - p0->x;
		y = apt->p[i].p.y - p0->y;


		t = v0->x * x + v0->y * y;


		u = u0.x * x + u0.y * y;

		if( ABS(u) > 20 )
			continue;


		t2 = t*t;

		m.a00 += t2*t2;
		m.a01 += t*t2;
		m.a02 += t2;


		m.a12 += t;

		m.a22 += 1;


		D.x += u*t2;
		D.y += u*t;
		D.z += u;
	}

	m.a10 = m.a01;
	m.a11 = m.a02;
	m.a20 = m.a02;
	m.a21 = m.a12;


	if( m.a22 < 3 )
		return( -1 );



	matrix3S_solve( &m, &D, &X  );

	A = X.x;
	B = X.y;
	C = X.z;

	pl->a0 = C;
	pl->a1 = B;
	pl->a2 = A;
	pl->a3 = pl->a4 = 0;



	return( m.a22 );
}



int
pt2d_approximate_polynom_2O( pt2dA_type *apt, int i0, int di, vec2f_type *p0, vec2f_type *v0, float maxDv, poly1d4_type *pl )
{
	matrix3_type	m;
	vec3d_type	D,	X;
	float	x,	y;
	float	t,	u,	t2;
	float	A,	B,	C;
	int	i;

	vec2f_type	u0;


	u0.x = -v0->y;
	u0.y =  v0->x;




	matrix3_zero( &m );
	D.x = D.y = D.z = 0;


	for( i = i0-di ; i <= i0+di ; i++ ){
		if( i < 0 || i > apt->nP )	continue;


		x = apt->p[i].p.x - p0->x;
		y = apt->p[i].p.y - p0->y;


		t = v0->x * x + v0->y * y;

		if( ABS(t) > maxDv )
			continue;

		u = u0.x * x + u0.y * y;

		if( ABS(u) > 20 )
			continue;


		t2 = t*t;

		m.a00 += t2*t2;
		m.a01 += t*t2;
		m.a02 += t2;


		m.a12 += t;

		m.a22 += 1;


		D.x += u*t2;
		D.y += u*t;
		D.z += u;
	}

	m.a10 = m.a01;
	m.a11 = m.a02;
	m.a20 = m.a02;
	m.a21 = m.a12;


	if( m.a22 <= 4 )
		return( -1 );



	matrix3S_solve( &m, &D, &X  );

	A = X.x;
	B = X.y;
	C = X.z;

	pl->a0 = C;
	pl->a1 = B;
	pl->a2 = A;
	pl->a3 = pl->a4 = 0;



	return( m.a22 );
}





int
pt2d_approximate_polynom( pt2dA_type *apt, int k0, int k1, double a[], int dgree )
{
matrix_type	*M,	*B,	*IM,	*IB;
float	x,	y;
int	i,	j,	k,	nA;
float	X[100];
int	ret;

//	pt2dA_dump( apt, "axb", 1, NULL );

	nA = dgree+1;
	M = matrix_alloc( nA, nA, MT_DOUBLE );
	B = matrix_alloc( nA, 1, MT_DOUBLE );

	matrix_zero( M );
	matrix_zero( B );



	if( k0 < 0 )
		k0 = 0;

	if( k1 < 0 || k1 > apt->nP )
		k1 = apt->nP;



	
	for( k = k0 ; k < k1 ; k++ ){

		x = apt->p[k].p.x;
		y = apt->p[k].p.y;

		for( i =1, X[0] = 1 ; i < nA ; i++ )
			X[i] = X[i-1]*x;

		for( i = 0 ; i < nA ; i++ ){
			for( j = 0 ; j < nA ; j++ )
				MATRIX_ELM( M, i, j ) += X[j]*X[i];

			MATRIX_ELM( B, i, 0 ) += y * X[i];
		}
	}


	IM = matrix_alloc( nA, nA, MT_DOUBLE );
	IB = matrix_alloc( nA, 1, MT_DOUBLE );

	ret = matrix_inverse( M, IM );

	if( ret > 0 ){

//		matrix_print( "IM", IM, stdout );
//		matrix_print( "B", B, stdout );


		matrix_multiply( IM, B, IB );
//		matrix_print( "IB", IB, stdout );

		for( j = 0 ; j < nA ; j++ )
			a[j] = MATRIX_ELM( IB, 0, j );
	}



	matrix_destroy( M );
	matrix_destroy( IM );
	matrix_destroy( B );
	matrix_destroy( IB );

	return( ret );

}


#include	"Upoly/Poly1d4Type.h"

int
pt2d_approximate_polynom_test()
{
pt2dA_type *apt;
poly1d4_type f,	f1;
float	x;
int	i,	ret;

	f.a0 = 5;
	f.a1 = 4;
	f.a2 = 3;
	f.a3 = 2;
	f.a4 = 1;


	apt = pt2dA_alloc( 21 );

	for( i = 0, x = 0 ; i < 20 ; i ++, x += 1.0 ){
		apt->p[i].p.x = x;
		apt->p[i].p.y = POLY1D4_VALUE( f, x );
	}

	apt->nP = 20;



	ret = pt2d_approximate_polynom( apt, 0, apt->nP, f1.a, 4 );

	poly1d4_print( &f1, stdout );
	
	return( ret );
}