
#include	<stdio.h>
#include	<math.h>


#include	"ViewType.h"




void
view_set_config( view_type *vw, config3d *dc )
{

	vw->cp.x = dc->x;
	vw->cp.y = dc->y;
	vw->cp.z = dc->z;


	view_euler_angle_set( vw, dc->u1, dc->u2, dc->u3  );
}



void
view_set_configD( view_type *vw, config3d *dc, view_type *nvw )
{
float	roll,	yaw,	pitch;

	if( vw != NULL && vw != nvw )
		*nvw = *vw;

	nvw->cp.x += dc->x;
	nvw->cp.y += dc->y;
	nvw->cp.z += dc->z;



	view_euler_angle( nvw, &roll, &yaw, &pitch );
	view_euler_angle_set( nvw, dc->u1+roll, dc->u2+yaw, dc->u3+pitch  );
}


void
view_set_configV( view_type *vw, vec3d *VR, config3d *c )
{

	vw->cp.x = c->x;
	vw->cp.y = c->y;
	vw->cp.z = c->z;


	view_eulerV_set( vw, VR, c->u1, c->u2, c->u3  );
}



void
view_set_configVD( view_type *vw, vec3d *VR, config3d *dc, view_type *nvw )
{
float	roll,	yaw,	pitch;

	if( vw != NULL && vw != nvw )
		*nvw = *vw;

	nvw->cp.x += dc->x;
	nvw->cp.y += dc->y;
	nvw->cp.z += dc->z;



	view_eulerV( nvw, VR, &roll, &yaw, &pitch );
	view_eulerV_set( nvw, VR, dc->u1+roll, dc->u2+yaw, dc->u3+pitch  );
}



