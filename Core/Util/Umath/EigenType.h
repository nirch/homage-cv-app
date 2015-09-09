/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _EIGEN_TYPE_
#define _EIGEN_TYPE_


#ifdef __cplusplus
extern "C" {
#endif

#include "Uigp/igp.h"
#include "Uvec/Vec2d.h"
#include "Umath/Matrix2Type.h"


typedef struct eigen2d_type {
	vec2f_type	p;

	vec2f_type	v1;
	vec2f_type	v2;

	float	e1;
	float	e2;

	float	r;
}eigen2d_type;



typedef struct eigen2dAux_type {

	int	id;

	vec2f_type	p;

	matrix2_type m;

	int n;


} eigen2dAux_type;


#define EIGEN2D_UPDATE( ea, x0, y0 ) \
{ \
	(ea).p.x += (x0); \
	(ea).p.y += (y0); \
	(ea).m.a00 += (x0)*(x0); \
	(ea).m.a01 += (x0)*(y0);  \
	(ea).m.a11 += (y0)*(y0); \
	(ea).n++; \
} 




int	eige2d_swap( eigen2d_type  *se, eigen2d_type *te );

int	eige2d_lt( eigen2d_type  *se, eigen2d_type *te, float d, struct lt2_type *lt );



void	eigen2dAux_init( eigen2dAux_type *e, int id );
int		eigen2dAux_final( eigen2dAux_type *ea, eigen2d_type *e );

int		eigen2dAuxA_sum( eigen2dAux_type ae[], int nE, eigen2dAux_type *e );

#ifdef __cplusplus
}
#endif

#endif