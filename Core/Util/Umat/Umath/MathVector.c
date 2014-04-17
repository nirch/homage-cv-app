/************************
 ***   MathVector.c   ***
 ************************/
#include	 <math.h>
#include	"umath.h"

#include	"Uigp/igp.h"


float
vector_inner( float a[], float b[], int n )
{
int	i;
float	sum;

	sum = 0;
	for( i = 0 ; i < n ; i++ ){

		sum += a[i] * b[i];
	}

	return( sum );
}


float
vector_inner_w( float a[], float b[], float w[], int n )
{
	int	i;
	float	sum;

	sum = 0;
	for( i = 0 ; i < n ; i++ ){

		sum += a[i] * b[i] * w[i];
	}

	return( sum );
}



float
vector_sum( float a[], int n )
{
int	i;
float	sum;

	sum = 0;
	for( i = 0 ; i < n ; i++ ){
		sum += a[i];
	}

	return( sum );
}



void
vector_swap( float a[], float b[], int n )
{
int	i;
float	tmp;

	for( i = 0 ; i < n ; i++ ){
		tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
	}
}



void
vector_multiply( float a[], double t, int n )
{
int	i;

	for( i = 0 ; i < n ; i++ )
		a[i] *= t;
}

void
vector_acumalute( float a[], float b[], double t, int n )
{
int	i;

	for( i = 0 ; i < n ; i++ )
		a[i] += t * b[i];
}



void
vector_print( FILE *fp, float *A, int n )
{
	float	*p;
	int	j;

	for( j = 0, p = A ; j < n ; j++, p++ )
		fprintf( fp, "%6.2f   ", *p );

	fprintf( fp, "\n" );

}




void
vector_weight_linear( float w[], int n, float a0, float a1 )
{
int	i;
float	x,	sum;


	sum = 0;
	for( i = 0 ; i < n ; i++ ){
		x = (float)( -n +1 + 2*i )/n;
		if( x < 0 )	x = -x;

		w[i] = a0 + x * ( a1- a0 );

		sum += w[i];
	}


	for( i = 0; i < n ; i++ )
		w[i] /= sum;

}



void
vector_weight_121( float w[], int n )
{
int	i,	k;
float	sum;



	for( k = 1 ; k <= n ; k++ ){
		w[k-1] = 0;
		for( i = k-1 ; i > 0 ; i-- ){
			w[i] = w[i]	 + w[i-1];
		}
		w[0] = 1;

//		vector_print( stdout, w, k );
	}

//	vector_print( stdout, w, n );
	for( i = 0, sum = 0 ; i < n ; i++ )
		sum += w[i];


	for( i = 0; i < n ; i++ )
		w[i] /= sum;

//	vector_print( stdout, w, n );
}



void
vector_weight_guasian( float w[], int n, float scale )
{
float	x,	t,	sum;
int	i;

	sum = 0;
	for( i = 0 ; i < n ; i++ ){
		x = (float)( -n +1 + 2*i )/n;
		if( x < 0 )	x = -x;

		x *=  scale;
		t = -0.5*x*x;

		w[i] = exp( t );

		sum += w[i];
	}


	for( i = 0 ; i < 2*n+1 ; i++ )
		w[i] /= sum;
}

