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


typedef struct eigen2d_type {
	vec2f_type	p;

	vec2f_type	v1;
	vec2f_type	v2;

	float	e1;
	float	e2;

	float	r;
}eigen2d_type;




int	eige2d_swap( eigen2d_type  *se, eigen2d_type *te );

int	eige2d_lt( eigen2d_type  *se, eigen2d_type *te, float d, struct lt2_type *lt );


#ifdef __cplusplus
}
#endif

#endif