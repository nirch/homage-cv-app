/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _MATRIX4_TYPE_
#define _MATRIX4_TYPE_

#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uvec/Vec3d.h"


typedef struct matrix4_type {
    union {
        struct {
            double        a00, a01, a02, a03;
            double        a10, a11, a12, a13;
            double        a20, a21, a22, a23;
            double        a30, a31, a32, a33;

        };

        double a[4][4];
    };
} matrix4_type;



typedef struct matrix4A_type {

	int nA;
	int	NA;

	matrix4_type	*a;

} matrix4A_type;


matrix4_type *	matrix4_alloc();

void	matrix4_destroy( matrix4_type *m );

void	matrix4_zero( matrix4_type *m );

void	matrix4_unity( matrix4_type *m );

void	matrix4_set( matrix4_type *m, float *a );
void	matrix4_setD( matrix4_type *m, double *a );


void	matrix4_multiply_scalar( matrix4_type *m, float a, matrix4_type *tm );

void	matrix4H_multiply_scalar( matrix4_type *m, float a, matrix4_type *tm );



void	matrix4_mult( matrix4_type *a, matrix4_type *b, matrix4_type *c );

void	matrix4_mult_vector( matrix4_type *a, float *b, float *c );

void	matrix4_multVD( matrix4_type *a, double *b, double *c );

void	matrix4H_mult_vector( matrix4_type *m, vec3f_type *b, vec3f_type *c );

void	matrix4H_mult_vectorD( matrix4_type *m, vec3d_type *b, vec3d_type *c );


//void	matrix4H_mult_V( matrix4_type *m, vec3f_type *b, vec3f_type *c );

void	matrix4H_set( matrix4_type *m, double T[3][4] );

void	matrix4H_setV( matrix4_type *m, vec3d_type *R1, vec3d_type *R2, vec3d_type *R3, vec3d_type *T );
void	matrix4H_getV( matrix4_type *m, vec3f_type *R1, vec3f_type *R2, vec3f_type *R3, vec3f_type *T );

void	matrix4H_set_RT( matrix4_type *m, struct matrix3_type *R, vec3d_type *T );
void	matrix4H_get_RT( matrix4_type *m, struct matrix3_type *R, vec3d_type *T );

void	matrix4H_get_T( matrix4_type *m, vec3d_type *T );


void	matrix4H_RT2CP( matrix4_type *m, vec3d_type *cp, vec3d_type *N );


void	matrix4H_compose( matrix4_type *m, double x, double y, double z, double omega, double phi, double kappa );

void	matrix4H_decompose( matrix4_type *m, double *x, double *y, double *z, double *omega, double *phi, double *kappa );
void	matrix4H_decomposeS( matrix4_type *m, double *x, double *y, double *z, double *omega, double *phi, double *kappa, double *scale );



double	matrix4H_L2( matrix4_type *m1, matrix4_type *m2 );
double	matrix4_dmax( matrix4_type *m1, matrix4_type *m2 );


int	matrix4H_test( matrix4_type	*m );





void	matrix4_rotationZ( matrix4_type *m, float rz );  // rotation around Z axis

void	matrix4_rotationY( matrix4_type *m, float ry );

void	matrix4_rotationX( matrix4_type *m, float yx );



void	matrix4_rotationYXZ( matrix4_type *m, float rx, float ry, float rz );

void	matrix4_inverse( matrix4_type *m, matrix4_type *im );

int	matrix4_solve( matrix4_type *m, double D[], double X[] );


void	matrix4_read( matrix4_type *m, FILE *fp );

void	matrix4_write( matrix4_type *m, FILE *fp );


void	matrix4H_read( matrix4_type *m, FILE *fp );

void	matrix4H_write( matrix4_type *m, FILE *fp );

int		matrix4_read_file( matrix4_type *m, char *file );

int		matrix4_write_file( matrix4_type *m, char *file );

int		matrix4_dump( matrix4_type *m, char *prefix, int index, char *suffix );

int	matrix4_to_quaternion( matrix4_type *m, double q[7] );

int	matrix4_from_quaternion( matrix4_type *m, double q[7] );


matrix4A_type *	matrix4A_alloc( int n );

matrix4A_type *	matrix4A_realloc( matrix4A_type *am, int n );

void	matrix4A_destroy( matrix4A_type *am );

matrix4A_type *	matrix4A_transform( matrix4A_type *oM, matrix4_type *T, matrix4A_type *tM );

void	matrix4_linaer_equation_add( matrix4_type *m, double D[], double X[], double y );


#ifdef __cplusplus
}
#endif

#endif