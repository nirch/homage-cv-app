/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

/*******************
 ***   Vec3d.h   ***
 *******************/
#ifndef _VEC3D_TYPE_
#define _VEC3D_TYPE_


#ifdef __cplusplus
extern "C" {
#endif


typedef struct vec3f_type {
	float 	x;	
	float 	y;	
	float 	z;	
} vec3f_type;


typedef vec3f_type	vec3d;

typedef struct vec3fA_type {
	int	nA,	NA;

	vec3f_type	*a;

} vec3fA_type;






typedef struct vec3d_type {
	double 	x;	
	double 	y;	
	double 	z;	
} vec3d_type ;



typedef struct vec3dA_type {
	int	nA,	NA;

	vec3d_type	*a;

} vec3dA_type;



//typedef struct plane3d {
//	vec3f_type	n;
//	float	d;
//} plane3d ;
//
//
//
//
//typedef struct plane3d_type {		// n*p = d
//	vec3d_type	n;
//
//	double	d;
//} plane3d_type ;



#define		VEC3D_SET( v, vx, vy, vz ) \
			    { (v).x = vx; (v).y = vy; (v).z = vz; }

#define		VEC3D_INNER( u, v ) \
		( (u).x * (v).x  +  (u).y * (v).y + (u).z * (v).z )

#define		VEC3D_NORM2( v )	VEC3D_INNER( (v) , (v) )

#define		VEC3D_NORM( v )	  sqrt( VEC3D_INNER( (v) , (v) ) )



#define		VEC3D_NORMALIZE( v )	\
	{ \
	double   t = 1.0 / VEC3D_NORM( v ); \
        	(v).x *= t; \
        	(v).y *= t; \
        	(v).z *= t; \
	}


//#define VEC3D_NORMAL	VEC3D_NORMALIZE

#define		VEC3D_PRODUCT( a, b, c ) \
	{ \
		(c).x =  (a).y * (b).z  -  (a).z * (b).y ; \
		(c).y = -(a).x * (b).z  +  (a).z * (b).x ; \
		(c).z =  (a).x * (b).y  -  (a).y * (b).x ; \
	}

   

#define		VEC3D_MINUS( v )	\
{ \
	(v).x = -(v).x; \
	(v).y = -(v).y; \
	(v).z = -(v).z; \
}


	// make R2 to be orthonormal to R1;  assume that R1 is unit vector
void		vec3d_orthonormal( vec3d_type *R1, vec3d_type *R2 );


vec3fA_type *	vec3fA_alloc( int n );

vec3fA_type *	vec3fA_realloc( vec3fA_type *av, int n );


void	vec3fA_destroy( vec3fA_type *av );

float	vec3f_distance( vec3f_type *p0, vec3f_type *p1 );

float	vec3f_distane2( vec3f_type *p0, vec3f_type *p1 );


vec3dA_type *	vec3dA_alloc( int n );

void	vec3dA_destroy( vec3dA_type *av );

float	vec3f_line_distane( vec3f_type *p0, vec3f_type *v, vec3f_type *p );
float	vec3f_lineS_distane( vec3f_type *p0, vec3f_type *p1, vec3f_type *p );

float	vec3d_lineS_distane( vec3d_type *p0, vec3d_type *p1, vec3d_type *p );

int	vec3fA_write_to_file( vec3fA_type *av, char *file );

int	vec3fA_read_to_file( vec3fA_type **av, char *file );

void	vec3fA_dump(  vec3fA_type *av, char *prefix, int index, char *suffix );



int	vec3fA_remove_dup( vec3fA_type *av );

void	vec3fA_add( vec3fA_type *av, vec3f_type *p );

vec3fA_type	*vec3fA_circle_yz( float x, float r, int nr, vec3fA_type *av );

vec3fA_type	*vec3fA_circle_xy( float z, float r, int nr, vec3fA_type *av );




int	vec3fA_approximate_plane( vec3fA_type *av, int i0, int i1, vec3f_type *U, vec3f_type *V, vec3f_type *N, vec3f_type *p );


int	vec3fA_approximate_plane_test( vec3fA_type *av, int i0, int i1, vec3f_type *p, vec3f_type *v );



#ifdef _DUMP
#define VEC3F_DUMP( apt, name, index, ext )  vec3fA_dump( apt, name, index, ext )

#else
#define VEC3F_DUMP( apt, name, index, ext )

#endif

#ifdef __cplusplus
}
#endif

#endif
