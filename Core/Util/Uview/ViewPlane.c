/************************
 ***	ViewPlane.c   ***
 ************************/
#include	<stdio.h>
#include	<math.h>


#include	"ViewType.h"



void
view_plane_g2v( view_type *vw,
		vec3d *N, vec3d *P, plane3f_type *plane )
{
vec3d	p1,	p2;
 
	plane->n.x = VEC3D_INNER( vw->U, *N );
	plane->n.y = VEC3D_INNER( vw->V, *N );
	plane->n.z = -VEC3D_INNER( vw->N, *N );
 
	p1.x = P->x - vw->cp.x;
	p1.y = P->y - vw->cp.y;
	p1.z = P->z - vw->cp.z;
 
	p2.x = VEC3D_INNER( vw->U, p1 );
	p2.y = VEC3D_INNER( vw->V, p1 );
	p2.z = -VEC3D_INNER( vw->N, p1 );
 
	plane->d =  -VEC3D_INNER( p2, plane->n );
} 


void
view_plane_v2pixel( view_type *vw, plane3f_type *pl )
{
vec3d	n;

	n = pl->n;
	pl->n.x = vw->Va_inv*n.y;
	pl->n.y = vw->Ua_inv* n.x;
	pl->n.z = n.z + vw->Ua_inv *vw->Ub*n.x + vw->Va_inv*vw->Vb*n.y;
}

