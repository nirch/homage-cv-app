#include	<math.h>

#include	"Uigp/igp.h"
#include	"Uview/ViewType.h"
#include	"Uconfig3d/Config3d.h"


void
view_camera_shift( view_type *bvw, float dn, float duv, config3d *cm,
							view_type *vw )
{
float	t,	cosu,	sinu;

	*vw = *bvw;


	vw->cp.x += dn*cm->y*vw->N.x + duv*cm->x*vw->U.x + duv*cm->z*vw->V.x;
	vw->cp.y += dn*cm->y*vw->N.y + duv*cm->x*vw->U.y + duv*cm->z*vw->V.y;
	vw->cp.z += dn*cm->y*vw->N.z + duv*cm->x*vw->U.z + duv*cm->z*vw->V.z;


	cosu = cos( cm->u1*M_PI/180.0 );
	sinu = sin( cm->u1*M_PI/180.0 );
	vw->N.x = cosu * vw->N.x + sinu*vw->U.x;
	vw->N.y = cosu * vw->N.y + sinu*vw->U.y;
	vw->N.z = cosu * vw->N.z + sinu*vw->U.z;

	t = 1/VEC3D_NORM( vw->N );
	vw->N.x *= t;
	vw->N.y *= t;
	vw->N.z *= t;


	cosu = cos( cm->u2*M_PI/180.0 ); 
	sinu = sin( cm->u2*M_PI/180.0 );
	vw->N.x = cosu * vw->N.x + sinu*vw->V.x;
	vw->N.y = cosu * vw->N.y + sinu*vw->V.y;
	vw->N.z = cosu * vw->N.z + sinu*vw->V.z;
 
	t = 1/VEC3D_NORM( vw->N );
	vw->N.x *= t;
	vw->N.y *= t;
	vw->N.z *= t;


	VEC3D_PRODUCT( vw->N, vw->V, vw->U );
	cosu = cos( cm->u3*M_PI/180.0 ); 
	sinu = sin( cm->u3*M_PI/180.0 );
	vw->V.x = cosu * vw->V.x + sinu*vw->U.x;
	vw->V.y = cosu * vw->V.y + sinu*vw->U.y;
	vw->V.z = cosu * vw->V.z + sinu*vw->U.z;

	t = 1/VEC3D_NORM( vw->V );
	vw->V.x *= t;
	vw->V.y *= t;
	vw->V.z *= t;






	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &vw->V, 1 );
}
