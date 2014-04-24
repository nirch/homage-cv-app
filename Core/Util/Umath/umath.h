/*******************
 ***   umath.h   ***
 *******************/
#ifndef	_UMATH_
#define _UMATH_

#include	<stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#include	"Matrix3Type.h"




int	math_linear_equation2( float A[2][2], float D[2], float minDet, float X[2] );

int	math_linearD_equation2( double A[2][2], double D[2], double minDet, double X[2] );

// A[0][1] = A[1][0]
int	math_linear_equation2_symtric( float A[2][2], float D[2], float EigThresh, float X[2] );

int	math_linearD_equation2_symtric( double A[2][2], double D[2], double EigThresh, double X[2] );


	// MathLinerExquation.c
int	math_matrix_inverse3( float DA[3][3], float B[3][3] );

int	math_matrixD_inverse3( double DA[3][3], double B[3][3] );

float	math_m33_det( float A[3][3] );

int	math_linear_equation3( float A[3][3], float D[3], float minDet, float *det, float X[3] );


	// MathMatrixInverse6.c
float	math_matrix_inverse6( float A[6][6], float B[6][6] );

void	math_matrix66_multiply( float A[6][6], float B[6][6], float c[6][6] );

void	math_matrix66_multiply1( float A[6][6], float B[6], float c[6] );

float	math_linear_equation6( float DA[6][6], float D[6], float X[6] );


	// MathMatrixInverse4.c
int	math_matrix_inverse4( float A[4][4], float B[4][4] );

int	math_matrixD_inverse4( double DA[4][4], double B[4][4] );

int	math_linear_equation4( float DA[4][4], float D[4], float X[4] );
int	math_linear_equation4D( double DA[4][4], double D[4], double X[4] );


	// MathMatrixInverse5.c
int	math_matrix_inverse5( float A[5][5], float B[5][5] );

int	math_matrixD_inverse5( double DA[5][5], double B[5][5] );


	// MathMatrixInverse9.c
int	math_matrix_inverse9( float A[9][9], float B[9][9] );

int	math_matrixD_inverse9( double DA[9][9], double B[9][9] );



	// MathLinerExquation9.c
int	math_linear_equation9( float DA[9][9], float A[9][9] );


	// MathMatrixInverse.c
int	math_matrix_inverse( float *aDA, float *aB, int N );

int	math_matrixD_inverse( double *aDA, double *aB, int N );





	// MathVector.c
float	vector_inner( float a[], float b[], int n );

float	vector_inner_w( float a[], float b[], float w[], int n );

float	vector_sum( float a[], int n );


void	vector_swap( float a[], float b[], int n );

void	vector_multiply( float a[], double t, int n );

void	vector_acumalute( float a[], float b[], double t, int n );

void	vector_weight_linear( float w[], int n, float a0, float a1 );

void	vector_weight_guasian( float w[], int n, float scale );

void	vector_weight_121( float w[], int n );



void	vector_print( FILE *fp, float *A, int n );


	// MathVEctorD.c
float	vectorD_inner( double a[], double b[], int n );

void	vectorD_swap( double a[], double b[], int n );

void	vectorD_multiply( double a[], double t, int n );

void	vectorD_acumalute( double a[], double b[], double t, int n );

float	vectorD_distance( double a[], double b[], int n );

float	vectorD_distance2( double a[], double b[], int n );


	// MathMatrix.c
void	math_matrix_multiply_vector( float *A, int m, int n, float	B[], float F[] );

void	math_matrixD_multiply_vector( double *A, int m, int n, double B[], double F[] );

void	math_matrix_print_file( char *file, float *a, int m, int n );

void	math_matrix_print( FILE *fp, float *A, int m, int n );


void	math_matrix_print_mathlab_file( char *file, float *a, int m, int n );

void	math_matrix_print_mathlab( FILE *fp, float *a, int m, int n );




	//	Quaternion.c
void	quaternion_from_rotation( matrix3_type *m, double q[4] );

void	quaternion_to_rotation( double q[4], matrix3_type *m );

#ifdef __cplusplus
}
#endif

#endif