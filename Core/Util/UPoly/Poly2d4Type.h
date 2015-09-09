/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

/*************************
 ***   Poly2d4Type.c   ***
 *************************/


#ifndef		_POLY2D4_TYPE_
#define		_POLY2D4_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include	<stdio.h>

#include	"Uvec/Vec2d.h"
#include	"Poly2d3Type.h"

#include	"Poly1d4Type.h"



#define		POLY2D4_ORDER	4
#define		POLY2D4_COEFF	(( POLY2D4_ORDER + 1)*( POLY2D4_ORDER +2)/2)


typedef struct poly2d4_type {
    union {
        struct {
            double	a00;

			double	a10;
			double	a01;
			
			double	a20;
			double	a11;
			double	a02;

			double	a30;
			double	a21;
			double	a12;
			double	a03;

			double	a40;
			double	a31;
			double	a22;
			double	a13;
			double	a04;
        };

        double a[ POLY2D4_COEFF ];
    };
} poly2d4_type;


#define	POLY2D4_VALUE( f, x, y ) \
				    ((f).a00 + (x)*((f).a10 + (x)*((f).a20 + (x)*((f).a30 + (x)*((f).a40)) )) +\
				(y)*((f).a01 + (x)*((f).a11 + (x)*((f).a21 + (x)*((f).a31) )) +\
				(y)*((f).a02 + (x)*((f).a12 + (x)*((f).a22) ) +\
				(y)*((f).a03 + (x)*((f).a13)+\
				(y)*((f).a04 ) ))))


double	poly2d4_value( poly2d4_type *f, double x, double y );

void	poly2d4_zero( poly2d4_type *f );

void	poly2d4_set( poly2d4_type *f, poly2d3_type *g );


void	poly2d4_derivative_x( poly2d4_type *f, poly2d3_type *fx );

void	poly2d4_derivative_y( poly2d4_type *f, poly2d3_type *fy );

void	poly2d4_shift( poly2d4_type *f, double x, double y, poly2d4_type *g );


void	poly2d4_max_diff( poly2d4_type *f0, poly2d4_type *f1, poly2d4_type *mf );

void	poly2d4_print( poly2d4_type *f, FILE *fp );


void	poly2d4_value_y( poly2d4_type *f, double y, poly1d4_type *fx );


int	poly2d4_read( poly2d4_type *f, FILE *fp );

void poly2d4_write( poly2d4_type *f, FILE *fp );




#ifdef __cplusplus
}
#endif
#endif
