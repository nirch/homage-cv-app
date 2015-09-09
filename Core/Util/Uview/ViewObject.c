#include	<math.h>

#include	"Uigp/igp.h"
#include	"Uview/ViewType.h"
#include	"Uconfig3d/Config3d.h"


void
view_object_shift( view_type *bvw, vec3d *P, float dn, float duv, config3d *cm,
					view_type *vw, vec3d *dp )
{
float	t,	cosu,	sinu;
vec3d	p,	np,	v;

	*vw = *bvw;

	p.x = vw->cp.x - P->x;
	p.y = vw->cp.y - P->y;
	p.z = vw->cp.z - P->z;

	v.x = VEC3D_INNER( p, vw->U );
	v.y = VEC3D_INNER( p, vw->V );
	v.z = VEC3D_INNER( p, vw->N );

/*
	np.x = P->x +  dn*cm->y*vw->N.x + duv*cm->x*vw->U.x + duv*cm->z*vw->V.x;
	np.y = P->y +  dn*cm->y*vw->N.y + duv*cm->x*vw->U.y + duv*cm->z*vw->V.y;
	np.z = P->z +  dn*cm->y*vw->N.z + duv*cm->x*vw->U.z + duv*cm->z*vw->V.z;
*/

	dp->x = dn*cm->y*vw->N.x + duv*cm->x*vw->U.x + duv*cm->z*vw->V.x;
	dp->y = dn*cm->y*vw->N.y + duv*cm->x*vw->U.y + duv*cm->z*vw->V.y;
	dp->z = dn*cm->y*vw->N.z + duv*cm->x*vw->U.z + duv*cm->z*vw->V.z;

	np.x = P->x + dp->x;
	np.y = P->y + dp->y;
	np.z = P->z + dp->z;


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




	vw->cp.x = np.x + v.x*vw->U.x + v.y*vw->V.x + v.z*vw->N.x;
	vw->cp.y = np.y + v.x*vw->U.y + v.y*vw->V.y + v.z*vw->N.y;
	vw->cp.z = np.z + v.x*vw->U.z + v.y*vw->V.z + v.z*vw->N.z;


	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &vw->V, 1 );
}
