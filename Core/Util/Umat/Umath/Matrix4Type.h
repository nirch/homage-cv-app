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


void	matrix4_zero( matrix4_type *m );

void	matrix4_unity( matrix4_type *m );

void	matrix4_set( matrix4_type *m, float *a );


void	matrix4_mult( matrix4_type *a, matrix4_type *b, matrix4_type *c );

void	matrix4_mult_vector( matrix4_type *a, float *b, float *c );

void	matrix4H_mult_vector( matrix4_type *m, vec3d *b, vec3d *c );

void	matrix4H_mult_V( matrix4_type *m, vec3d *b, vec3d *c );

void	matrix4H_set( matrix4_type *m, double T[3][4] );

void	matrix4H_setV( matrix4_type *m, vec3f_type *R1, vec3f_type *R2, vec3f_type *R3, vec3f_type *T );
void	matrix4H_getV( matrix4_type *m, vec3f_type *R1, vec3f_type *R2, vec3f_type *R3, vec3f_type *T );


void	matrix4_rotationZ( matrix4_type *m, float rz );  // rotation around Z axis

void	matrix4_rotationY( matrix4_type *m, float ry );

void	matrix4_rotationX( matrix4_type *m, float yx );



void	matrix4_rotationYXZ( matrix4_type *m, float rx, float ry, float rz );

void	matrix4_inverse( matrix4_type *m, matrix4_type *im );


void	matrix4_read( matrix4_type *m, FILE *fp );

void	matrix4_write( matrix4_type *m, FILE *fp );

int		matrix4_dump( matrix4_type *m, char *prefix, int index, char *suffix );

void	matrix4_to_quaternion( matrix4_type *m, double q[7] );

void	matrix4_from_quaternion( matrix4_type *m, double q[7] );




#ifdef __cplusplus
}
#endif

#endif