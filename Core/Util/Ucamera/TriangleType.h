/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/*******************
 ***   Triangle.h   ***
 *******************/
#ifndef	_TRIANGLE_TYPE_
#define _TRIANGLE_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include "Uvec/Vec2d.h"

#include "Pt2dType.h"



	// p2d	-  2d point

typedef struct triangle_type {

	vec2d	p[3];
	int	i0;
	int	i1;
	int	i2;

	float	area;
} triangle_type ;



typedef struct triangleA_type {
	int	nT,	NT;

	triangle_type	*t;

	float	av;
	float	var;

} triangleA_type;




	/* V2c2dTool.c */
triangleA_type *	triangleA_alloc( int n );

void	triangleA_destroy( triangleA_type *av );

void	triangleA_add( triangleA_type *at, pt2dA_type *apt, int i0, int i1, int i2 );


void	triangleA_measure( triangleA_type *at, float *av, float *var );


pt2dA_type *	triangleA_circle( triangleA_type *at, float areaT );


float	triangle_aera( vec2d *p0, vec2d *p1, vec2d *p2 );

void	triangle_bounded_circle( triangle_type	*tr, vec2f_type *ctr, float *r );


#ifdef __cplusplus
}
#endif

#endif
