/**********************
 ***	Vec3dTool.c   ***
 **********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Vec3d.h"

#include "Umath/Matrix3Type.h"



int
vec3fA_approximate_plane( vec3fA_type *av, int i0, int i1, vec3f_type *U, vec3f_type *V, vec3f_type *N, vec3f_type *p )
{
matrix3_type	m;
float	x,	y,	z;
double	sx,	sy,	sz;
int	i,	n;
matrix3_type em;
double ev[3];


	matrix3_zero( &m );



	if( i0 < 0 )
		i0 = 0;

	if( i1 > av->nA )
		i1 = av->nA;

	sx = sy = sz = 0;


	for( i = i0, n = 0 ; i < i1 ; i++ ){


		x = av->a[i].x;
		y = av->a[i].y;
		z = av->a[i].z;


		sx += x;
		sy += y;
		sz += z;

		m.a00 += x*x;
		m.a01 += x*y;
		m.a02 += x*z;
		m.a11 += y*y;
		m.a12 += y*z;
		m.a22 += z*z;
		n++;
	}

	sx /= n;
	sy /= n;
	sz /= n;

	m.a00 = m.a00 / n - sx*sx;
	m.a01 = m.a01 / n - sx*sy;
	m.a02 = m.a02 / n - sx*sz;

	m.a11 = m.a11 / n - sy*sy;
	m.a12 = m.a12 / n - sy*sz;

	m.a22 = m.a22 / n - sz*sz;


	m.a10 = m.a01;
	m.a20 = m.a02;
	m.a21 = m.a12;


	p->x = sx;
	p->y = sy;
	p->z = sz;



	matrixS3_eigen( &m, &em, ev );
	U->x = em.a00;
	U->y = em.a10;
	U->z = em.a20;

	V->x = em.a01;
	V->y = em.a11;
	V->z = em.a21;

	N->x = em.a02;
	N->y = em.a12;
	N->z = em.a22;


	vec3fA_approximate_plane_test( av, i0, i1, p, N );


	return( 1 );
}



int
vec3fA_approximate_plane_test( vec3fA_type *av, int i0, int i1, vec3f_type *p, vec3f_type *v )
{
	int	i,	n;
	vec3f_type	dp;
	float	t = 0;

	for( i = i0, n = 0 ; i < i1 ; i++, n++ ){
		dp.x = av->a[i].x  - p->x;
		dp.y = av->a[i].y  - p->y;
		dp.z = av->a[i].z  - p->z;

		t += VEC3D_INNER( dp, *v );


	}

	t /= n;

	return(  t );
}