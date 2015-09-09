/********************************
 ***   MathLinerExquation9.c   ***
 ********************************/

#include	"umath.h"

#include	"Uigp/igp.h"

#define	NW	9

float	math_matrix9_diagonal( float A[NW][NW] );



static void	build_result( float A[9][9], int i0, float B[9] );

static int	find_zero( float B[9][9], int i0 );



static void	math_matric9_mult( float A[9][9], float	B[9], float F[9] );


int
math_linear_equation9( float DA[NW][NW], float F[NW][NW] )
{
float	A[NW][NW]; 
int	i,	j,	i0,	n;

	for( i = 0 ; i < NW ; i++ )
		for( j = 0 ; j < NW ; j++ ){
			A[i][j] = DA[i][j];
		}


	math_matrix9_diagonal( A );


	n = 0;
	i0 = 0;

	while( (i0 = find_zero( A, i0 ) ) >= 0 ){
		build_result( A, i0, F[n] );

//		math_linear_equation9_test( DA, F[n] );

		i0++;
		n++;
	}


	return( n );
}




float
math_matrix9_diagonal( float A[NW][NW] )
{
double	a;
int	j,	k,	iMax,	i0;
float	max;



	i0 = 0;
	for( j = 0 ; j < NW ; j++ ){
		
		max = A[i0][j];
		iMax = i0;
		for( k = i0+1 ; k < NW ; k++ )
			if( ABS(A[k][j]) > ABS(max) ){
				iMax = k;
				max = A[k][j];
			}

			

//		if( ABS(A[j][j]) < 0.00001 ){
		if( ABS(max) < 0.00000001 ){
			for( k = i0 ; k < NW ; k++ )
				A[k][j] = 0;
			continue;
		}



		if( iMax != j ) // i0 != iMax )
			vector_swap( A[j], A[iMax], NW );



		a = 1.0 / A[j][j];
		vector_multiply( A[j], a, NW );

		for( k = 0 ; k < NW ; k++ ){
			if( k != j ){
				a = -A[k][j];
				vector_acumalute( A[k], A[j], a, NW );
			}
		}

		i0++;
	}



	math_matrix_print( stdout, A[0], NW, NW );
			
	return( 1 );
}



static void
build_result( float A[9][9], int i0, float	B[9] )
{
int	i,	j;

	B[i0] = 1;
	for( j = i0+1 ; j < NW ; j++ )
		B[j] = 0;

	for( i = i0 - 1 ; i >= 0 ; i-- ){
		if( A[i][i] == 0 ){
			B[i] = 0;
			continue;
		}

		B[i] = 0;
		for( j = i+1 ; j < NW ; j++ )
			B[i] += -A[i][j]* B[j];
	}
}


static int
find_zero( float B[9][9], int i0 )
{
int	i;

	for( i = i0 ; i < NW ; i++ )
		if( B[i][i] == 0 )	return( i );

	return( -1 );
}




int
math_linear_equation9_test( float DA[NW][NW], float F[NW] )
{
float	B[NW];

	fprintf( stdout, " F\n" );
	vector_print( stdout, F, NW );

	fprintf( stdout, " AA\n" );
	math_matric9_mult( DA, F, B );
	vector_print( stdout, B, NW );


	return( 1 );
}


static void
math_matric9_mult( float A[9][9], float	B[9], float F[9] )
{
int	i,	j;

	for( i = 0 ; i < NW ; i++ ){
		F[i] = 0;
		for( j = 0 ; j < NW ; j++ )
			F[i] += A[i][j]*B[j];
	}
}

