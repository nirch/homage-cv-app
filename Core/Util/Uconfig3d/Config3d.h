/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/***********************
 ***	Config3d.h   ***
 ***********************/

#ifndef		_CONFIG_3D_
#define		_CONFIG_3D_

#include	<stdio.h>

#include	"Uigp/igp.h"



typedef struct config3d {
	float	x;
	float	y;
	float	z;


	float	u1;		// roll

	float	u2;		// yaw

	float	u3;		// pitch

}	config3d;



#define		ALLOC_CONFIG3D()  ( (config3d *)malloc( sizeof(config3d) ))

#define		CONFIG3D_IS_INIT( c )  \
	( (c).x == 0 && (c).y == 0 && (c).z == 0 && \
		(c).u1 == 0 && (c).u2 == 0 && (c).u3 == 0 )

#define		CONFIG3D_IS_EQUAL( c, c1 )  \
	( (c).x == (c1).x && (c).y == (c1).y && (c).z == (c1).z && \
		(c).u1 == (c1).u1 && (c).u2 == (c1).u2 && (c).u3 == (c1).u3 )


	/* Config3D.h */
void	config3d_init( config3d *cp );


	/* ConfigIo.h */
void	config3d_read( FILE *fp, config3d *cp );
void	config3d_write( FILE *fp, config3d *cp );

int	config3d_load( char *file, config3d cp[], int *cp_no );

int	config3d_dump( char *file, config3d cp[], int cp_no );


int	config3d_create_trajectory( config3d cm[], int cm_no,
					int no, config3d tm[] );

#endif
