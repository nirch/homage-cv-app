/**********************
 ***   ViewTool.c   ***
 **********************/

#include	<math.h>

#include	"Uigp/igp.h"
#include	"ViewType.h"


void
view_3d_point( view_type *vw, float u, float v, vec3d *p )
{
	p->x = vw->cp.x + u* vw->U.x + v* vw->V.x + vw->dis*vw->N.x;
	p->y = vw->cp.y + u* vw->U.y + v* vw->V.y + vw->dis*vw->N.y;
	p->z = vw->cp.z + u* vw->U.z + v* vw->V.z + vw->dis*vw->N.z;
}


void
view_3d_pointD( view_type *vw, float u, float v, float depth, vec3d *p )
{

	p->x = vw->cp.x + depth*( u* vw->U.x + v* vw->V.x + vw->dis* vw->N.x );
	p->y = vw->cp.y + depth*( u* vw->U.y + v* vw->V.y + vw->dis* vw->N.y );
	p->z = vw->cp.z + depth*( u* vw->U.z + v* vw->V.z + vw->dis* vw->N.z );
}



void
view_3d_vector( view_type *vw, float u, float v, vec3d *n )
{
	if( vw->type == VIEW_PARALLEL ){
		*n = vw->N;
		return;
	}

	n->x = vw->dis* vw->N.x + u* vw->U.x + v* vw->V.x;
	n->y = vw->dis* vw->N.y + u* vw->U.y + v* vw->V.y;
	n->z = vw->dis* vw->N.z + u* vw->U.z + v* vw->V.z;
}



void
view_3d_ray( view_type *vw, float u, float v, vec3d *p, vec3d *n )
{
	if( vw->type == VIEW_PARALLEL ){
		*n = vw->N;

		p->x = vw->cp.x + vw->dis*vw->N.x + u* vw->U.x + v* vw->V.x;
		p->y = vw->cp.y + vw->dis*vw->N.y + u* vw->U.y + v* vw->V.y;
		p->z = vw->cp.z + vw->dis*vw->N.z + u* vw->U.z + v* vw->V.z;

		return;
	}


	n->x = vw->dis* vw->N.x + u* vw->U.x + v* vw->V.x;
	n->y = vw->dis* vw->N.y + u* vw->U.y + v* vw->V.y;
	n->z = vw->dis* vw->N.z + u* vw->U.z + v* vw->V.z;

	p->x = vw->cp.x + n->x;
	p->y = vw->cp.y + n->y;
	p->z = vw->cp.z + n->z;



}




void
view_cut_point( view_type *vw, vec3d *p, vec3d *p1, vec2d *v )
{
vec3d	n;

	n.x = p1->x - p->x;
	n.y = p1->y - p->y;
	n.z = p1->z - p->z;

	view_cut_point_n( vw, p, &n, v );
}




void
view_cut_point_n( view_type *vw, vec3d *p, vec3d *n, vec2d *v )
{
float	t;
vec3d	v3,	lp;

	lp.x = p->x - vw->cp.x;
	lp.y = p->y - vw->cp.y;
	lp.z = p->z - vw->cp.z;

	t = ( vw->dis  - VEC3D_INNER( vw->N, lp ) )/ VEC3D_INNER( vw->N, *n );


	v3.x = lp.x + t*n->x;
	v3.y = lp.y + t*n->y;
	v3.z = lp.z + t*n->z;


	v->x = VEC3D_INNER( vw->U, v3 );
	v->y = VEC3D_INNER( vw->V, v3 );
}



void
view_actoal_box( view_type *base, view_type *vw, box2d *box )
{
vec3d	n,	p;
vec2d	v;

	view_3d_ray( vw, vw->uv.x0, vw->uv.y0, &p, &n );
	view_cut_point_n( base, &p, &n, &v );
	BOX2D_INIT( *box, v.x, v.y );


	view_3d_ray( vw, vw->uv.x0, vw->uv.y1, &p, &n );
	view_cut_point_n( base, &p, &n, &v );
	BOX2D_UPDATE( *box, v.x, v.y );
 
	view_3d_ray( vw, vw->uv.x1, vw->uv.y0, &p, &n );
	view_cut_point_n( base, &p, &n, &v );
	BOX2D_UPDATE( *box, v.x, v.y );

	view_3d_ray( vw, vw->uv.x1, vw->uv.y1, &p, &n );
	view_cut_point_n( base, &p, &n, &v );
	BOX2D_UPDATE( *box, v.x, v.y );
}


