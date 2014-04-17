/*************************
 ***   MatrixEigen.c   ***
 *************************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/umath.h"
#include	"Matrix3Type.h"



//#define USE_IPP
#ifdef USE_IPP
#pragma comment( lib, "ippm.lib" )

#include "ippm.h"
#endif

static int	matrixS3_eigen_test( matrix3_type *m, matrix3_type *em, double ev[] );



// eigen vector are column of matrix em
int
matrixS3_eigen( matrix3_type *m, matrix3_type *em, double ev[] )
{
#ifdef USE_IPP
double	buffer[3*3];
IppStatus	status;


	status = ippmEigenValuesVectorsSym_m_64f ( m->A, 3*sizeof(double), 1*sizeof(double), 
		buffer, 
		em->A, 3*sizeof(double), 1*sizeof(double),
		ev,
		3 );

	if( status == ippStsNoErr ){
//		matrixS3_eigen_test( m, em, ev );
		return( 1 );
	}
#endif

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