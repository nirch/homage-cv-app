/*************************
 ***   MatrixEigen.c   ***
 *************************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/umath.h"
#include	"Matrix3Type.h"
#include	"Matrix.h"


#define USE_IPP
#ifdef USE_IPP
#pragma comment( lib, "ippm.lib" )

#include "ippm.h"
#endif


static int	matrixS_eigen_test( matrix_type *m, matrix_type *em, double ev[] );

static void	matrix_normalize( matrix_type *m );


int
matrixSD_eigen( matrix_type *m, matrix_type *em, double ev[] )
{
IppStatus	status;
matrix_type *buffer;

	buffer = matrix_alloc( m->n, m->n, MT_DOUBLE );


	status = ippmEigenValuesVectorsSym_m_64f ( m->d, m->n*sizeof(double), 1*sizeof(double), 
		buffer->d, 
		em->d, m->n*sizeof(double), 1*sizeof(double),
		ev,
		m->n );

	matrix_destroy( buffer );


	if( status == ippStsNoErr || status == ippStsSingularErr ){
		matrix_normalize( em );
		matrixS_eigen_test( m, em, ev );
		return( 1 );
	}

matrix_normalize( em );
	return( -1 );


}

static int
matrixS_eigen_test( matrix_type *m, matrix_type *em, double ev[] )
{
	matrix_type	*c;
	double	d[100],	x[100],	b,	tmp;
	int	i,	j;

	fprintf( stdout, "Test Eigen:  " );

	c = matrix_alloc( m->n, m->n,MT_DOUBLE );
	matrix_multiply( m, em, c );
//	matrix_print( "m*em", c, stdout );


	for( i = 0 ; i < m->n ; i++ ){

		matrixD_get_column( em, i, d );

		matrixD_multV( m, d, x );

		for( j = 0, tmp = 0 ; j < m->n ; j++ ){
			b = x[j] - ev[i] * d[j];

			tmp += b;
		}

		fprintf( stdout, "%f  ", tmp );
	}

	fprintf( stdout, "\n\n", tmp );

	matrix_destroy( c );

	return( 1 );

}


static int	matrixS3_eigen_test( matrix3_type *m, matrix3_type *em, double ev[] );

// eigen vector are column of matrix em
int
matrixS3_eigen( matrix3_type *m, matrix3_type *em, double ev[] )
{
double	buffer[3*3];
IppStatus	status;


	status = ippmEigenValuesVectorsSym_m_64f ( m->A, 3*sizeof(double), 1*sizeof(double), 
		buffer, 
		em->A, 3*sizeof(double), 1*sizeof(double),
		ev,
		3 );

	if( status == ippStsNoErr ){
		matrixS3_eigen_test( m, em, ev );
		return( 1 );
	}

	return( -1 );

	
}

static int
matrixS3_eigen_test( matrix3_type *m, matrix3_type *em, double ev[] )
{
matrix3_type	c;
vec3d_type	d,	x,	b;
int	i;
double	tmp;

fprintf( stdout, "Test Eigen:  " );

	matrix3_mult( m, em, &c );

	for( i = 0 ; i < 3 ; i++ ){

		d.x = em->a[0][i];
		d.y = em->a[1][i];
		d.z = em->a[2][i];

		matrix3_multVD( m, &d, &x );

		b.x = x.x - ev[i]*d.x;
		b.y = x.y - ev[i]*d.y;
		b.z = x.z - ev[i]*d.z;

		tmp = ABS(b.x) + ABS(b.y) + ABS(b.z );

		fprintf( stdout, "%f  ", tmp );
	}

	fprintf( stdout, "\n\n", tmp );


	return( 1 );

}

static void	matrix_normalize_column( matrix_type *m, int column );

static void
matrix_normalize( matrix_type *m )
{
	int	j;
	for( j = 0 ; j < m->nj ; j++ )
		matrix_normalize_column( m, j );
}


static void
matrix_normalize_column( matrix_type *m, int column )
{
	int	i;
	double	*d,	maxD;

	for( i = 0, d = m->d + column, maxD = 0 ; i < m->ni ; i++, d += m->nj ){
		if(ABS(*d) > maxD )
			maxD = *d;
	}

	if( maxD < 0 ){
		for( i = 0, d = m->d + column, maxD = 0 ; i < m->ni ; i++, d += m->nj ){
			*d = - *d;
		}
	}
}
