/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

/*************************
 ***   Poly2d3Type.c   ***
 *************************/


#ifndef		_POLY2D3_TYPE_
#define		_POLY2D3_TYPE_


#include	"Uvec/Vec2d.h"


#define		POLY2D3_ORDER	3
#define		POLY2D3_COEFF	(( POLY2D3_ORDER + 1)*( POLY2D3_ORDER +2)/2)


typedef struct poly2d3_type {
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
        };

        double a[ POLY2D3_COEFF ];
    };
} poly2d3_type;


#define	POLY2D3_VALUE( f, x, y ) \
				    ((f).a00 + (x)*((f).a10 + (x)*((f).a20 + (x)*(f).a30 )) +\
				(y)*((f).a01 + (x)*((f).a11 + (x)*(f).a21 ) +\
				(y)*((f).a02 + (x)* (f).a12 +\
				(y)*((f).a03) )))



void	poly2d3_zero( poly2d3_type *f );

void	poly2d3_derivative_x( poly2d3_type *f, poly2d3_type *fx );

void	poly2d3_derivative_y( poly2d3_type *f, poly2d3_type *fy );

#endif
