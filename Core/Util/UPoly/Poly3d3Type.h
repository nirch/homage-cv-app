/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

/*************************
 ***   Poly3d3Type.c   ***
 *************************/


#ifndef		_poly3d3_TYPE_
#define		_poly3d3_TYPE_


#include	"Uvec/Vec2d.h"


#define		POLY3D3_ORDER	3
#define		POLY3D3_COEFF	20 //(( poly3d3_ORDER + 1)*( poly3d3_ORDER +2)/2)


typedef struct poly3d3_type {
    union {
        struct {
            double	a000;

			double	a100;
			double	a010;
			double	a001;
			
			double	a110;
			double	a101;
			double	a011;
			double	a200;
			double	a020;
			double	a002;

			double	a111;
			double	a300;
			double	a120;
			double	a102;
			double	a210;
			double	a030;
			double	a012;
			double	a201;
			double	a021;
			double	a003;

			
        };

        double a[ POLY3D3_COEFF ];
    };
} poly3d3_type;


//#define	poly3d3_VALUE( f, x, y ) \
//				    ((f).a00 + (x)*((f).a10 + (x)*((f).a20 + (x)*(f).a30 )) +\
//				(y)*((f).a01 + (x)*((f).a11 + (x)*(f).a21 ) +\
//				(y)*((f).a02 + (x)* (f).a12 +\
//				(y)*((f).a03) )))



double		poly3d3_value( poly3d3_type *f, double x, double y, double z );


void	poly3d3_zero( poly3d3_type *f );

void	poly3d3_derivative_x( poly3d3_type *f, poly3d3_type *fx );

void	poly3d3_derivative_y( poly3d3_type *f, poly3d3_type *fy );

#endif
