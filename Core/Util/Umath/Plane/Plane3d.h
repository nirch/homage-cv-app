/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

/*********************
 ***   Plane3d.h   ***
 *********************/
#ifndef _PLANE_TYPE_
#define _PLANE_TYPE_


#ifdef __cplusplus
extern "C" {
#endif


#include "Uvec/Vec3d.h"



typedef struct plane3f_type {
	vec3f_type	n;
	float	d;
} plane3f_type ;




typedef struct plane3d_type {		// n*p = d
	vec3d_type	n;

	double	d;
} plane3d_type ;




// compute cut line of two planes
int	plane3d_cut_line( plane3d_type *A, plane3d_type *B, vec3d_type *p, vec3d_type *n );


// compute the cut point of   plane and the vectot cp + t*v
int	plane3d_cut_point( plane3d_type *A, vec3d_type *cp, vec3d_type *v, vec3d_type *p );


// find point on plane P + t*V + s*U   nearest to all the plane
int	plane3d_nearset_point( plane3d_type ap[], int nP, vec3d_type *P, vec3d_type *V, vec3d_type *U, vec3d_type *p );


#ifdef __cplusplus
}
#endif

#endif
