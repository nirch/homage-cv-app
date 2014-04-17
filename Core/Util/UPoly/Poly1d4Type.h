/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/*************************
 ***   Poly1d4Type.c   ***
 *************************/


#ifndef		_POLY1D4_TYPE_
#define		_POLY1D4_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include	"Uvec/Vec2d.h"



#define		POLY1D4_ORDER	4
#define		POLY1D4_COEFF	( POLY1D4_ORDER + 1)

typedef struct poly1d4_type {
    union {
        struct {
            double	a0;

			double	a1;
			double	a2;
			
			double	a3;
			double	a4;

        };

        double a[ POLY1D4_COEFF ];
    };
} poly1d4_type;


#define	POLY1D4_VALUE( f, x )	((f).a0 + (x)*((f).a1 + (x)*((f).a2 + (x)*((f).a3 + (x)*(f).a4 )) )) 


void	poly1d4_zero( poly1d4_type *f );


void	poly1d4_derivative( poly1d4_type *f, poly1d4_type *fx );

void	poly1d4_shift( poly1d4_type *f, double x, poly1d4_type *g );


void	poly1d4_max_diff( poly1d4_type *f0, poly1d4_type *f1, poly1d4_type *mf );

void	poly1d4_print( poly1d4_type *f, FILE *fp );


double	poly1d4_curvtur( poly1d4_type *f, double x0 );


#ifdef __cplusplus
}
#endif
#endif
