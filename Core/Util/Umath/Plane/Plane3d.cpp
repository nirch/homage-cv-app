
#include	<stdio.h>
#include	<math.h>

#include "Uigp/igp.h"


#include "Umath/Matrix2Type.h"

#include "Plane3d.h"


int
plna3d_cut_line( plane3d_type *A, plane3d_type *B, vec3d_type *p, vec3d_type *n )
{
matrix2_type	m;
vec2d_type		D,	X;
float	t,	s;

	VEC3D_PRODUCT( A->n, B->n, *n );
	VEC3D_NORMAL( *n );


	m.a00 = VEC3D_INNER( A->n, A->n );

	m.a01 = m.a10 = VEC3D_INNER( A->n, B->n );

	m.a11 = VEC3D_INNER( B->n, B->n );

	D.x = A->d;
	D.y = B->d;

	matrix2S_solve( &m, &D, &X );

	t = m.a00 *X.x + m.a01 * X.y - D.x;
	s = m.a10 *X.x + m.a11 * X.y - D.y;



	t = X.x;
	s = X.y;

	p->x = t * A->n.x + s * B->n.x;
	p->y = t * A->n.y + s * B->n.y;
	p->z = t * A->n.z + s * B->n.z;


// Test
	t = VEC3D_INNER( A->n, *p ) - A->d;
	s = VEC3D_INNER( B->n, *p ) - B->d;

	vec3d_type	p1;
	p1.x = p->x + 10*n->x;
	p1.y = p->y + 10*n->y;
	p1.z = p->z + 10*n->z;
	t = VEC3D_INNER( A->n, *p ) - A->d;
	s = VEC3D_INNER( B->n, *p ) - B->d;





	return( 1 );
}




