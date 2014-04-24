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




int	plna3d_cut_line( plane3d_type *A, plane3d_type *B, vec3d_type *p, vec3d_type *n );



#ifdef __cplusplus
}
#endif

#endif
