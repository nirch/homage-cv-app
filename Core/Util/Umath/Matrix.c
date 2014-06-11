#include	<string.h>
#include	<stdio.h>
#include	<math.h>

#include "umath.h"


#include	"Uigp/igp.h"
#include	"Matrix.h"


matrix_type *	
matrix_alloc( int m, int n, int type )
{
matrix_type	*mt;

	if( type != MT_FLOAT && type != MT_DOUBLE )
		return( NULL );

	mt = ( matrix_type *)malloc( sizeof(matrix_type));
	mt->n = n;
	mt->m = m;
	mt->type = type;

	if( type == MT_FLOAT )
		mt->f = ( float *)malloc( mt->m*mt->n*sizeof(float));
	else
		mt->d = ( double *)malloc( mt->m*mt->n*sizeof(double));

	
	return( mt );
}

matrix_type *	
matrix_realloc( matrix_type *mt, int m, int n, int type )
{
	if( mt == NULL ){
		mt = matrix_alloc( m, n, type );
		return( mt );
	}

	if( mt->m == m && mt->n == n && mt->type == type )
		return( mt );



	if( type != MT_FLOAT && type != MT_DOUBLE )
		return( NULL );

	mt->n = n;
	mt->m = m;
	mt->type = type;

	free( mt->f );



	if( type == MT_FLOAT )
		mt->f = ( float *)malloc( mt->m*mt->n*sizeof(float));
	else
		mt->d = ( double *)malloc( mt->m*mt->n*sizeof(double));


	return( mt );
}

void	
matrix_destroy( matrix_type *m )
{
	if( m == NULL )
		return;

	free( m->d );

	free( m );
}


matrix_type	*
matrix_copy( matrix_type *sm, matrix_type *m )
{
int	i;
	m = matrix_realloc( m, sm->m, sm->n, sm->type );

	if( sm->type == MT_FLOAT ){
		for( i = 0 ; i < sm->m*sm->n ; i++ )
			m->f[i] = sm->f[i];
		return( m );
	}


	for( i = 0 ; i < sm->m*sm->n ; i++ )
		m->d[i] = sm->d[i];


	return( m );
}



// A mxn   B nXp     C mxp
void
matrixF_multiply( float *A, int m, int n, float *B, int p, float C[] )
{
	int	i,	j,	k;
	float	*cp;

	for( i = 0, cp = C ; i < m ; i++ ){
		for( j = 0 ; j < p ; j++, cp++ ){

			*cp = 0;
			for( k = 0 ; k < n ; k++ ){
				*cp += A[i*n + k] * B[ k*p + j ]; 
			}

		}
	}
}

// A mxn   B nXp     C mxp
void
matrixD_multiply( double *A, int m, int n, double *B, int p, double C[] )
{
int	i,	j,	k;
double	*cp;

	for( i = 0, cp = C ; i < m ; i++ ){
		for( j = 0 ; j < p ; j++, cp++ ){

			*cp = 0;
			for( k = 0 ; k < n ; k++ ){
				*cp += A[i*n + k] * B[ k*p + j ]; 
			}

		}
	}
}

void
matrixF_diagonal( float *A, int m, int n, float *D, int p )
{
	int	i,	j;
	float	*tp;

	for( i = 0, tp = A ; i < m ; i++ )
		for( j = 0 ; j < n ; j++ )
			*tp++ = 0;

	for( i = 0 ; i < p ; i++ )
		A[i*n+i] = D[i];
}

void
matrixD_diagonal( double *A, int m, int n, double *D, int p )
{
int	i,	j;
double	*tp;

	for( i = 0, tp = A ; i < m ; i++ )
		for( j = 0 ; j < n ; j++ )
			*tp++ = 0;

	for( i = 0 ; i < p ; i++ )
		A[i*n+i] = D[i];
}



void
matrixF_print( char *title, float *a, int m, int n, FILE *fp )
{
float	*ap;
int	i,	j;

	fprintf( fp, "%s\n", title );

	ap = a;
	for( i = 0 ; i < m ; i++ ){
		for( j = 0; j < n ; j++ ){
			fprintf( fp, "%f     ", *ap++ );
		}

		fprintf( fp, "\n" );
	}
}



