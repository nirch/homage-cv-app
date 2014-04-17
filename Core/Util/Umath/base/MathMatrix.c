/********************************
 ***   MathLinerExquation9.c   ***
 ********************************/

#include	"../umath.h"

#include	"Uigp/igp.h"





void
math_matrix_multiply_vector( float *A, int m, int n, float	B[], float F[] )
{
int	i,	j;
float	*AI;

	for( i = 0, AI = A ; i < m ; i++, AI += n ){
		F[i] = 0;
		for( j = 0 ; j < n ; j++ )
			F[i] += AI[j]*B[j];
	}
}




void
math_matrix_print_file( char *file, float *a, int m, int n )
{
FILE *fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return;

	math_matrix_print( fp, a, m, n );

	fclose( fp );
}


void
math_matrix_print( FILE *fp, float *A, int M, int N )
{
float	*p;
int	i;

	for( i = 0, p = A ; i < M ; i++, p += N ){
		vector_print( fp, p, N );
	}
}


//void
//vector_print( FILE *fp, float *A, int n )
//{
//float	*p;
//int	j;
//
//	for( j = 0, p = A ; j < n ; j++, p++ )
//		fprintf( fp, "%6.2f   ", *p );
//
//	fprintf( fp, "\n" );
//
//}



void
math_matrix_print_mathlab_file( char *file, float *a, int m, int n )
{
FILE *fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return;

	math_matrix_print_mathlab( fp, a, m, n );

	fclose( fp );
}


void
math_matrix_print_mathlab( FILE *fp, float *a, int m, int n )
{
int	i,	j;
float	*p;

	fprintf( fp, "[\n" );

	for( i = 0, p = a ; i < m ; i++ ){
		for( j = 0 ; j < n ; j++, p++ )
			fprintf( fp, "  %.2f", *p );

		fprintf( fp, "  ;\n" );
	}
	fprintf( fp, "]\n" );
}