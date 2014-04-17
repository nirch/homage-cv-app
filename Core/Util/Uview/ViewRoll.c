
#include	<stdio.h>
#include	<math.h>


#include	"ViewType.h"




float
view_roll( view_type *vw )
{	
float	roll;

	roll = atan2( -vw->V.x, vw->U.x );


	return( roll );
}



void
view_roll_set( view_type *vw, float angle  )
{
float	roll;

	roll = view_roll( vw );

	view_roll_rotate( vw, angle - roll );
}



/*
float
view_roll( view_type *vw )
{
vec3d	V,	U;
float	t,	x,	y,	r,	r1;

	V.x = 0;
	V.y = 0;
	V.z = 1;

	t = -VEC3D_INNER( V, vw->N );
    V.x += t * vw->N.x;
    V.y += t * vw->N.y;
    V.z += t * vw->N.z;

	t = 1/VEC3D_NORM( V );
	V.x *= t;
	V.y *= t;
	V.z *= t;


	VEC3D_PRODUCT( vw->N, V, U );


	x = VEC3D_INNER( U, vw->U );
	y = VEC3D_INNER( V, vw->U );


	r = atan2( y, x ); 


	r1 = atan2( -vw->V.x, vw->U.x );


	return( r );
}



void
view_roll_set( view_type *vw, float angle  )
{
float	cosa,	sina;
vec3d	V,	U;
float	t;

	V.x = 0;
	V.y = 0;
	V.z = 1;

	t = -VEC3D_INNER( V, vw->N );
    V.x += t * vw->N.x;
    V.y += t * vw->N.y;
    V.z += t * vw->N.z;

	t = 1/VEC3D_NORM( V );
	V.x *= t;
	V.y *= t;
	V.z *= t;


	VEC3D_PRODUCT( vw->N, V, U );



	cosa = cos( angle );
	sina = sin( angle );

	vw->V.x = -sina * U.x + cosa * V.x;
	vw->V.y = -sina * U.y + cosa * V.y;
	vw->V.z = -sina * U.z + cosa * V.z;



	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &vw->V, 1 );
}
*/


void
view_roll_rotate( view_type *vw, float angle  )
{
float	cosa,	sina;
vec3d	V;

	cosa = cos( angle );
	sina = sin( angle );

	V.x = -sina * vw->U.x + cosa * vw->V.x;
	V.y = -sina * vw->U.y + cosa * vw->V.y;
	V.z = -sina * vw->U.z + cosa * vw->V.z;



	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &V, 1 );
}



/*
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



void
view_euler_angle( view_type *vw, float *roll, float *yaw, float *pitch  )
{
float	r,	r1;
float	ar,	ar1;
float	cosp;

	*roll = r = view_roll( vw );
	
	*roll = atan2( -vw->V.x, vw->U.x );

	*pitch = atan2( -vw->N.y, -vw->N.z );


	cosp = cos( *pitch );
	if( cosp != 0 ){
		*yaw = atan2( -vw->N.x, -vw->N.z/cosp );
		return;
	}
	

	*yaw = asin( vw->N.x );
}



void
view_euler_angle_set( view_type *vw, float roll, float yaw, float pitch  )
{

	vw->N.x = 0;
	vw->N.y = 0;
	vw->N.z = -1;


	vw->V.x = 0;
	vw->V.y = 1;
	vw->V.z = 0;

	vw->U.x = 1;
	vw->U.y = 0;
	vw->U.z = 0;

	
	view_rotate( vw, roll, yaw, pitch  );
}

*/