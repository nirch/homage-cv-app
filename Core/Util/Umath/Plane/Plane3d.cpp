
#include	<stdio.h>
#include	<math.h>

#include "Uigp/igp.h"


#include "Umath/Matrix2Type.h"

#include "Plane3d.h"


int
plane3d_cut_line( plane3d_type *A, plane3d_type *B, vec3d_type *p, vec3d_type *n )
{
matrix2_type	m;
vec2d_type		D,	X;
float	t,	s;

	VEC3D_PRODUCT( A->n, B->n, *n );
	VEC3D_NORMALIZE( *n );


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




// compute the cut point of   plane and the vectot cp + t*v
int
plane3d_cut_point( plane3d_type *pl, vec3d_type *cp, vec3d_type *v, vec3d_type *p )
{
double	t,	d;

	d = VEC3D_INNER( pl->n, *v );


	t = (pl->d - VEC3D_INNER( *cp, pl->n ))/ VEC3D_INNER( pl->n, *v );

	p->x = cp->x + t* v->x;
	p->y = cp->y + t* v->y;
	p->z = cp->z + t* v->z;

	return( 1 );
}



// find point on plane P + t*V + s*U   nearest to all the plane
int
plane3d_nearset_point( plane3d_type ap[], int nP, vec3d_type *P, vec3d_type *V, vec3d_type *U, vec3d_type *p )
{
	matrix2_type	m;
	vec2d_type	D,	X;
	double	t,	s;
	float	A,	B,	C;
	int	i;

	matrix2_zero( &m );
	D.x = D.y = 0;

	for(i = 0 ; i < nP ; i++ ){
		plane3d_type	*pn = &ap[i];


		A = VEC3D_INNER( *V, pn->n );
		B = VEC3D_INNER( *U, pn->n );


		C = VEC3D_INNER( *P, pn->n );


		m.a00 += A*A;
		m.a01 += B*A;

		m.a10 += A*B;
		m.a11 += B*B;

		D.x += (-C + pn->d)*A;
		D.y += (-C + pn->d)*B;
	}

	if( matrix2S_solve( &m, &D, &X ) < 0 )
		return( -1 );

	t = X.x;
	s = X.y;

	p->x = P->x + t * V->x + s * U->x;
	p->y = P->y + t * V->y + s * U->y;
	p->z = P->z + t * V->z + s * U->z;


	return( 1 );

}