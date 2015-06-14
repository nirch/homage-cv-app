/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/*******************
 ***   Box3d.h   ***
 *******************/


#ifndef         BOX3D_TYPE
#define         BOX3D_TYPE

#ifdef __cplusplus
extern "C" {
#endif




#include	"Uvec/Vec3d.h"



typedef struct box3f_type {

	union {
		struct {
			float	x0;
			float	y0;
			float	z0;
		};

		vec3f_type	p0;
	};


	union {
		struct {
			float	x1;
			float	y1;
			float	z1;
		};

		vec3f_type	p1;
	};

} box3f_type ;







typedef struct box3d_type {

	union {
		struct {
			double	x0;
			double	y0;
			double	z0;
		};

		vec3d_type	p0;
	};


	union {
		struct {
			double	x1;
			double	y1;
			double	z1;
		};

		vec3d_type	p1;
	};

} box3d_type ;



typedef struct box3i_type {
	int	x0,	x1;
	int	y0,	y1;
	int	z0,	z1;
} box3i_type ;




#define		BOX3D_SET( b, x0, x1, y0, y1, z0, z1 ) \
			{ \
				(b).x0 = x0;	\
			    	(b).x1 = x1;	\
			    	(b).y0 = y0;	\
			    	(b).y1 = y1;	\
					(b).z0 = z0;	\
					(b).z1 = z1;	\
			}

#define         BOX3D_INIT( b, x, y, z ) \
			{ \
				(b).x0 = (b).x1 = x;	\
			   	(b).y0 = (b).y1 = y;	\
			   	(b).z0 = (b).z1 = z;	\
			}


#define		BOX3D_COPY( sb, b ) \
{ \
	(b).x0 = (sb).x0;	\
	(b).x1 = (sb).x1;	\
	(b).y0 = (sb).y0;	\
	(b).y1 = (sb).y1;	\
	(b).z0 = (sb).z0;	\
	(b).z1 = (sb).z1;	\
}


#define         BOX3D_UPDATE( b, x, y, z ) \
			{ \
				if( x < (b).x0 )	(b).x0 = x; \
				else	if( x > (b).x1 )        (b).x1 = x; \
				if( y < (b).y0 )	(b).y0 = y; \
				else	if( y > (b).y1 )        (b).y1 = y; \
				if( z < (b).z0 )	(b).z0 = z; \
				else	if( z > (b).z1 )        (b).z1 = z; \
			}

#define         BOX3D_UPDATEP( b, p ) \
{ \
	if( (p).x < (b).x0 )	(b).x0 = (p).x; \
				else	if( (p).x > (b).x1 )        (b).x1 = (p).x; \
				if( (p).y < (b).y0 )	(b).y0 = (p).y; \
				else	if( (p).y > (b).y1 )        (b).y1 = (p).y; \
				if( (p).z < (b).z0 )	(b).z0 = (p).z; \
				else	if( (p).z > (b).z1 )        (b).z1 = (p).z; \
}


#define         BOX3D_UPDATE_BOX( b, b1 ) \
{ \
				if( (b1).x0 < (b).x0 )	(b).x0 = (b1).x0; \
				if( (b1).y0 < (b).y0 )	(b).y0 = (b1).y0; \
				if( (b1).z0 < (b).z0 )	(b).z0 = (b1).z0; \
				if( (b1).x1 > (b).x1 )	(b).x1 = (b1).x1; \
				if( (b1).y1 > (b).y1 )	(b).y1 = (b1).y1; \
				if( (b1).z1 > (b).z1 )	(b).z1 = (b1).z1; \
}


#define         BOX3D_IS_IN( b, x, y, z ) \
			(	x >= (b).x0 && x <= (b).x1  &&  \
				y >= (b).y0 && y <= (b).y1 &&\
				z >= (b).z0 && z <= (b).z1 )

#define         BOX3D_IS_IN_S( b, x, y, z, S ) \
				(	x >= (b).x0-S && x <= (b).x1+S  &&  \
					y >= (b).y0-S && y <= (b).y1+S &&\
					z >= (b).z0-S && z <= (b).z1+S )


#define         BOX3D_IS_IN_O( b, x, vy ) \
			(	x > (b).x0 && x < (b).x1  &&  \
				y > (b).y0 && y < (b).y1  &&  z > (b).z0 && z < (b).z1 )





#define		BOX3D_IS_BOX_IN( b, b1 )	(  BOX3D_IS_IN( b, (b1).x0, (b1).y0, (b1).z0 ) && \
											BOX3D_IS_IN( b, (b1).x1, (b1).y1, (b1).z1 ) )


#define         BOX3D_IS_EQUAL( b, b1 )  \
						( (b).x0 == (b1).x0 && (b).y0 == (b1).y0  && (b).z0 == (b1)z0  &&  (b).x1 == (b1).x1 && (b).y1 == (b1).y1  && (b).z1 == (b1).z1 )







#ifdef __cplusplus
}
#endif

#endif