void
matrixD_print( char *title, double *a, int m, int n, FILE *fp )
{
double	*ap;
int	i,	j;

	fprintf( fp, "%s\n", title );

	ap = a;
	for( i = 0 ; i < m ; i++ ){
		for( j = 0; j < n ; j++ ){
			fprintf( fp, "%lf     ", *ap++ );
		}

		fprintf( fp, "\n" );
	}
}

void
matrixF_const( matrix_type *m, float val )
{
	float	*mp;
	int	i,	j;

	mp = m->f;
	for( i = 0 ; i < m->m ; i++ ){
		for( j = 0; j < m->n ; j++ )
			*mp++ = val;
	}
}

void
matrixD_const( matrix_type *m, float val )
{
	double	*mp;
	int	i,	j;

	mp = m->d;
	for( i = 0 ; i < m->m ; i++ ){
		for( j = 0; j < m->n ; j++ )
			*mp++ = val;
	}
}

void
matrix_zero( matrix_type *m )
{
	double	*mp;
	int	i,	j;

	mp = m->d;
	for( i = 0 ; i < m->m ; i++ ){
		for( j = 0; j < m->n ; j++ )
			*mp++ = 0;
	}
}

void
matrix_unity( matrix_type *m )
{
double	*mp;
int	i,	j;

	mp = m->d;
	for( i = 0 ; i < m->m ; i++ ){
		for( j = 0; j < m->n ; j++ )
			*mp++ = ( i==j)? 1.0 : -0;
	}
}


void
matrix_centering( matrix_type *m )
{
double	*mp,	tmp;
int	i,	j;

	
	tmp = 1.0 / m->n;

	mp = m->d;
	for( i = 0 ; i < m->m ; i++ ){
		for( j = 0; j < m->n ; j++ )
			*mp++ = ( i==j)? 1.0 - tmp : -tmp;
	}
}


void
matrix_diagonal( matrix_type *A, double *D, int p )
{
int	i;


	matrix_zero( A );

	if( p > A->m )
		p = A->m;

	if( p > A->n )
		p = A->n;


	for( i = 0 ; i < p && i < A->m ; i++ )
		A->d[i*A->n+i] = D[i];
}


// A mxn   B nXp     C mxp
matrix_type *
matrix_multiply( matrix_type *A, matrix_type *B, matrix_type *C )
{
int	i,	j,	k;
double	*cp;

	if( A->n != B->m )
		return(NULL );


	if( C == NULL )
		C = matrix_alloc( A->ni, B->nj, A->type );


	if( C->m != A->m || C->n != B->n )
		return( NULL );



	for( i = 0, cp = C->d ; i < C->m ; i++ ){
		for( j = 0 ; j < C->n ; j++, cp++ ){

			*cp = 0;
			for( k = 0 ; k < A->n ; k++ ){
				*cp += A->d[i*A->n + k] * B->d[ k*B->n + j ]; 
			}

		}
	}

	return( C );
}


// A mxn   B nX1     C mx1
int
matrixD_multV( matrix_type *A, double *B, double *C )
{
int	i,	j;


	for( i = 0 ; i < A->m ; i++ ){

		C[i] = 0;
		for( j = 0 ; j < A->n ; j++ ){
			C[i] += A->d[i*A->n + j] * B[ j ]; 
		}
	}

	return( 1 );
}

int
matrixD_multVF( matrix_type *A, float *B, float *C )
{
	int	i,	j;


	for( i = 0 ; i < A->m ; i++ ){

		C[i] = 0;
		for( j = 0 ; j < A->n ; j++ ){
			C[i] += A->d[i*A->n + j] * B[ j ]; 
		}
	}

	return( 1 );
}


void
matrixD_get_column( matrix_type *m, int j, double f[] )
{
	int	i;

	for( i = 0 ; i < m->m ; i++ )
		f[i] = *MATRIX_ELM_P( m, i, j );
}

void
matrixD_set_column( matrix_type *m, int j, double f[] )
{
	int	i;

	for( i = 0 ; i < m->m ; i++ )
		*MATRIX_ELM_P( m, i, j ) = f[i];
}

void
matrixD_set_columnF( matrix_type *m, int j, float f[] )
{
	int	i;

	for( i = 0 ; i < m->m ; i++ )
		*MATRIX_ELM_P( m, i, j ) = f[i];
}


