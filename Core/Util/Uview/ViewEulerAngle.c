/**************************
 *** ViewEulerAngle.c   ***
 **************************/
#include	<stdio.h>
#include	<math.h>


#include	"ViewType.h"

//#define	CHEACK


void
view_euler_angle_set( view_type *vw, float roll, float yaw, float pitch  )
{
vec3d	N,	V;

float	cos_p,	sin_p;
float	cos_y,	sin_y;
float	cos_r,	sin_r;


	cos_p = cos( pitch );
	sin_p = sin( pitch );

	cos_y = cos( yaw );
	sin_y = sin( yaw );

	cos_r = cos( roll );
	sin_r = sin( roll );



	N.x = -sin_y;
	N.y = -cos_y * sin_p;
	N.z = cos_y * cos_p;


	V.x = -sin_r * cos_y;							// r == 0	-->  V.x = 0
	V.y = sin_r * sin_y * sin_p + cos_r * cos_p;	// r == 0   -->  V.y = cos_p
	V.z = -sin_r * sin_y * cos_p + cos_r * sin_p;	// r == 0   -->  V.z = sin_p;



#ifdef CHEACK
	{
		float	nn,	vv,	uu,	nv,	nu,	vu;
		vec3d U;
		U.x = cos_r * cos_y;
		U.y = -cos_r * sin_y * sin_p + sin_r * cos_p;
		U.z = cos_r * sin_y * cos_p + sin_r * sin_p;



		nn = VEC3D_INNER( N, N );
		vv = VEC3D_INNER( V, V );
		uu = VEC3D_INNER( U, U );


		nv = VEC3D_INNER( N, V );
		nu = VEC3D_INNER( N, U );
		vu = VEC3D_INNER( V, U );
	}
#endif

	N.x = -N.x;
	N.y = -N.y;
	N.z = -N.z;

	view_per_set( vw, &vw->cp, 1, &N, 1, &V, 1 );

}


void
view_euler_angle( view_type *vw, float *roll, float *yaw, float *pitch  )
{
float	cos_p;
vec3d	N;

	N.x = -vw->N.x;
	N.y = -vw->N.y;
	N.z = -vw->N.z;

	
	*roll = atan2( -vw->V.x, vw->U.x );


	*pitch = atan2( -N.y, N.z );


	cos_p = cos( *pitch );
	if( cos_p != 0 )
		*yaw = atan2( -N.x, N.z/cos_p );
	else	// sin( *pitc )	== 1;
		*yaw = atan2( -N.x, -N.y );
}



void
view_euler_angle_set_by_vector_roll( view_type *vw, vec3d *N, float roll )
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


	view_euler_angle_set( vw, roll, yaw, pitch );
}




int
view_force_vertical( view_type *vw )
{
vec3d	V;
float	t;

	V.x = 0;
	V.y = 0;
	V.z = 1.0;

	t = VEC3D_INNER( vw->N, V );
	if( ABS(t) < 0.01 )	return( -1 );


	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &V, 1 );

	return( 1 );
}




void
view_rotate( view_type *vw, float roll, float yaw, float pitch  )
{
float	cosa,	sina;
vec3d	U0,	V0,	N0;
vec3d	U,	V,	N;
vec3d	U1,	V1,	N1;

	U0.x = vw->U.x;
	U0.y = vw->U.y;
	U0.z = -vw->U.z;

	V0.x = vw->V.x;
	V0.y = vw->V.y;
	V0.z = -vw->V.z;

	N0.x = vw->N.x;
	N0.y = vw->N.y;
	N0.z = -vw->N.z;




	cosa = cos( pitch );
	sina = sin( pitch );

	V.x = cosa * V0.x + sina * N0.x;
	V.y = cosa * V0.y + sina * N0.y;
	V.z = cosa * V0.z + sina * N0.z;


	N.x = -sina * V0.x + cosa * N0.x;
	N.y = -sina * V0.y + cosa * N0.y;
	N.z = -sina * V0.z + cosa * N0.z;




	cosa = cos( yaw );
	sina = sin( yaw );

	U.x = cosa * U0.x + sina * N.x;
	U.y = cosa * U0.y + sina * N.y;
	U.z = cosa * U0.z + sina * N.z;


	N1.x = -sina * U0.x + cosa * N.x;
	N1.y = -sina * U0.y + cosa * N.y;
	N1.z = -sina * U0.z + cosa * N.z;




	cosa = cos( roll );
	sina = sin( roll );

	U1.x = cosa * U.x + sina * V.x;
	U1.y = cosa * U.y + sina * V.y;
	U1.z = cosa * U.z + sina * V.z;


	V1.x = -sina * U.x + cosa * V.x;
	V1.y = -sina * U.y + cosa * V.y;
	V1.z = -sina * U.z + cosa * V.z;


	V1.z = -V1.z;
	N1.z = -N1.z;
//	N1.z = -N1.z;
//	V1.z = -V1.z;
	view_per_set( vw, &vw->cp, 1, &N1, 1, &V1, 1 );
}






float
view_euler_pitch_yaw_dpixel( view_type *vw, float pitch  )
{

float	a,	b,	c,	dp,	sina,	cosa;

	a = atan2( vw->uv.x1, vw->dis );
	b = a + pitch;

	sina = sin(b);
	cosa = cos(b);

	c = vw->dis* sina/cosa;

	dp = vw->Upixel/2 *(c -  vw->uv.x1);

	return( dp );


//	fprintf( stderr, "DV  VW: %f   pixel: %f\n", c, d );
}


float
view_euler_roll_dpixel( view_type *vw, float roll, float x, float y )
{

float	x1,	y1,	dxy,	dp,	sina,	cosa;
vec2d	dv;

	sina = sin(roll);
	cosa = cos(roll);

	x1 = cosa * x + sina * y;
	y1 = -sina* x + cosa * y;

	dv.x = x1 - x;
	dv.y = y1 - y;

	dxy = hypot( dv.x, dv.y );

	dp = dxy *  vw->Upixel/(vw->uv.x1 - vw->uv.x0);

	return( dp );
}
