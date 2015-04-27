#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Uvec/Vec2d.h"


#include "Matrix.h"
#include "umath.h"

#include	"levmar\levmar\lm.h"


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "levmarD.lib" )
#pragma comment( lib, "clapackD.lib" )
#else
#pragma comment( lib, "levmar.lib" )
#pragma comment( lib, "clapack.lib" )
#endif




typedef struct nmds_type {
	double	l;
	
	int	i,	j;

	double	d;
	double	sd;		// accumulate of d

	double	p;

} nmds_type; 






typedef struct levmarData_type {
	matrix_type	*P;


	int	nA;

	nmds_type *A;


	int	n;

} levmarData_type;


static void nmds_fitting_levmar( double *paramter, double *x, int nParamter, int nMeasure, void *data );

void	nmdsAux_init( matrix_type *L, nmds_type **A, int *nA );

void	nmdsAux_set_distance( nmds_type *A, int nA, matrix_type *P );

void	nmdsAux_set_d( nmds_type *A, int nA, matrix_type *P );

static int	nmdsAux_convex( nmds_type *A, int nA, int i0, vec2d *p0 );

void	nmdsAux_disparity( nmds_type *A, int nA );

static double	nmdsAux_stress1( nmds_type *A, int nA );

static double	nmdsAux_measure( matrix_type *P, nmds_type *A, int nA );



void
matrix_nmdsT( matrix_type *L, int rank, float T, matrix_type **rP )
{
matrix_type	*m;
int	i;

	m = matrix_copy( L, NULL );


	for( i = 0 ; i < m->m*m->n ; i++ )
		if( m->d[i] > T )	m->d[i] = -1;


	matrix_nmds( m, rank, rP );

	matrix_destroy( m );
}


void
matrix_nmds( matrix_type *L, int rank, matrix_type **rP )
{
double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
int	nParamter,	nMeasure;
int	ret;
matrix_type	*P;
nmds_type	*A;
int	nA;

levmarData_type	data;




//	*rP = P = matrix_alloc( L->m, rank, MT_DOUBLE );

//	matrix_print( "L", L, stdout );
	matrix_mds( L, rank, &P );
//	matrix_print( "P", P, stdout );

//	MATRIX_ELM( P, 0, 0 ) = MATRIX_ELM( P, 0, 0 ) + 5;
//	matrix_print( "P1", P, stdout );

	nmdsAux_init( L , &A, &nA );

	data.P = P;
	data.A = A;
	data.nA = nA;
	data.n = 0;


	float s = nmdsAux_measure( P, A, nA );
	fprintf( stdout, "s: %f\n", s );


//	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;

//	opts[0]=LM_INIT_MU; opts[1]=1E-10; opts[2]=1E-10; opts[3]=1E-10;
	if( L->m > 70 ){
		opts[0]=LM_INIT_MU; opts[1]=1E-6; opts[2]=1E-6; opts[3]=1E-6;
	}
	else	{
		opts[0]=LM_INIT_MU; opts[1]=1E-8; opts[2]=1E-8; opts[3]=1E-8;
	}


	opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference jacobian version is u

	nParamter = P->m*P->n;
	double *paramter = (double *)malloc( nParamter * sizeof(double));
	memcpy( (void*)paramter, (void*)P->d, sizeof(double)*nParamter );


	nMeasure = nA;
	double *measurement = (double*)malloc( nMeasure*sizeof(double));
	for( int i = 0; i < nMeasure ; i++ )
		measurement[i] = 0;


	ret = dlevmar_dif( nmds_fitting_levmar, paramter, measurement, nParamter, nMeasure, 100, opts, info, NULL, NULL, &data );

	memcpy( (void*)P->d, (void*)paramter, sizeof(double)*nParamter );


	s = nmdsAux_measure( P, A, nA );
	fprintf( stdout, "s: %f\n", s );

	*rP = P;


//	GPLOG( (" Measure: %d", nMeasure ) );
}



static void 
nmds_fitting_levmar( double *paramter, double *x, int nParamter, int nMeasure, void *data )
{
levmarData_type *ld = ( levmarData_type *)data;
nmds_type	*a;
int	i;
double	sum,	t;
static int n = 0;

	memcpy( (void*)ld->P->d, (void*)paramter, sizeof(double)*nParamter );

//matrix_print( "P2", ld->P, stdout );


	nmdsAux_set_distance( ld->A, ld->nA, ld->P );

	nmdsAux_disparity( ld->A, ld->nA );

	float s = nmdsAux_measure( ld->P, ld->A, ld->nA );
	fprintf( stdout, "%d   s: %f\n", n++, s );


	for( i = 0, sum = 0 ; i < ld->nA ; i++ ){
		a = &ld->A[i];
		sum += a->d * a->d;
	}

	for( i = 0 ; i < ld->nA ; i++ ){
		a = &ld->A[i];

		t = a->d - a->p;

//		t = a->l - a->d;
		x[i] = t*t / sum;
	}


	ld->n ++;
}



