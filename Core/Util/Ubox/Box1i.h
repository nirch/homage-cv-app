/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/*******************
 ***   Box1d.h   ***
 *******************/


#ifndef         BOX1D_TYPE
#define         BOX1D_TYPE

#ifdef __cplusplus
extern "C" {
#endif


#include	<stdio.h>

#include	"Uvec/Vec2d.h"




typedef struct box1i_type {

	int	i0;
	int	i1;

	int n;
} box1i_type;






#define		BOX1D_SET( b, i0, i1 ) \
			{ \
				(b).i0 = i0;	\
			    (b).i1 = i1;	\
			}






#define         BOX1D_UPDATE( b, x, y ) \
			{ \
				if( x < (b).i0 )	(b).i0 = x; \
				else	if( x > (b).i1 )        (b).i1 = x; \
			}


#define         BOX1D_UPDATE_BOX( b, b1 ) \
{ \
				if( (b1).i0 < (b).i0 )	(b).i0 = (b1).i0; \
				if( (b1).i0 < (b).i0 )	(b).i0 = (b1).i0; \
}


#define         BOX1D_IS_IN( b, vx, vy ) \
			(	vx >= (b).i0 && vx <= (b).i1  )







#ifdef __cplusplus
}
#endif

#endif
