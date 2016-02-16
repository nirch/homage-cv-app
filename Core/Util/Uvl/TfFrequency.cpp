
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif

#include	"Uigp/igp.h"
#include	"Utime\GpTime.h"
#include "Uvl/TfType.h"
#include "Umath/Matrix2Type.h"


int	tf_absmax( tf_type *tf, int i0 );

tf_type *	tf_fourier( tf_type *tf, tf_type *f );

static void	tf_fourier_n( tf_type *tf, int n, double *a, double *b );


int
tf_frequencyA( tf_type *tf, double *freq, double *amp )
{
double	a,	b;

	if( tf_app_axb( tf, &a, &b ) < 0 ){
		tf_frequency( tf, freq, amp );
		return( 1 );
	}

	tf_type *t = tf_axb( tf, a, b, NULL );

	

	tf_frequency( t, freq, amp );


	double a1, b1;
	tf_app_axb( t, &a1, &b1 ) ;

	tf_destroy( t );

	return( 1 );
}



tf_type *
tf_axb( tf_type *stf, double a, double b, tf_type *tf )
{
int	i;

	if( tf == NULL )
		tf = tf_alloc( stf->nA );

	for( i = 0 ; i < tf->nA ; i++ )
		tf->a[i] = stf->a[i] -( a*i + b );

	return( tf );
}

int
tf_app_axb( tf_type *tf, double *a, double *b )
{
	matrix2_type m;
	vec2d_type	D,	X;
	int	i;

	matrix2_unit( &m );
	D.x = D.y = 0;
	for( i = 0 ; i < tf->nA ; i++ ){
		m.a00 += i*i;
		m.a01 += i;
		D.x += tf->a[i]*i;

//		m.a10 += i;
		m.a11 += 1;
		D.y += tf->a[i];

	}

	m.a10 = m.a01;

	if( matrix2_solve( &m, &D, &X ) < 0 )
		return( -1 );

	*a = X.x;
	*b = X.y;

	return( 1 );

}


void
tf_frequency( tf_type *tf, double *freq, double *amp )
{
	tf_type *f = tf_fourier( tf, NULL );
	
	int i = tf_absmax( f, 1 );

	*freq = i;
	*amp = f->a[i];

	tf_destroy( f );
}


void
tf_frequency_test( double f, double a, int N )
{
	int	i;

	tf_type *tf = tf_alloc( N );
	double c = 1.0 / sqrt( (float)N );

	
	for( i = 0 ; i < tf->nA ; i++ ){

		tf->a[i] = a * cos( 0.75+ f*i*2*M_PI/N);
	}

	double freq,	amp;
	tf_frequency( tf, &freq, &amp );
}


int
tf_absmax( tf_type *tf, int i0 )
{
	int i;

	int	iMax = i0;
	for( i = i0+1 ; i < tf->nA ; i++ ){

		if( ABS(tf->a[i]) > ABS(tf->a[iMax]) )
			iMax = i;
	}

	return( iMax );
}

tf_type * 
tf_fourier( tf_type *tf, tf_type *f )
{
int	n;

	if( f == NULL )
		f = tf_alloc( tf->nA );

	for( n = 0 ; n < tf->nA ; n++ ){

		double a,	b;
		

		tf_fourier_n( tf, n, &a, &b );

		f->a[n] = hypot( a, b );
	}


	return( f );
}






static void
tf_fourier_n( tf_type *tf, int n, double *a, double *b )
{
int	i;

	int N = tf->nA;
	double c = 1.0 / sqrt( (float)N );
	c = 2.0 /N;
	
	*a = *b = 0;
	for( i = 0 ; i < tf->nA ; i++ ){

		double x = n*(2*i)*M_PI / N;

		*a += tf->a[i]*cos(x);
		*b += tf->a[i]*sin(x);
	}

	*a *= c;
	*b *= c;
}