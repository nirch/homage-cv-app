/**************************
 ***	ViewCreate.c	***
 *************************/

#include	<math.h>

#include	"Uigp/igp.h"
#include	"Uview/ViewType.h"



static void	view_create_normal_vec( vec3d *cp, float Lv,
						vec2d *n, float *lv );


void
view_create( view_type *vw, vec3d *cp, float Lv, int row, int column )
{
float	lv,	lu,	t;
vec2d	n;

	vw->type = VIEW_PERSPECTIVE;
	vw->Upixel = column;
	vw->Vpixel = row;


	vw->cp.x = 0;
	vw->cp.y = cp->y;
	vw->cp.z = cp->z;
	vw->dis = 1;


	view_create_normal_vec( cp, Lv, &n, &lv );

	vw->N.x = 0;
	vw->N.y = n.x;
	vw->N.z = n.y;

	t = 1/ VEC3D_NORM( vw->N );
	vw->N.x *= t;
	vw->N.y *= t;
	vw->N.z *= t;


	if( ABS( vw->N.z ) < 0.8 ){
		vw->V.x = 0;
		vw->V.y = 0;
		vw->V.z = 1;
	}
	else	{
		vw->V.x = 0;
		vw->V.y = 1;
		vw->V.z = 0;
	}



	vw->uv.y0 = -0.5 *lv;
	vw->uv.y1 = 0.5 *lv;

	lu = lv * ((float)vw->Upixel/ (float)vw->Vpixel);
	vw->uv.x0 = -0.5 *lu;
	vw->uv.x1 = 0.5 *lu;


	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &vw->V, 1 );
	view_set_pixel( vw, row, column );
}



static void
view_create_normal_vec( vec3d *cp, float Lv, vec2d *n, float *lv )
{
vec2d	v0,	v1;
float	t;

	v0.x = -Lv - cp->y;
	v0.y = 0 - cp->z;

	t = hypot( v0.x, v0.y );
	v0.x /= t;
	v0.y /= t;


	v1.x = Lv - cp->y;
	v1.y = 0 - cp->z;

	t = hypot( v1.x, v1.y );
	v1.x /= t;
	v1.y /= t;



	n->x = v0.x + v1.x;
	n->y = v0.y + v1.y;

	t = hypot( n->x, n->y );
	n->x /= t;
	n->y /= t;


	t = 1.0 / ( v1.x *n->x + v1.y *n->y );
	*lv = 2.0 * sqrt( t*t -1 );
}

