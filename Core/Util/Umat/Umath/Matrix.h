/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _MATRIX_TYPE_
#define _MATRIX_TYPE_


#ifdef __cplusplus
extern "C" {
#endif

#define MT_FLOAT	0
#define MT_DOUBLE	1


#define MATRIX_ELM( M, i, j )	(*(M->d + i*(M)->n + j ))	

#define MATRIX_ELM_P( M, i, j )	(M->d + i*(M)->n + j )	

#define MATRIX_ROW( M, i )	( M->d + i*(M)->n )	


typedef struct matrix_type {
	union {
		int	ni;
		int	m;	// row
	};
	union {
		int	nj;
		int	n;	// column
	};
	int	type;	// MT_FLOAT, MT_DOUBLE

	union {
		double *d;
		float	*f;
	};

} matrix_type;


matrix_type *	matrix_alloc( int m, int n, int type );

matrix_type *	matrix_realloc( matrix_type *mt, int m, int n, int type );


void	matrix_destroy( matrix_type *m );

void	matrix_zero( matrix_type *m );

void	matrix_unity( matrix_type *m );

matrix_type	*matrix_copy( matrix_type *sm, matrix_type *m );


void	matrix_centering( matrix_type *m );

int		matrix_multiply( matrix_type *A, matrix_type *B, matrix_type *C );

int		matrixD_multV( matrix_type *A, double *B, double *C );


void	matrix_multiply_scalar( matrix_type *A, double s, matrix_type *B );

void	matrixD_column( matrix_type *m, int j, double f[] );


int		matrix_read( matrix_type **A, char *file );

int		matrix_write( matrix_type *A, char *file );

int		matrix_print( char *title, matrix_type *A, FILE *fp );


void	matrix_diagonal( matrix_type *A, double *D, int p );

void	matrixF_const( matrix_type *m, float val );

void	matrixD_const( matrix_type *m, float val );




	// MatrixTool.c
void	matrixF_print( char *title, float *a, int m, int n, FILE *fp );

void	matrixD_print( char *title, double *a, int m, int n, FILE *fp );


void	matrixF_multiply( float *A, int m, int n, float *B, int p, float C[] );

void	matrixD_multiply( double *A, int m, int n, double *B, int p, double C[] );


void	matrixF_diagonal( float *A, int m, int n, float *D, int p );

void	matrixD_diagonal( double *A, int m, int n, double *D, int p );

void	matrix_centering( matrix_type *m );

int		matrix_inverse( matrix_type *A, matrix_type *B );

void	matrix_transpose( matrix_type *A, matrix_type *AT );

matrix_type *	matrix_crop( matrix_type *A, int m, int n, matrix_type *mt );

void	matrixD_symmetric( matrix_type *mt );



	// MatrixSvd.c
int		matrix_svd( matrix_type *M, matrix_type **S, matrix_type **U, matrix_type **V );

void	matrix_svd_test( matrix_type *M, matrix_type *S, matrix_type *U, matrix_type *V  );


int		matrixD_svd( double *M, int m, int n, double *S, double *U, double *V  );

void	matrixD_svd_test( double *M, int m, int n, double *S, double *U, double *V  );


int		matrixF_svd( float *M, int m, int n, float *S, float *U, float *V  );

void	matrixF_svd_test( float *M, int m, int n, float *S, float *U, float *V  );



	// MatrixMsd.c
int	matrix_mds( matrix_type *D, int rank, matrix_type **P );


matrix_type *	matrix_mds_P2D( matrix_type *P );


	// MatrixNmsd.c
void	matrix_nmdsT( matrix_type *L, int rank, float T, matrix_type **rP );

void	matrix_nmds( matrix_type *L, int rank, matrix_type **rP );



int	matrixSD_eigen( matrix_type *m, matrix_type *em, double ev[] );

#ifdef __cplusplus
}
#endif

#endif