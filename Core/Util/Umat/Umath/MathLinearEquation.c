/********************************
 ***   MathLinerExquation.c   ***
 ********************************/

#include	"umath.h"

#include	"Uigp/igp.h"


#define	MSIZE	3

float
math_matrix_inverse3( float DA[MSIZE][MSIZE], float B[MSIZE][MSIZE] )
{
float	A[MSIZE][MSIZE];
double	a;
int	i,	j,	k,	iMax;
float	max;

	for( i = 0 ; i < MSIZE ; i++ )
		for( j = 0 ; j < MSIZE ; j++ ){
			B[i][j] = ( i != j )? 0 : 1;
			A[i][j] = DA[i][j];
		}


	for( i = 0 ; i < MSIZE ; i++ ){
		
		max = A[i][i];
		iMax = i;
		for( k = i+1 ; k < MSIZE ; k++ )
			if( ABS(A[i][k]) > ABS(max) ){
				iMax = k;
				max = A[i][k];
			}
	
		if( i != iMax ){
			vector_swap( A[i], A[iMax], MSIZE );
			vector_swap( B[i], B[iMax], MSIZE );
		}

		a = 1.0 / A[i][i];
		vector_multiply( A[i], a, MSIZE );
		vector_multiply( B[i], a, MSIZE );

		for( j = 0 ; j < MSIZE ; j++ ){
			if( j != i ){
				a = -A[j][i];
				vector_acumalute( A[j], A[i], a, MSIZE );
				vector_acumalute( B[j], B[i], a, MSIZE ); 
			}
		}
	}
			
	return( 1 );
}



float
math_m33_det( float A[3][3] )
{
float	det;

	det =  A[0][0] * ( A[1][1] * A[2][2] - A[1][2]*A[2][1] ) -
			A[0][1] * ( A[1][0] * A[2][2] - A[1][2]*A[2][0] ) +
			A[0][2] * ( A[1][0] * A[2][1] - A[1][1]*A[2][0] );

	return( det );
}


int
math_linear_equation3( float A[3][3], float D[3], float minDet, float *det, float X[3] )
{
	*det =  A[0][0] * ( A[1][1] * A[2][2] - A[1][2]*A[2][1] ) -
			A[0][1] * ( A[1][0] * A[2][2] - A[1][2]*A[2][0] ) +
			A[0][2] * ( A[1][0] * A[2][1] - A[1][1]*A[2][0] );

	if( ABS(*det) < minDet )	return( -1 );


	X[0] =  D[0] * ( A[1][1] * A[2][2] - A[1][2]*A[2][1] ) -
			A[0][1] * ( D[1] * A[2][2] - A[1][2]*D[2] ) +
			A[0][2] * ( D[1] * A[2][1] - A[1][1]*D[2] );

	X[0] /= *det;


	X[1] =  A[0][0] * ( D[1] * A[2][2] - A[1][2]*D[2] ) -
			D[0] * ( A[1][0] * A[2][2] - A[1][2]*A[2][0] ) +
			A[0][2] * ( A[1][0] * D[2] - D[1]*A[2][0] );

	X[1] /= *det;


	X[2] =  A[0][0] * ( A[1][1] * D[2] - D[1]*A[2][1] ) -
			A[0][1] * ( A[1][0] * D[2] - D[1]*A[2][0] ) +
			D[0] * ( A[1][0] * A[2][1] - A[1][1]*A[2][0] );

	X[2] /= *det;

	return( 1 );
}