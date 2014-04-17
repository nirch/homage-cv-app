/******************************
 ***   MathLinerInverse.c   ***
 ******************************/

#include	"../umath.h"

#include	"Uigp/igp.h"


#define	MSIZE	4


int
math_matrix_inverse( float *aDA, float *aB, int N )
{
float	*aA;
float	**A,	**DA,	**B;
double	a;
int	i,	j,	k,	iMax;
float	max;

	A = ( float **)malloc( N*sizeof(float *));
	DA = ( float **)malloc( N*sizeof(float *));
	B = ( float **)malloc( N*sizeof(float *));

	aA = (float*)malloc( N*N*sizeof(float));

	for( i = 0 ; i < N ; i++ ){
		A[i] = &aA[i*N];
		B[i] = &aB[i*N];
		DA[i] = &aDA[i*N];
	}




	for( i = 0 ; i < N ; i++ )
		for( j = 0 ; j < N ; j++ ){
			B[i][j] = ( i != j )? 0 : 1;
			A[i][j] = DA[i][j];
		}


	for( i = 0 ; i < N ; i++ ){
		
		max = A[i][i];
		iMax = i;

		for( k = i+1 ; k < N ; k++ )
			if( ABS(A[k][i]) > ABS(max) ){
				iMax = k;
				max = A[k][i];
			}
		if( i != iMax ){
			vector_swap( A[i], A[iMax], N );
			vector_swap( B[i], B[iMax], N );
		}



		a = 1.0 / A[i][i];
		vector_multiply( A[i], a, N );
		vector_multiply( B[i], a, N );

		for( j = 0 ; j < N ; j++ ){
			if( j != i ){
				a = -A[j][i];
				vector_acumalute( A[j], A[i], a, N );
				vector_acumalute( B[j], B[i], a, N ); 
			}
		}
	}

	free( aA );
	free( A );
	free( B );
	free( DA );
			
	return( 1 );
}

int
math_matrixD_inverse( double *aDA, double *aB, int N )
{
double	*aA;
double	**A,	**DA,	**B;
double	a;
int	i,	j,	k,	iMax;
double	max;

	A = ( double **)malloc( N*sizeof(double *));
	DA = ( double **)malloc( N*sizeof(double *));
	B = ( double **)malloc( N*sizeof(double *));

	aA = (double*)malloc( N*N*sizeof(double));

	for( i = 0 ; i < N ; i++ ){
		A[i] = &aA[i*N];
		B[i] = &aB[i*N];
		DA[i] = &aDA[i*N];
	}




	for( i = 0 ; i < N ; i++ )
		for( j = 0 ; j < N ; j++ ){
			B[i][j] = ( i != j )? 0 : 1;
			A[i][j] = DA[i][j];
		}


		for( i = 0 ; i < N ; i++ ){

			max = A[i][i];
			iMax = i;

			for( k = i+1 ; k < N ; k++ )
				if( ABS(A[k][i]) > ABS(max) ){
					iMax = k;
					max = A[k][i];
				}
				if( i != iMax ){
					vectorD_swap( A[i], A[iMax], N );
					vectorD_swap( B[i], B[iMax], N );
				}

				a = 1.0 / A[i][i];
				vectorD_multiply( A[i], a, N );
				vectorD_multiply( B[i], a, N );

				for( j = 0 ; j < N ; j++ ){
					if( j != i ){
						a = -A[j][i];
						vectorD_acumalute( A[j], A[i], a, N );
						vectorD_acumalute( B[j], B[i], a, N ); 
					}
				}
		}

		free( aA );
		free( A );
		free( B );
		free( DA );

		return( 1 );
}
