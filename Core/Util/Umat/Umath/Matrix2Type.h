/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _MATRIX2_TYPE_
#define _MATRIX2_TYPE_

#include <stdio.h>
#include "Uvec/Vec2d.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct matrix2_type {
    union {

        struct {
            double        a00, a01;
            double        a10, a11;
        };

        double a[2][2];

		double A[2*2];
    };
} matrix2_type;


typedef struct matrix2f_type {
	union {

		struct {
			float        a00, a01;
			float        a10, a11;
		};

		float a[2][2];

		float A[2*2];
	};
} matrix2f_type;



void	matrix2_unit( matrix2_type *m );

void	matrix2_zero( matrix2_type *m );
void	matrix2f_zero( matrix2f_type *m );


void	matrix2_mult( matrix2_type *a, matrix2_type *b, matrix2_type *c );

void	matrix2_mult_vector( matrix2_type *m, float *b, float *c );

void	matrix2_mult_vectorN( matrix2_type *m,
							float x, float y,
							float *mx, float *my );


int	matrix2_solve( matrix2_type *m, vec2d_type *D, vec2d_type *X );

int	matrix2S_solve( matrix2_type *m, vec2d_type *D, vec2d_type *X );

int	matrix2fS_solve( matrix2f_type *m, vec2f_type *D, vec2f_type *X );


int	matrix2S_eigen( matrix2_type *m, float *e1, vec2f_type *v1, float *e2 );





//void	matrix2_inverse( matrix2_type *m, matrix2_type *im );


int		matrix2_write_to_file( matrix2_type *m, char *file );

void	matrix2_write( matrix2_type *m, FILE *fp );



#ifdef __cplusplus
}
#endif

#endif