matrix_type *	
matrix_transpose( matrix_type *m, matrix_type *mt )
{
int	i,	j;
double	*tp;

	if( mt == NULL )
		mt = matrix_alloc( m->nj, m->ni, m->type );

	tp = mt->d;
	for( i = 0 ; i < mt->ni ; i++ ){
		for( j = 0 ; j < mt->nj ; j++ ){
			*tp++ = m->d[j*m->nj + i];
		}
	}

	return( mt );
}


// A mxn       B mxn
void
matrix_multiply_scalar( matrix_type *A, double s, matrix_type *B )
{
int	i,	j;
double	*sp,	*tp;


	sp = A->d;
	tp = B->d;
	for( i = 0 ; i < A->m ; i++ ){
		for( j = 0 ; j < A->n ; j++ ){

			*tp++ = s * *sp++;

		}
	}
}


// A nxn   B nXn
int
matrix_inverse( matrix_type *A, matrix_type *B )
{
int	ret;

	ret = math_matrixD_inverse( A->d, B->d, A->n );
	return( ret );
}

int
matrix_solve( matrix_type *m, double D[], double X[] )
{
	int	ret;

	matrix_type *im = matrix_alloc( m->n, m->n, MT_DOUBLE );
	
	ret = matrix_inverse( m, im );

	matrixD_multV( im, D, X );

	matrix_destroy( im );

	return( ret );
}



// A mxn 
matrix_type *
matrix_crop( matrix_type *A, int m, int n, matrix_type *mt )
{
double	*sp,	*tp;
int	i,	j;



	mt = matrix_realloc( mt, m, n, MT_DOUBLE );


	tp = mt->d;
	for( i = 0 ; i < mt->m ; i++ ){
		sp = MATRIX_ROW( A, i );
		for( j = 0 ; j < mt->n ; j++ ){

			*tp++ = *sp++;
		}
	}

	return( mt );
}


int
matrix_read_file( matrix_type **A, char *file )
{
FILE	*fp;
int	i,	j,	m,	n;
double	*cp;


	if( ( fp = fopen( file, "rb" )) == NULL )
		return( -1 );

	fscanf( fp, "%d %d", &m, &n );

	*A = matrix_alloc( m, n, MT_DOUBLE );


	for( i = 0, cp = (*A)->d ; i < (*A)->m ; i++ ){
		for( j = 0 ; j < (*A)->n ; j++, cp++ ){

			fscanf( fp, "%lf", cp );

		}
	}

	fclose( fp );

	return( 1 );
}


void
matrixD_symmetric( matrix_type *mt )
{
int	i,	j;
double	*f;


	for( i = 1 ; i < mt->n ; i++ ){
		f = MATRIX_ELM_P( mt, i, 0 );

		for( j = 0 ; j < i ; j++, f++ ){
			*f = *MATRIX_ELM_P( mt, j, i );
		}
	}
}


int
matrix_write_file( matrix_type *m, char *file )
{
FILE	*fp;

	if( ( fp = fopen( file, "wb" )) == NULL )
		return( -1 );

	matrix_write( m, fp );

	fclose( fp );

	return( 1 );
}


int
matrix_write( matrix_type *A, FILE *fp )
{
int	i,	j;
double	*cp;


	fprintf( fp, "%d %d\n", A->m, A->n );


	for( i = 0, cp = (A)->d ; i < (A)->m ; i++ ){
		for( j = 0 ; j < (A)->n ; j++, cp++ ){
			fprintf( fp, "%lf    ", *cp );
		}

		fprintf( fp, "\n" );
	}


	return( 1 );
}


int
matrix_print( char *title, matrix_type *A, FILE *fp )
{
	int	i,	j;
	double	*cp;


	if( title != NULL )
		fprintf( fp, "%s\n", title );


	fprintf( fp, "%d %d\n", A->m, A->n );


	for( i = 0, cp = (A)->d ; i < (A)->m ; i++ ){
		for( j = 0 ; j < (A)->n ; j++, cp++ ){
			fprintf( fp, "%lf    ", *cp );
		}

		fprintf( fp, "\n" );
	}


	return( 1 );
}