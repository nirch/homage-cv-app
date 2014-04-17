/*************************
 ***   MathVectorD.c   ***
 *************************/

#include	"umath.h"

#include	"Uigp/igp.h"


float
vectorD_inner( double a[], double b[], int n )
{
int	i;
double	sum;

	sum = 0;
	for( i = 0 ; i < n ; i++ ){

		sum += a[i] * b[i];
	}

	return( sum );
}


void
vectorD_swap( double a[], double b[], int n )
{
int	i;
double	tmp;

	for( i = 0 ; i < n ; i++ ){
		tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
	}
}



void
vectorD_multiply( double a[], double t, int n )
{
int	i;

	for( i = 0 ; i < n ; i++ )
		a[i] *= t;
}

void
vectorD_acumalute( double a[], double b[], double t, int n )
{
int	i;

	for( i = 0 ; i < n ; i++ )
		a[i] += t * b[i];
}



/*
void
math_matrixD_multiply_vector( double *A, int m, int n, double B[], double F[] )
{
int	i,	j;
double	*AI;

	for( i = 0, AI = A ; i < m ; i++, AI += n ){
		F[i] = 0;
		for( j = 0 ; j < n ; j++ )
			F[i] += AI[j]*B[j];
	}
}




void
math_matrixD_print_file( char *file, double *a, int m, int n )
{
FILE *fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return;

	math_matrixD_print( fp, a, m, n );

	fclose( fp );
}


void
math_matrixD_print( FILE *fp, double *A, int M, int N )
{
double	*p;
int	i;

	for( i = 0, p = A ; i < M ; i++, p += N ){
		vectorD_print( fp, p, N );
	}
}


void
vectorD_print( FILE *fp, double *A, int n )
{
double	*p;
int	j;

	for( j = 0, p = A ; j < n ; j++, p++ )
		fprintf( fp, "%6.2f   ", (float)*p );

	fprintf( fp, "\n" );

}
*/
