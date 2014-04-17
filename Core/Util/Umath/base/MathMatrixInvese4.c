/********************************
 ***   MathLinerExquation.c   ***
 ********************************/

#include	"../umath.h"

#include	"Uigp/igp.h"





int
math_matrix_inverse4( float DA[4][4], float B[4][4] )
{
float	A[4][4];
double	a;
int	i,	j,	k,	iMax;
float	max;

	for( i = 0 ; i < 4 ; i++ )
		for( j = 0 ; j < 4 ; j++ ){
			B[i][j] = ( i != j )? 0 : 1;
			A[i][j] = DA[i][j];
		}


	for( i = 0 ; i < 4 ; i++ ){
		
		max = A[i][i];
		iMax = i;

		for( k = i+1 ; k < 4 ; k++ )
			if( ABS(A[k][i]) > ABS(max) ){
				iMax = k;
				max = A[k][i];
			}
		if( i != iMax ){
			vector_swap( A[i], A[iMax], 4 );
			vector_swap( B[i], B[iMax], 4 );
		}

		a = 1.0 / A[i][i];
		vector_multiply( A[i], a, 4 );
		vector_multiply( B[i], a, 4 );

		for( j = 0 ; j < 4 ; j++ ){
			if( j != i ){
				a = -A[j][i];
				vector_acumalute( A[j], A[i], a, 4 );
				vector_acumalute( B[j], B[i], a, 4 ); 
			}
		}
	}
			
	return( 1 );
}


int
math_matrixD_inverse4( double DA[4][4], double B[4][4] )
{
double	A[4][4];
double	a;
int	i,	j,	k,	iMax;
double	max;

	for( i = 0 ; i < 4 ; i++ )
		for( j = 0 ; j < 4 ; j++ ){
			B[i][j] = ( i != j )? 0 : 1;
			A[i][j] = DA[i][j];
		}


	for( i = 0 ; i < 4 ; i++ ){
		
		max = A[i][i];
		iMax = i;

		for( k = i+1 ; k < 4 ; k++ )
			if( ABS(A[k][i]) > ABS(max) ){
				iMax = k;
				max = A[k][i];
			}
		if( i != iMax ){
			vectorD_swap( A[i], A[iMax], 4 );
			vectorD_swap( B[i], B[iMax], 4 );
		}

		a = 1.0 / A[i][i];
		vectorD_multiply( A[i], a, 4 );
		vectorD_multiply( B[i], a, 4 );

		for( j = 0 ; j < 4 ; j++ ){
			if( j != i ){
				a = -A[j][i];
				vectorD_acumalute( A[j], A[i], a, 4 );
				vectorD_acumalute( B[j], B[i], a, 4 ); 
			}
		}
	}
			
	return( 1 );
}


int
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


int
math_linear_equation4D( double DA[4][4], double D[4], double X[4] )
{
	double	A[4][4+1];
	double	a;
	int	i,	j,	k,	iMax;
	double	max;

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
					vectorD_swap( A[i], A[iMax], 4+1 );
				}

				a = 1.0 / A[i][i];
				vectorD_multiply( A[i], a, 4+1 );

				for( j = 0 ; j < 4 ; j++ ){
					if( j != i ){
						a = -A[j][i];
						vectorD_acumalute( A[j], A[i], a, 4+1 );
					}
				}
		}

		for( i = 0 ; i < 4 ; i++ )
			X[i] = A[i][4];

		return( 1 );
}