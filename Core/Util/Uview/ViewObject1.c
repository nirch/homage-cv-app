#include	<math.h>

#include	"Uigp/igp.h"
#include	"Uview/ViewType.h"
#include	"Uconfig3d/Config3d.h"


void	view_object1_test( view_type *vw, view_type *nvw, vec3d *P );


void
view_object_shift1( view_type *bvw, vec3d *P, vec3d *N,
		float dn, float duv, config3d *cm, view_type *vw, vec3d *dp )
{
float	t,	cosu,	sinu;
vec3d	p,	np,	v;
vec3d	U1,	V1;
vec3d	U2,	V2,	N2;
vec3d	Uv,	Vv,	Nv;

	*vw = *bvw;

	t = 1/VEC3D_NORM( *N );
        N->x *= t;
        N->y *= t;
        N->z *= t;

	t = VEC3D_INNER( *N, vw->U );
	U1.x = vw->U.x - t*N->x;
	U1.y = vw->U.y - t*N->y;
	U1.z = vw->U.z - t*N->z;
	t = 1/VEC3D_NORM( U1 );
        U1.x *= t;
        U1.y *= t;
        U1.z *= t;


	t = VEC3D_INNER( *N, vw->V );
	V1.x = vw->V.x - t*N->x;
	V1.y = vw->V.y - t*N->y;
	V1.z = vw->V.z - t*N->z;
	t = 1/VEC3D_NORM( V1 );
        V1.x *= t;
        V1.y *= t;
        V1.z *= t;


	Uv.x = VEC3D_INNER( vw->U, U1 );
	Uv.y = VEC3D_INNER( vw->U, V1 );
	Uv.z = VEC3D_INNER( vw->U, *N );

	Vv.x = VEC3D_INNER( vw->V, U1 );
	Vv.y = VEC3D_INNER( vw->V, V1 );
	Vv.z = VEC3D_INNER( vw->V, *N );


	Nv.x = VEC3D_INNER( vw->N, U1 );
	Nv.y = VEC3D_INNER( vw->N, V1 );
	Nv.z = VEC3D_INNER( vw->N, *N );



	p.x = vw->cp.x - P->x;
	p.y = vw->cp.y - P->y;
	p.z = vw->cp.z - P->z;

	v.x = VEC3D_INNER( p, U1 );
	v.y = VEC3D_INNER( p, V1 );
	v.z = VEC3D_INNER( p, *N );





	dp->x = dn*cm->z*N->x + duv*cm->x*U1.x + duv*cm->y*V1.x;
	dp->y = dn*cm->z*N->y + duv*cm->x*U1.y + duv*cm->y*V1.y;
	dp->z = dn*cm->z*N->z + duv*cm->x*U1.z + duv*cm->y*V1.z;

	np.x = P->x +  dp->x;
	np.y = P->y +  dp->y;
	np.z = P->z +  dp->z;

/*
	np.x = P->x +  dn*cm->z*N->x + duv*cm->x*U1.x + duv*cm->y*V1.x;
	np.y = P->y +  dn*cm->z*N->y + duv*cm->x*U1.y + duv*cm->y*V1.y;
	np.z = P->z +  dn*cm->z*N->z + duv*cm->x*U1.z + duv*cm->y*V1.z;
*/


	cosu = cos( cm->u2*M_PI/180.0 );
	sinu = sin( cm->u2*M_PI/180.0 );
	N2.x = cosu * N->x + sinu*U1.x;
	N2.y = cosu * N->y + sinu*U1.y;
	N2.z = cosu * N->z + sinu*U1.z;

	t = 1/VEC3D_NORM( N2 );
	N2.x *= t;
	N2.y *= t;
	N2.z *= t;


	cosu = cos( cm->u3*M_PI/180.0 ); 
	sinu = sin( cm->u3*M_PI/180.0 );
	N2.x = cosu * N2.x + sinu*V1.x;
	N2.y = cosu * N2.y + sinu*V1.y;
	N2.z = cosu * N2.z + sinu*V1.z;
 
	t = 1/VEC3D_NORM( N2 );
	N2.x *= t;
	N2.y *= t;
	N2.z *= t;


	VEC3D_PRODUCT( N2, V1, U2 );
	cosu = cos( cm->u1*M_PI/180.0 ); 
	sinu = sin( cm->u1*M_PI/180.0 );
	V2.x = cosu * V1.x + sinu*U2.x;
	V2.y = cosu * V1.y + sinu*U2.y;
	V2.z = cosu * V1.z + sinu*U2.z;

	t = 1/VEC3D_NORM( V2 );
	V2.x *= t;
	V2.y *= t;
	V2.z *= t;


	VEC3D_PRODUCT( N2, V2, U2 );



	vw->cp.x = np.x + v.x*U2.x + v.y*V2.x + v.z*N2.x;
	vw->cp.y = np.y + v.x*U2.y + v.y*V2.y + v.z*N2.y;
	vw->cp.z = np.z + v.x*U2.z + v.y*V2.z + v.z*N2.z;


	vw->N.x = Nv.x*U2.x + Nv.y*V2.x + Nv.z*N2.x;
	vw->N.y = Nv.x*U2.y + Nv.y*V2.y + Nv.z*N2.y;
	vw->N.z = Nv.x*U2.z + Nv.y*V2.z + Nv.z*N2.z;

	vw->V.x = Vv.x*U2.x + Vv.y*V2.x + Vv.z*N2.x;
	vw->V.y = Vv.x*U2.y + Vv.y*V2.y + Vv.z*N2.y;
	vw->V.z = Vv.x*U2.z + Vv.y*V2.z + Vv.z*N2.z;


	view_per_set( vw, &vw->cp, 1, &vw->N, 1, &vw->V, 1 );
}




void
view_object1_test( view_type *vw, view_type *nvw, vec3d *P )
{
vec3d	v,	v0;
vec2d	v2,	v3;
vec2d	p2,	p3;

	v.x = P->x - vw->cp.x;
	v.y = P->y - vw->cp.y;
	v.z = P->z - vw->cp.z;
        v0.x = VEC3D_INNER( v, vw->U );
        v0.y = VEC3D_INNER( v, vw->V );
        v0.z = VEC3D_INNER( v, vw->N );

	v.x = P->x - nvw->cp.x;
	v.y = P->y - nvw->cp.y;
	v.z = P->z - nvw->cp.z;
        v0.x = VEC3D_INNER( v, nvw->U );
        v0.y = VEC3D_INNER( v, nvw->V );
        v0.z = VEC3D_INNER( v, nvw->N );


	view_project_point( vw, P, &v2 );
	view_vp2pixel( vw, &v2, &p2 );
	view_project_point( nvw, P, &v3 );
	view_vp2pixel( vw, &v3, &p3 );
}
