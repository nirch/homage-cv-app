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



typedef struct vec3d_type {
	double 	x;	
	double 	y;	
	double 	z;	
} vec3d_type ;


typedef struct vec3f_type {
	float 	x;	
	float 	y;	
	float 	z;	
} vec3f_type;


typedef vec3f_type	vec3d;




typedef struct plane3d {
	vec3f_type	n;
	float	d;
} plane3d ;


typedef struct plane3d_type {
	vec3d_type	n;

	double	d;
} plane3d_type ;



#define		VEC3D_SET( v, vx, vy, vz ) \
			    { (v).x = vx; (v).y = vy; (v).z = vz; }

#define		VEC3D_INNER( u, v ) \
		( (u).x * (v).x  +  (u).y * (v).y + (u).z * (v).z )

#define		VEC3D_NORM2( v )	VEC3D_INNER( (v) , (v) )

#define		VEC3D_NORM( v )	  sqrt( VEC3D_INNER( (v) , (v) ) )



#define		VEC3D_NORMAL( v )	\
	{ \
	double   t = 1.0 / VEC3D_NORM( v ); \
        	(v).x *= t; \
        	(v).y *= t; \
        	(v).z *= t; \
	}



#define		VEC3D_PRODUCT( a, b, c ) \
	{ \
		(c).x =  (a).y * (b).z  -  (a).z * (b).y ; \
		(c).y = -(a).x * (b).z  +  (a).z * (b).x ; \
		(c).z =  (a).x * (b).y  -  (a).y * (b).x ; \
	}

   


	// make R2 to be orthonormal to R1;  assume that R1 is unit vector
void		vec3d_orthonormal( vec3d_type *R1, vec3d_type *R2 );




#ifdef __cplusplus
}
#endif

#endif
