/**********************************
 ***	ViewCreateNormal.c	***
 **********************************/

#include	<math.h>

#include	"Uigp/igp.h"
#include	"Uview/ViewType.h"



void
view_create_noraml( view_type *vw, float rf, int row, int column )
{
	vw->type = VIEW_PERSPECTIVE;
	vw->Upixel = column;
	vw->Vpixel = row;

	vw->cp.x = 0;
	vw->cp.y = 0;
	vw->cp.z = 1.0;


	vw->dis = 1.0;


	VEC3D_SET( vw->N, 0, 0, -1 );
	VEC3D_SET( vw->V, 0, 1, 0 );
	VEC3D_SET( vw->U, 1, 0, 0 );



	vw->uv.y0 = -0.5 * rf * vw->Vpixel;
	vw->uv.y1 =  0.5 * rf * vw->Vpixel;

	vw->uv.x0 = -0.5 * rf * vw->Upixel;
	vw->uv.x1 =  0.5 * rf * vw->Upixel;

	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &vw->V, 1 );
	view_set_pixel( vw, row, column );
}




void
view_init_normal( view_type *vw, int width, int height, int worldUnit, float w0, float h0 )
{
	vw->type = VIEW_PERSPECTIVE;
	vw->Upixel = width;
	vw->Vpixel = height;

	vw->cp.x = 0;
	vw->cp.y = 0;
	vw->cp.z = 1.0;


	vw->dis = 1.0;


	VEC3D_SET( vw->N, 0, 0, -1 );
	VEC3D_SET( vw->V, 0, 1, 0 );
	VEC3D_SET( vw->U, 1, 0, 0 );



	vw->uv.y0 = (h0 - 0.5 * vw->Vpixel) / worldUnit;
	vw->uv.y1 = (h0 + 0.5 * vw->Vpixel) / worldUnit;

	vw->uv.x0 = -(w0 + 0.5 * vw->Upixel) / worldUnit;
	vw->uv.x1 = -(w0 - 0.5 * vw->Upixel) / worldUnit;

	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &vw->V, 1 );
	view_set_pixel( vw, height, width );
}
