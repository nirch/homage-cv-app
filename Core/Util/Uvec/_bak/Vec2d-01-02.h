/*******************
 ***   vec2d.h   ***
 *******************/
#ifndef	_VEC2D_TYPE_
#define _VEC2D_TYPE_



typedef struct vec2d {
	float	x;	
	float	y;	
} vec2d ;


typedef struct vec2i {
	int	x;	
	int	y;	
} vec2i ;

typedef struct vec2s {
	short	x;	
	short	y;	
} vec2s ;



typedef struct plane2d {
	vec2d	n;
	float	d;
} plane2d;


typedef struct vec2fA_type {
	int	nV;
	vec2d	*v;

} vec2fA_type;


#define		VEC2D_EQUAL(u,v)	( ((v).x == (u).x ) && ((v).y == (u).y) )

#define		VEC2D_SET( v, xval, yval )     { (v).x = xval; (v).y = yval; }

#define		VEC2D_DOT(u,v)		( (u).x * (v).x  +  (u).y * (v).y )

//#define		VEC2D_INNER(u,v)	( (u).x * (v).x  +  (u).y * (v).y )

#define		VEC2D_NORM2( v )	VEC2D_DOT( (v) , (v) )


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


void	vec2d_reorder( vec2d V[], int no );

int	vec2d_select( vec2d V[], int no, vec2d *p );

float	vec2d_distance( vec2d *v0 ,vec2d *v1 );

float	vec2d_distance2( vec2d *v0 ,vec2d *v1 );


int	vec2d_joint( vec2d *mv0, vec2d *mv1, vec2d *mv );


float	vec2d_angle( vec2d *v0 ,vec2d *v1 );



#endif
