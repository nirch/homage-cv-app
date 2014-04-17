/**********************
 *** ViewEulerV.c   ***
 **********************/
#include	<stdio.h>
#include	<math.h>


#include	"ViewType.h"


// The roll angle is given according to VR


void
view_eulerV_set( view_type *vw, vec3d *VR, float roll, float yaw, float pitch  )
{
vec3d	N,	V,	U;

float	cos_p,	sin_p;
float	cos_y,	sin_y;
float	cos_r,	sin_r;
float	t;

	cos_p = cos( pitch );
	sin_p = sin( pitch );

	cos_y = cos( yaw );
	sin_y = sin( yaw );




	N.x = -sin_y;
	N.y = -cos_y * sin_p;
	N.z = cos_y * cos_p;


#ifdef	VIEW_MINUS_N
	N.x = -N.x;
	N.y = -N.y;
	N.z = -N.z;
#endif




	t = VEC3D_INNER( *VR, N );
    V.x = VR->x - t * N.x;
    V.y = VR->y - t * N.y;
    V.z = VR->z - t * N.z;

	t = 1/VEC3D_NORM( V );
	V.x *= t;
	V.y *= t;
	V.z *= t;


	VEC3D_PRODUCT( N, V, U );


	cos_r = cos( roll );
	sin_r = sin( roll );


	vw->V.x = -sin_r * U.x + cos_r * V.x;
	vw->V.y = -sin_r * U.y + cos_r * V.y;
	vw->V.z = -sin_r * U.z + cos_r * V.z;



	view_per_set( vw, &vw->cp, 1, &N, 1, &vw->V, 1 );
}



void
view_eulerV( view_type *vw, vec3d *VR, float *roll, float *yaw, float *pitch  )
{
float	cos_p;
vec3d	N,	V,	U;
float	t,	x,	y;

	N.x = -vw->N.x;
	N.y = -vw->N.y;
	N.z = -vw->N.z;

	
	*pitch = atan2( -N.y, N.z );

	cos_p = cos( *pitch );
	if( cos_p != 0 )
		*yaw = atan2( -N.x, N.z/cos_p );
	else	// sin( *pitc )	== 1;
		*yaw = atan2( -N.x, -N.y );




	t = VEC3D_INNER( *VR, N );
    V.x = VR->x - t * N.x;
    V.y = VR->y - t * N.y;
    V.z = VR->z - t * N.z;


	t = 1/VEC3D_NORM( V );
	V.x *= t;
	V.y *= t;
	V.z *= t;


	VEC3D_PRODUCT( vw->N, V, U );


	x = VEC3D_INNER( U, vw->U );
	y = VEC3D_INNER( V, vw->U );


	*roll = atan2( y, x ); 
}




void
view_eulerV_set_by_vector_roll( view_type *vw, vec3d *VR, vec3d *N, float roll )
{
float	pitch,	yaw;
float	t,	cos_p;

	t = sqrt( VEC3D_INNER( *N, *N ) );
	N->x /= t;
	N->y /= t;
	N->z /= t;

	N->x = -N->x;
	N->y = -N->y;
	N->z = -N->z;


	pitch = atan2( -N->y, N->z );

	cos_p = cos( pitch );
	if( cos_p != 0 )
		yaw = atan2( -N->x, N->z/cos_p );
	else	// sin( *pitc )	== 1;
		yaw = atan2( -N->x, -N->y );


	view_eulerV_set( vw, VR, roll, yaw, pitch );
}













