/***************************
 ***   ViewPerNormal.c   ***
 ***************************/

#include	"Uvec/Vec3d.h"
#include	"Ubox/Box2d.h"
#include	"Ujet2/Jet2Type.h"
#include	"Ujet2/Jet23Type.h"

#include	"ViewType.h"


void
view_per_normal_base( view_type *bv, view_type *nbv )
{

	*nbv = *bv;

	nbv->cp.x = 0;
	nbv->cp.y = 0;
	nbv->cp.z = ( nbv->type == VIEW_PERSPECTIVE ) ? bv->dis : 0;


	VEC3D_SET( nbv->N, 0, 0, -1 );
	VEC3D_SET( nbv->V, 0, 1, 0 );
	VEC3D_SET( nbv->U, 1, 0, 0 );
}


//#pragma optimize( "", off )
void
view_per_normal( view_type *base, view_type *vw, view_type *nvw )
{
vec3d	dr;

	*nvw = *vw;

	nvw->N.x = VEC3D_INNER( base->U, vw->N );
	nvw->N.y = VEC3D_INNER( base->V, vw->N );
	nvw->N.z = -VEC3D_INNER( base->N, vw->N );

	nvw->V.x = VEC3D_INNER( base->U, vw->V );
	nvw->V.y = VEC3D_INNER( base->V, vw->V );
	nvw->V.z = -VEC3D_INNER( base->N, vw->V );


	dr.x = vw->cp.x - base->cp.x;
	dr.y = vw->cp.y - base->cp.y;
	dr.z = vw->cp.z - base->cp.z;

	nvw->cp.x = VEC3D_INNER( base->U, dr );
	nvw->cp.y = VEC3D_INNER( base->V, dr );
	nvw->cp.z = -VEC3D_INNER( base->N, dr ) + base->dis; // since normal cp = (0, 0, 1 )


	view_per_set( nvw, &nvw->cp, 1, &nvw->N, 1, &nvw->V, 1 );
}
//#pragma optimize( "", on )
