#include	<string.h>
#include	<stdio.h>
#include	<math.h>

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "clapack.lib" )
#else
#pragma comment( lib, "clapackD.lib" )
#endif




#include	"Uigp/igp.h"
#include	"Matrix.h"

#ifdef WIN32
#pragma warning(disable :4005)
#endif

#include "CLAPACK/SRC/f2c.h"
#include "CLAPACK/clapack.h"


//	M mXn, U mxn,    V nxn, S min(m,n)
int
matrix_svd( matrix_type *M, matrix_type **S, matrix_type **U, matrix_type **V )
{
int	n,	m;
int	lda,	ldu,	ldv;
int	l0,	l2,	lwork,	info;
double	*work;

	m = M->m;
	n = M->n;

	lda = m;
	ldu = m;
	ldv = n;

	l0 = 3*MIN( n, m ) + MAX( n, m );
	l2 = 5*MIN( n, m );

	lwork = MAX( l0, l2 ) +500;

	 work = ( double *)malloc( lwork * sizeof(double) );



	*U = matrix_alloc( M->m, M->m, MT_DOUBLE );
	*V = matrix_alloc( M->m, M->m, MT_DOUBLE );
	*S = matrix_alloc( 1,M->m, MT_DOUBLE );

	dgesvd_("A", "A", (integer *)&m, (integer *)&n, 
		(double*)M->d, (integer *)&lda, 
		(double*)(*S)->d, 
		(double*)(*V)->d, (integer *)&ldu, 
		(double*)(*U)->d, (integer *)&ldv, 
		(double*)work, (integer *)&lwork, (integer *)&info);


	free( work );


	if( info != 0 ){
		matrix_destroy( *U );
		matrix_destroy( *V );
		matrix_destroy( *S );

		return( -1 );
	}

	return( 1 );
}




//	M mXn, U mxn,    V nxn, S min(m,n)
void
matrix_svd_test( matrix_type *M, matrix_type *S, matrix_type *U, matrix_type *V  )
{
matrix_type	*A0,	*A1,	*A2;

	A0 = matrix_alloc( M->n, M->n, MT_DOUBLE );
	A1 = matrix_alloc( M->n, M->n, MT_DOUBLE );
	A2 = matrix_alloc( M->n, M->n, MT_DOUBLE );



	matrix_diagonal( A0, S->d, min( M->m, M->n ) );

	matrix_print( "A0", A0, stdout );

	matrix_multiply( A0, V, A1 );

	matrix_multiply( U, A1, A2 );

	matrix_print( "A2", A2, stdout );


	matrix_destroy( A0 );
	matrix_destroy( A1 );
	matrix_destroy( A2 );
}





//	M mXn, U mxn,    V nxn, S min(m,n)
int
matrixD_svd( double *M, int m, int n, double *S, double *U, double *V  )
{
	int	lda = m;
	int	ldu = m;
	int	ldv = n;

	int	l0 = 3*MIN( n, m ) + MAX( n, m );
	int	l2 = 5*MIN( n, m );

	int lwork = MAX( l0, l2 ) +500;

	double *work = ( double *)malloc( lwork * sizeof(double) );



	int	info;
	dgesvd_("A", "A", (integer *)&m, (integer *)&n, 
				(double*)M, (integer *)&lda, 
				(double*)S, 
				(double*)V, (integer *)&ldu, 
				(double*)U, (integer *)&ldv, 
				(double*)work, (integer *)&lwork, (integer *)&info);


	free( work );


	if( info != 0 )
		return( -1 );

	return( 1 );
}




//	M mXn, U mxn,    V nxn, S min(m,n)
void
matrixD_svd_test( double *M, int m, int n, double *S, double *U, double *V  )
{
double	A0[200],	A1[200],	A2[200];

	matrixD_diagonal( A0, m, n, S, min( m, n ) );

	matrixD_print( "A0", A0, m, n, stdout );

	matrixD_multiply( A0, m, n, V, m, A1 );

	matrixD_multiply( U, m, m, A1, n, A2 );

	matrixD_print( "A2", A2, m, n, stdout );
}








//	M mXn, U mxn,    V nxn, S min(m,n)
int
matrixF_svd( float *M, int m, int n, float *S, float *U, float *V  )
{
	int	lda = m;
	int	ldu = m;
	int	ldv = n;

	int	l0 = 3*MIN( n, m ) + MAX( n, m );
	int	l2 = 5*MIN( n, m );

	int lwork = MAX( l0, l2 ) +500;

	float *work = ( float *)malloc( lwork * sizeof(float) );



	int	info;
	sgesvd_("A", "A", (integer *)&m, (integer *)&n, 
		(float*)M, (integer *)&lda, 
		(float*)S, 
		(float*)V, (integer *)&ldu, 
		(float*)U, (integer *)&ldv, 
		(float*)work, (integer *)&lwork, (integer *)&info);


	free( work );


	if( info != 0 )
		return( -1 );

	return( 1 );
}




//	M mXn, U mxn,    V nxn, S min(m,n)
void
matrixF_svd_test( float *M, int m, int n, float *S, float *U, float *V  )
{
float	A0[200],	A1[200],	A2[200];

	matrixF_diagonal( A0, m, n, S, min( m, n ) );

	matrixF_print( "A0", A0, m, n, stdout );

	matrixF_multiply( A0, m, n, V, m, A1 );

	matrixF_multiply( U, m, m, A1, n, A2 );

	matrixF_print( "A2", A2, m, n, stdout );
}

