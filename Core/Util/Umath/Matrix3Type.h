/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

#ifndef _MATRIX3_TYPE_
#define _MATRIX3_TYPE_
#include <stdio.h>
#include "Uvec/Vec3d.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct matrix3_type {
    union {

        struct {
            double        a00, a01, a02;
            double        a10, a11, a12;
            double        a20, a21, a22;
        };

		struct {
			vec3d_type	R0;
			vec3d_type	R1;
			vec3d_type	R2;
		};

        double a[3][3];

		double A[3*3];
    };
} matrix3_type;




void	matrix3_unit( matrix3_type *m );

void	matrix3_zero( matrix3_type *m );

void	matrix3_set( matrix3_type *m, vec3d_type *R0, vec3d_type *R1, vec3d_type *R2 );

void	matrix3_get( matrix3_type *m, vec3d_type *R0, vec3d_type *R1, vec3d_type *R2 );


void	matrix3_set_row( matrix3_type *m, vec3d_type *R0, vec3d_type *R1, vec3d_type *R2 );



void	matrix3_plus( matrix3_type *a, matrix3_type *b, matrix3_type *c );

void	matrix3_transpose( matrix3_type *a, double s, matrix3_type *m );

void	matrix3_mult_scalar( matrix3_type *a, double s, matrix3_type *m );


void	matrix3_mult( matrix3_type *a, matrix3_type *b, matrix3_type *c );

void	matrix3_mult_vector( matrix3_type *a, float *b, float *c );
void	matrix3_multV( matrix3_type *m, vec3f_type *b, vec3f_type *c );
void	matrix3_multVD( matrix3_type *m, vec3d_type *b, vec3d_type *c );
void	matrix3_multV_N( matrix3_type *m, float bx, float by, float bz, float *cx, float *cy, float *cz );

void	matrix3_multVD_T( matrix3_type *m, vec3d_type *b, vec3d_type *c );


void	matrix3_mult( matrix3_type *a, matrix3_type *b, matrix3_type *c );

void	matrix3_inverse( matrix3_type *m, matrix3_type *im );

void	matrix3_solve( matrix3_type *m, vec3d_type *D, vec3d_type *X );

void	matrix3S_solve( matrix3_type *m, vec3d_type *D, vec3d_type *X );

void	matrix3_column( matrix3_type *m, vec3d_type *R0, vec3d_type *R1, vec3d_type *R2 );

void	matrix3_column_i( matrix3_type *m, int j, vec3d_type *c );


void	matrix3_rotationZ( matrix3_type *m, double rz );  // rotation around Z axis

void	matrix3_rotationY( matrix3_type *m, double ry );

void	matrix3_rotationX( matrix3_type *m, double rx );


void	matrix3_rotationXYZ( matrix3_type *m, double rx, double ry, double rz );

void	matrix3_rotationYXZ( matrix3_type *m, double rx, double ry, double rz );
void	matrix3_rotationYXZ_inv( matrix3_type *m, float rx, float ry, float rz );

void	matrix3_rotationYZX( matrix3_type *m, double rx, double ry, double rz );


void	matrix3_rotationYXZ_decompose_1( matrix3_type *m, double *rx, double *ry, double *rz );

void	matrix3_rotationZYX( matrix3_type *m, double rx, double ry, double rz );

void	matrix3_rotationZYX_decompose( matrix3_type *m, double *rx, double *ry, double *rz );


void	matrix3_rotation_OPK( matrix3_type *m, double omega, double phi, double kappa );

void	matrix3_rotation_OPK_decompose( matrix3_type *m, double *omega, double *phi, double *kappa );


int		matrix3_write_to_file( matrix3_type *m, char *file );

void	matrix3_write( matrix3_type *m, FILE *fp );
int		matrix3_dump( matrix3_type *m, char *prefix, int index, char *suffix );


int	matrix3_to_quaternion( matrix3_type *m, double q[] ); // w  x  y   z

void	matrix3_from_quaternion( matrix3_type *m, double q[] ); // w x  y   


double	matrix3_L2( matrix3_type *m1, matrix3_type *m2 );

double	matrix3_dmax( matrix3_type *m1, matrix3_type *m2 );

int	matrix3_test( matrix3_type *m );


void	matrix3_linaer_equation_add( matrix3_type *m, double D[], double X[], double y );



	// MatrixEigen.c
int	matrixS3_eigen( matrix3_type *m, matrix3_type *em, double ev[] );

#ifdef __cplusplus
}
#endif

#endif