//int
//matrix_nmds( matrix_type *L, int rank, double T, matrix_type *P )
//{
//nmds_type	*A;
//int	nA;
//
//
//	matrix_mds( L, rank, &P );
////	matrix_print( "P", P, stdout );
//
//
////	D = matrix_mds_P2D( P );
////	matrix_print( "DD", D, stdout );
//
//
//
//	nmdsAux_init(L, T, &A, &nA );
//
//	nmdsAux_set_d( A, nA, P );
//
//	nmdsAux_disparity( A, nA );
//
//
//	return( 1 );
//}



static double
nmdsAux_measure( matrix_type *P, nmds_type *A, int nA )
{
double	s;
	nmdsAux_set_distance( A, nA, P );

	nmdsAux_disparity( A, nA );

	s = nmdsAux_stress1( A, nA );

	return( s );
}


void
nmdsAux_init( matrix_type *L, nmds_type **A, int *nA )
{
nmds_type	*a;
double	*d;
int	i,	j,	k,	n;

	*A = a = (nmds_type *)malloc( L->m *L->n*sizeof(nmds_type));

//	d = L->d;
	for( i = 0, n = 0 ; i < L->m ; i++ ){
		d = MATRIX_ELM_P( L, i, i+1 );
		for( j = i+1 ; j < L->n ; j++, d++ ){
			if(  *d < 0  )	continue;


			for( k = n ; k > 0 ; k-- ){
				if( a[k-1].l < *d )
					break;

				a[k] = a[k-1];
			}

			a[k].l = *d;
			a[k].i = i;
			a[k].j = j;
			n++;
		}
	}

	*nA = n;
}



void
nmdsAux_set_distance( nmds_type *A, int nA, matrix_type *P )
{
matrix_type	*D;
//nmds_type	*a;
//int	i;

	D = matrix_mds_P2D( P );


	//for( i = 0; i < nA ; i++ ){
	//	a = &A[i];

	//	a->d =  MATRIX_ELM( D, a->i, a->j );
	//}

	nmdsAux_set_d( A, nA, D );

	matrix_destroy( D );
}


void
nmdsAux_set_d( nmds_type *A, int nA, matrix_type *D )
{
nmds_type	*a;
int	i;

	for( i = 0; i < nA ; i++ ){
		a = &A[i];

		a->d =  MATRIX_ELM( D, a->i, a->j );
	}
}


void
nmdsAux_disparity( nmds_type *A, int nA )
{
nmds_type	*a;
int	i,	j,	k;
double	sum,	disparity;//,	sd;
vec2d	v;

	for( i = 0, sum = 0 ; i < nA ; i++ ){
		a = &A[i];

		sum += a->d;

		a->sd = sum;
	}


	v.x = 0;
	v.y = 0;


	for( i = 0 ; i < nA ; ){
		k = nmdsAux_convex( A, nA, i, &v );
		disparity = (A[k].sd - v.y )/( k - i+1 );
		for( j = i ; j <= k ; j++ )
			A[j].p = disparity;// v.y + (A[k].sd - v.y )*(j-i+1)/( k - i+1 );

		v.x = k+1;
		v.y = A[k].sd;
		i = k+1;
	}
}


static int
nmdsAux_convex( nmds_type *A, int nA, int i0, vec2d *p0 )
{
vec2d	v,	u;
float	t;
int	i,	k;
	
	for( i = nA-1 ; i > i0 ;){

		v.x = i+1 - p0->x;
		v.y = A[i].sd - p0->y;

		u.x = -v.y;
		u.y = v.x;

		for( k = i-1 ; k >=i0 ; k-- ){
			v.x = k+1 - p0->x;
			v.y = A[k].sd - p0->y;

			t = u.x * v.x +u.y*v.y;
			if( t < 0 )	break;
		}

		if( k < i0 )
			return( i);

		i = k;
	}

	return( i0 );
}




static double
nmdsAux_stress1( nmds_type *A, int nA )
{
nmds_type	*a;
int	i;
double	s0,	s1,	t;

	s0 = s1 = 0 ;
	for( i = 0 ; i < nA ; i++ ){
		a = &A[i];

		t = a->d - a->p;
		s0 += t*t;
		s1 += a->d * a->d;
	}


	t = sqrt( s0 / s1 );

	return( t );
}


void
nmds_test()
{
nmds_type	*A;
int	nA;
matrix_type	*L,	*D;

	matrix_read( &L, "2d.txt");
	matrix_print( "L", L, stdout );

	nmdsAux_init( L, &A, &nA );

	matrix_read( &D, "2d-1.txt");
	matrix_print( "D", L, stdout );


	nmdsAux_set_d( A, nA, D );


	nmdsAux_disparity( A, nA );


}