/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/********************
 ***   SlType.h   ***
 ********************/

#ifndef	_SL_TYPE_
#define _SL_TYPE_

#ifdef __cplusplus
extern "C" {
#endif



#include "Uvec/Vec2d.h"
#include "Uvec/Vec3d.h"

// straight line 
//  a * x + b * y + d = 0;



typedef	struct sl_type	{
	union {

		vec3f_type	l;

		struct {
			union {
				vec2f_type	n;

				struct {
					float	a;
					float	b;
				};
			};

			float	d;
		};


	};


	float	t0;
	float	t1;

} sl_type;



void	sl_cutpoint( sl_type *sl0, sl_type *sl1, vec2f_type *p );

void	sl_set( sl_type *sl, vec2f_type *p0, vec2f_type *p1 );



#ifdef __cplusplus
}
#endif

#endif
