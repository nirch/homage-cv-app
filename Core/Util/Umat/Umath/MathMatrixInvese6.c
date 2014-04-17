/********************************
 ***   MathLinerExquation.c   ***
 ********************************/

#include	"umath.h"

#include	"Uigp/igp.h"






float
math_matrix_inverse6( float DA[6][6], float B[6][6] )
{
float	A[6][6];
double	a;
int	i,	j,	k,	iMax;
float	max;

	for( i = 0 ; i < 6 ; i++ )
		for( j = 0 ; j < 6 ; j++ ){
			B[i][j] = ( i != j )? 0 : 1;
			A[i][j] = DA[i][j];
		}


	for( i = 0 ; i < 6 ; i++ ){
		
		max = A[i][i];
		iMax = i;
		for( k = i+1 ; k < 6 ; k++ )
//			if( A[i][k] > max ){
			if( ABS(A[i][k]) > ABS(max) ){
				iMax = k;
				max = A[i][k];
			}
	
		if( i != iMax ){
			vector_swap( A[i], A[iMax], 6 );
			vector_swap( B[i], B[iMax], 6 );
		}

		a = 1.0 / A[i][i];
		vector_multiply( A[i], a, 6 );
		vector_multiply( B[i], a, 6 );

		for( j = 0 ; j < 6 ; j++ ){
			if( j != i ){
				a = -A[j][i];
				vector_acumalute( A[j], A[i], a, 6 );
				vector_acumalute( B[j], B[i], a, 6 ); 
			}
		}
	}
			
	return( 1 );
}



void
math_matrix66_multiply( float A[6][6], float B[6][6], float C[6][6] )
{
int	i,	j,	k;
double	sum;
	for( i = 0 ; i < 6 ; i++ )
		for( j = 0 ; j < 6 ; j++ ){

			sum = 0;
			for( k = 0 ; k < 6 ; k ++ )
				sum += A[i][k] * B[k][j];

			C[i][j] = sum;
		}
}


void
math_matrix66_multiply1( float A[6][6], float B[6], float C[6] )
{
int	i,	k;
double	sum;
	for( i = 0 ; i < 6 ; i++ ){
		sum = 0;
		for( k = 0 ; k < 6 ; k ++ )
			sum += A[i][k] * B[k];

		C[i] = sum;
	}
}

float
math_linear_equation6( float DA[6][6], float D[6], float X[6] )
{
float	A[6][7];
double	a;
int	i,	j,	k,	iMax;
float	max;

	for( i = 0 ; i < 6 ; i++ )
		for( j = 0 ; j < 6 ; j++ ){
			A[i][j] = DA[i][j];
		}

	for( i = 0 ; i < 6 ; i++ )
		A[i][6] = D[i];

	for( i = 0 ; i < 6 ; i++ ){
		
		max = A[i][i];
		iMax = i;
		for( k = i+1 ; k < 6 ; k++ )
			if( A[i][k] > max ){
				iMax = k;
				max = A[i][k];
			}
	
		if( i != iMax ){
			vector_swap( A[i], A[iMax], 7 );
		}

		a = 1.0 / A[i][i];
		vector_multiply( A[i], a, 7 );

		for( j = 0 ; j < 6 ; j++ ){
			if( j != i ){
				a = -A[j][i];
				vector_acumalute( A[j], A[i], a, 7 );
			}
		}
	}

	for( i = 0 ; i < 6 ; i++ )
		X[i] = A[i][6];

	return( 1 );
}

/*
float
math_linear_equation4( float DA[4][4], float D[4], float X[4] )
{
float	A[4][4+1];
double	a;
int	i,	j,	k,	iMax;
float	max;

	for( i = 0 ; i < 4 ; i++ )
		for( j = 0 ; j < 4 ; j++ ){
			A[i][j] = DA[i][j];
		}

	for( i = 0 ; i < 4 ; i++ )
		A[i][4] = D[i];

	for( i = 0 ; i < 4 ; i++ ){
		
		max = A[i][i];
		iMax = i;
		for( k = i+1 ; k < 4 ; k++ )
			if( A[i][k] > max ){
				iMax = k;
				max = A[i][k];
			}
	
		if( i != iMax ){
			vector_swap( A[i], A[iMax], 4+1 );
		}

		a = 1.0 / A[i][i];
		vector_multiply( A[i], a, 4+1 );

		for( j = 0 ; j < 4 ; j++ ){
			if( j != i ){
				a = -A[j][i];
				vector_acumalute( A[j], A[i], a, 4+1 );
			}
		}
	}

	for( i = 0 ; i < 4 ; i++ )
		X[i] = A[i][4];

	return( 1 );
}
*/