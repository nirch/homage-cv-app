/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

/*******************
 ***   vec2d.h   ***
 *******************/
#ifndef	_VEC2D_TYPE_
#define _VEC2D_TYPE_

#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct vec2d_type {
	double	x;	
	double	y;	
} vec2d_type ;


typedef struct vec2f_type {
	float	x;	
	float	y;	
} vec2f_type ;


typedef vec2f_type	vec2d;



typedef struct vec2i_type {
	int	x;	
	int	y;	
} vec2i_type ;

//typedef vec2i_type vec2i;

typedef struct vec2s {
	short	x;	
	short	y;	
} vec2s ;



typedef struct plane2d {
	vec2d	n;
	float	d;
} plane2d;


typedef struct vec2fA_type {
	int	nA,	NA;
	vec2f_type	*a;

} vec2fA_type;


//#include "Ubox/Box2d.h"

#define		VEC2D_EQUAL(u,v)	( ((v).x == (u).x ) && ((v).y == (u).y) )

#define		VEC2D_SET( v, xval, yval )     { (v).x = xval; (v).y = yval; }

#define		VEC2D_DOT(u,v)		( (u).x * (v).x  +  (u).y * (v).y )

#define		VEC2D_INNER(u,v)	( (u).x * (v).x  +  (u).y * (v).y )

#define		VEC2D_NORM2( v )	VEC2D_DOT( (v) , (v) )

#define		VEC2D_NORMALIZE( v )	\
{ \
	double   t = 1.0 / hypot( (v).x, (v).y ); \
	(v).x *= t; \
	(v).y *= t; \
}

#define		VEC2D_SWAP( v )	\
{ \
	double   t = (v).x; \
	(v).x = (v).y; \
	(v).y = t; \
}


#define		VEC2D_LEFT( v, u )	\
{ \
	(u).x = -(v).y; \
	(u).y =  (v).x; \
}


#define		VEC2D_RIGHT( v, u )	\
{ \
	(u).x =  (v).y; \
	(u).y = -(v).x; \
}

#define		VEC2D_MINUS( v )	\
{ \
	(v).x = -(v).x; \
	(v).y = -(v).y; \
}


#define         VEC2D_UPDATE_BOUND( v, beg, end ) \
                        { \
                        if( (v).x < (beg).x )   (beg).x = (v).x;            \
                        else    if( (v).x > (end).x )      (end).x = (v).x; \
                        if( (v).y < (beg).y )   (beg).y = (v).y;            \
                        else    if( (v).y > (end).y )      (end).y = (v).y; \
                        }
   

	/* V2c2dTool.c */
vec2fA_type *	vec2fA_alloc( int n );

void	vec2fA_destroy( vec2fA_type *av );

int	vec2fA_write_to_file( vec2fA_type *av, char *file );

int	vec2fA_write( vec2fA_type *av,  FILE *fp );

vec2fA_type *	vec2fA_read( FILE *fp );




void	vec2d_reorder( vec2d V[], int no );

int	vec2d_select( vec2d V[], int no, vec2d *p );

float	vec2d_distance( vec2d *v0 ,vec2d *v1 );

float	vec2d_distance2( vec2d *v0 ,vec2d *v1 );


int	vec2d_joint( vec2d *mv0, vec2d *mv1, vec2d *mv );


float	vec2d_angle( vec2d *v );

void	vec2d_middle( vec2d *p0, vec2d *p1, vec2d *p);



int	vec2dA_write_to_file( vec2d av[], int nV, char *file );


void	vec2fA_approximate_line( vec2fA_type *av, vec2f_type *ctr, vec2f_type *v, float *len );



int	vec2fA_select( vec2fA_type *av, vec2d *p, float dT );

//int	vec2fA_box(vec2f_type ap[], int nA,  box2f_type *b  );


#ifdef __cplusplus
}
#endif

#endif
