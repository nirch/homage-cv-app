
#ifndef _ELLIPSE_H_
#define _ELLIPSE_H_

#include "Uvec/Vec2d.h"


typedef struct ellipse_type {

	vec2f_type	p;

	vec2f_type	v;

	float		rx,	ry;

	float s;

} ellipse_type;


ellipse_type *	ellipse_alloc();


void	ellipse_destroy( ellipse_type *e );

void	ellipse_set( ellipse_type *e, struct eigen2d_type *eg );

void	ellipse_scale( ellipse_type *e, float scale );


float	ellipse_distance( ellipse_type *e, vec2f_type *p );

float	ellipse_distance( ellipse_type *be, ellipse_type *e );

float	ellipse_distanceN( ellipse_type *be, ellipse_type *e, int N );


int		ellipse_union( ellipse_type *e0, float w0, ellipse_type *e1, float w1, ellipse_type *e );


#endif


