/***************************
 ***   ViewActualBox.c   ***
 ***************************/

#include	<math.h>

#include	"Uigp/igp.h"
#include	"ViewType.h"




/*
int
view_cut_point_ray( view_type *vw, float depth, vec3d *p, vec3d *n, vec2d *v )
{
float	t,	ipn;
float	tv,	tu,	t0,	t1;
vec3d	v3;
vec2d	pp,	pn;

	ipn = VEC3D_INNER( vw->N, *n );

	t = ( VEC3D_INNER( vw->N, vw->rp )+depth - VEC3D_INNER( vw->N, *p ) )/ipn;
	if( t >= 0 ){
		v3.x = p->x + t*n->x;
		v3.y = p->y + t*n->y;
		v3.z = p->z + t*n->z;


		view_project_point( vw, &v3, v );

		return( 1 );
	}


	pn.x = VEC3D_INNER( vw->U, *n );
	pn.y = VEC3D_INNER( vw->V, *n );
	v3.x = p->x - vw->rp.x;
	v3.y = p->y - vw->rp.y;
	v3.z = p->z - vw->rp.z;

	pp.x = VEC3D_INNER( vw->U, v3 );
	pp.y = VEC3D_INNER( vw->V, v3 );


	tu = 10000;
	if( pn.x != 0 ){
		t0 = (vw->uv.x0 - pp.x )/pn.x;
		t1 = (vw->uv.x1 - pp.x )/pn.x;
		tu = MAX( t0, t1 );
		if( tu < 0 )	tu = 10000;
	}


	tv = 10000;
	if( pn.y != 0 ){
		t0 = (vw->uv.y0 - pp.y )/pn.y;
		t1 = (vw->uv.y1 - pp.y )/pn.y;
		tv = MAX( t0, t1 );
		if( tv < 0 )	tv = 10000;
	}

	t = MIN( tu, tv );
	if( t < 0 || t == 10000 )	return( 0 );


	v->x = pp.x + t * pn.x;
	v->y = pp.y + t * pn.y;

	return( 2 );
}

void
view_actoal_depth_box( view_type *base, float depth,
				view_type *vw, box2d *box )
{
vec3d	n,	p;
vec2d	v;

	view_3d_ray( vw, vw->uv.x0, vw->uv.y0, &p, &n );
	view_cut_point_ray( base, depth, &p, &n, &v );
	BOX2D_INIT( *box, v.x, v.y );


	view_3d_ray( vw, vw->uv.x0, vw->uv.y1, &p, &n );
	view_cut_point_ray( base, depth, &p, &n, &v );
	BOX2D_UPDATE( *box, v.x, v.y );
 
	view_3d_ray( vw, vw->uv.x1, vw->uv.y0, &p, &n );
	view_cut_point_ray( base, depth, &p, &n, &v );
	BOX2D_UPDATE( *box, v.x, v.y );

	view_3d_ray( vw, vw->uv.x1, vw->uv.y1, &p, &n );
	view_cut_point_ray( base, depth, &p, &n, &v );
	BOX2D_UPDATE( *box, v.x, v.y );
}




void
view_actoal_depth_pixel_box( view_type *base, float depth0, float depth1,
						view_type *vw, box2d *box )
{
box2d	box0,	box1;

	view_actoal_depth_box( base, depth0, vw, &box0 );
	view_actoal_depth_box( base, depth1, vw, &box1 );

	BOX2D_UPDATE( box0, box1.x0, box1.y0 );
	BOX2D_UPDATE( box0, box1.x1, box1.y1 );

	box->y0 = base->Ua*box0.x0 + base->Ub;
	box->x0 = base->Va*box0.y1 + base->Vb;

	box->y1 = base->Ua*box0.x1 + base->Ub;
	box->x1 = base->Va*box0.y0 + base->Vb;
}
*/