void
view_actoal_pixel_box( view_type *base, view_type *vw, box2d *box )
{
vec3d	n,	p;
vec2d	v,	pv;

	view_3d_vector( vw, vw->uv.x0, vw->uv.y0, &n );
	view_3d_point( vw, vw->uv.x0, vw->uv.y0, &p );
	view_cut_point_n( base, &p, &n, &v );
	pv.y = base->Ua*v.x + base->Ub;
	pv.x = base->Va*v.y + base->Vb;
	BOX2D_INIT( *box, pv.x, pv.y );


	view_3d_vector( vw, vw->uv.x0, vw->uv.y1, &n );
	view_3d_point( vw, vw->uv.x0, vw->uv.y1, &p );
	view_cut_point_n( base, &p, &n, &v );
	pv.y = base->Ua*v.x + base->Ub;
	pv.x = base->Va*v.y + base->Vb;
	BOX2D_UPDATE( *box, pv.x, pv.y );
 
	view_3d_vector( vw, vw->uv.x1, vw->uv.y0, &n );
	view_3d_point( vw, vw->uv.x1, vw->uv.y0, &p );
	view_cut_point_n( base, &p, &n, &v );
	pv.y = base->Ua*v.x + base->Ub;
	pv.x = base->Va*v.y + base->Vb;
	BOX2D_UPDATE( *box, pv.x, pv.y );

	view_3d_vector( vw, vw->uv.x1, vw->uv.y1, &n );
	view_3d_point( vw, vw->uv.x1, vw->uv.y1, &p );
	view_cut_point_n( base, &p, &n, &v );
	pv.y = base->Ua*v.x + base->Ub;
	pv.x = base->Va*v.y + base->Vb;
	BOX2D_UPDATE( *box, pv.x, pv.y );
}


void
view_project_point( view_type *vw, vec3d *p, vec2d *v )
{
vec3f_type	dv;
float	t,	d;

	dv.x = p->x - vw->cp.x;
	dv.y = p->y - vw->cp.y;
	dv.z = p->z - vw->cp.z;


	if( vw->type == VIEW_PARALLEL ){
		v->x = VEC3D_INNER( vw->U, dv );
		v->y = VEC3D_INNER( vw->V, dv );

		return;
	}

	d = VEC3D_INNER( vw->N, dv );
	t = vw->dis / d;

	v->x = VEC3D_INNER( vw->U, dv ) * t;
	v->y = VEC3D_INNER( vw->V, dv ) * t;
}


float
view_project_depth( view_type *vw, float x0, float y0, float z0 )
{
vec3d	v;
float	depth;


	v.x = x0 - vw->cp.x;
	v.y = y0 - vw->cp.y;
	v.z = z0 - vw->cp.z;

	depth = v.x*v.x + v.y*v.y + v.z*v.z;

	return( depth );
}


void
view_project_2pixel( view_type *vw, vec3d *p, vec2d *v )
{
float	t;
vec3d	v3;
vec2d	vp;

	v3.x = p->x - vw->cp.x;
	v3.y = p->y - vw->cp.y;
	v3.z = p->z - vw->cp.z;


	if( vw->type == VIEW_PARALLEL ){
		v->x = VEC3D_INNER( vw->U, v3 );
		v->y = VEC3D_INNER( vw->V, v3 );

		return;
	}



	t = VEC3D_INNER( vw->N, v3 );
	t = 1.0 / t;

	vp.x = t* VEC3D_INNER( vw->U, v3 );
	vp.y = t* VEC3D_INNER( vw->V, v3 );


	v->x = vw->Va *vp.y + vw->Vb;
	v->y = vw->Ua *vp.x + vw->Ub;
}


void
view_vp2pixel( view_type *vw, vec2d *vp, vec2d *p )
{
	p->x = vw->Va *vp->y + vw->Vb;
	p->y = vw->Ua *vp->x + vw->Ub;
}


void
view_pixel2vp( view_type *vw, vec2d *p, vec2d *vp )
{
	vp->x = vw->Ua_inv*(p->y - vw->Ub);
	vp->y = vw->Va_inv*(p->x - vw->Vb);
}

