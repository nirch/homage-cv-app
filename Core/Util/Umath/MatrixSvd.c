/***********************
 ***   MatrixSvd.c   ***
 ***********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/umath.h"
#include	"Umath/Matrix.h"


//#define USE_IPP
#ifdef USE_IPP
//#pragma comment( lib, "ippm.lib" )

#pragma comment( lib, "ippcorel.lib" )
#pragma comment( lib, "ippmemerged.lib" )
#pragma comment( lib, "ippmmerged.lib" )

#pragma comment( lib, "ippsemerged.lib" )
#pragma comment( lib, "ippsmerged.lib" )
#pragma comment( lib, "ippsremerged.lib" )
#pragma comment( lib, "ippsrmerged.lib" )
#include "ippm.h"
#include "ippsr.h"
#endif


static int	matrix_svd_test1( matrix_type *m, matrix_type *em, double ev[] );


int
matrixD_svd( matrix_type *A, matrix_type **U, matrix_type **W, matrix_type **V )
{
#ifdef USE_IPP
IppStatus	status;
//matrix_type *buffer;


	*V = matrix_alloc( A->nj, A->nj, MT_DOUBLE );

	*U = matrix_alloc( A->ni, A->ni, MT_DOUBLE );

	*W = matrix_alloc( 1, A->nj, MT_DOUBLE );




	status = ippsSVDSort_64f_D2( A->d, (*U)->d, A->ni,
		(*W)->d, (*V)->d, A->nj, A->nj, 100 );


//	matrix_destroy( buffer );


	if( status == ippStsNoErr || status == ippStsSingularErr ){
//		matrix_normalize( em );
//		matrixS_eigen_test( m, em, ev );
		return( 1 );
	}

//matrix_normalize( em );
#endif
	return( -1 );


//	matrix_print( "W", *W, stdout );

}

static int
matrix_svd_test1( matrix_type *m, matrix_type *em, double ev[] )
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