/*
void
view_set_XY( view_type *vw, float x, float y, float z )
{

	vw->type = VIEW_PARALLEL;
	VEC3D_SET( vw->rp, x, y, 0 );
	VEC3D_SET( vw->cp, x, y, z );
	vw->dis = z;

	VEC3D_SET( vw->N, 0, 0, -1 );
	VEC3D_SET( vw->V, 0, 1, 0 );
	VEC3D_SET( vw->U, 1, 0, 0 );

	BOX2D_SET( vw->uv, -1.0, 1.0, -1.0, 1.0 );
}
*/


void
view_pixel2plane_xyz( view_type *vw, vec2d *p, vec3d *P, vec3d *N, vec3d *p3 )
{
vec2d   v;
vec3d   n,      p33;
float   t,      D;

	view_pixel2vp( vw, p, &v );


	view_3d_ray( vw, v.x, v.y, &p33, &n );

	D = VEC3D_INNER( *P, *N );
	t = ( D - VEC3D_INNER( vw->cp, *N ) ) / VEC3D_INNER( n, *N );

	p3->x = vw->cp.x + t *n.x;
	p3->y = vw->cp.y + t *n.y;
	p3->z = vw->cp.z + t *n.z;
}

void
view_pixel2planeD_xyz( view_type *vw, vec2d *p, vec3d *N, float D, vec3d *p3 )
{
vec2d   v;
vec3d   n,      p33;
float   t;

	view_pixel2vp( vw, p, &v );


	view_3d_ray( vw, v.x, v.y, &p33, &n );
/*
	D = VEC3D_INNER( *P, *N );
*/
	t = ( D - VEC3D_INNER( vw->cp, *N ) ) / VEC3D_INNER( n, *N );

	p3->x = vw->cp.x + t *n.x;
	p3->y = vw->cp.y + t *n.y;
	p3->z = vw->cp.z + t *n.z;
}


void
view_transform_vector( view_type *vw, vec3d *v, view_type *nvw, vec3d *nv )
{
vec3d	dp;

	dp.x = vw->U.x * v->x + vw->U.y * v->y + vw->U.z * v->z;
	dp.y = vw->V.x * v->x + vw->V.y * v->y + vw->V.z * v->z;
	dp.z = vw->N.x * v->x + vw->N.y * v->y + vw->N.z * v->z;

	nv->x = dp.x * nvw->U.x + dp.y * nvw->V.x + dp.z * nvw->N.x;
	nv->y = dp.x * nvw->U.y + dp.y * nvw->V.y + dp.z * nvw->N.y;
	nv->z = dp.x * nvw->U.z + dp.y * nvw->V.z + dp.z * nvw->N.z;
}


void
view_transform_point( view_type *vw, vec3d *P, view_type *nvw, vec3d *nP )
{
vec3d	dp,	p;

	p.x = P->x - vw->cp.x;
	p.y = P->y - vw->cp.y;
	p.z = P->z - vw->cp.z;

	dp.x = vw->U.x * p.x + vw->U.y * p.y + vw->U.z * p.z;
	dp.y = vw->V.x * p.x + vw->V.y * p.y + vw->V.z * p.z;
	dp.z = vw->N.x * p.x + vw->N.y * p.y + vw->N.z * p.z;

	p.x = dp.x * nvw->U.x + dp.y * nvw->V.x + dp.z * nvw->N.x;
	p.y = dp.x * nvw->U.y + dp.y * nvw->V.y + dp.z * nvw->N.y;
	p.z = dp.x * nvw->U.z + dp.y * nvw->V.z + dp.z * nvw->N.z;

	nP->x = p.x + nvw->cp.x;
	nP->y = p.y + nvw->cp.y;
	nP->z = p.z + nvw->cp.z;

}



int
view_centering( view_type *vw )
{
box2d	*uv;
float	t;
int	ret;

	ret = 0;

	uv = &vw->uv;

	t = ( uv->x0 + uv->x1 )/2;
	if( t != 0 ){
		uv->x1 -= t;
		uv->x0 -= t;
		ret = 1;
	}

	t = ( uv->y0 + uv->y1 )/2;
	if( t != 0 ){
		uv->y1 -= t;
		uv->y0 -= t;
		ret = 1;
	}

	view_set_pixel( vw, vw->Vpixel, vw->Upixel );

	return( ret );
}