
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#include	"Uigp/igp.h"
#include	"Umath/Matrix3Type.h"
#include	"PcaType.h"



int
pca_vec3f( vec3f_type ap[], int nP, vec3d_type *a, matrix3_type *em, double *ev )
{
	matrix3_type m;
	vec3f_type *p;
	int	i,	ret;

	matrix3_zero( &m );
	a->x = a->y = a->z = 0;


	for( i = 0 ; i < nP ; i++ ){
		p = &ap[i];

		m.a00 += p->x*p->x;
		m.a01 += p->x*p->y;
		m.a02 += p->x*p->z;

		
		m.a11 += p->y*p->y;
		m.a12 += p->y*p->z;

		m.a22 += p->z*p->z;

		a->x += p->x;
		a->y += p->y;
		a->z += p->z;
	}

	a->x /= nP;
	a->y /= nP;
	a->z /= nP;


	m.a00 = m.a00 / nP - a->x*a->x;
	m.a01 = m.a01 / nP - a->x*a->y;
	m.a02 = m.a02 / nP - a->x*a->z;


	m.a11 = m.a11 / nP - a->y*a->y;
	m.a12 = m.a12 / nP - a->y*a->z;

	m.a22  = m.a22 / nP - a->z*a->z;

	
	m.a10 = m.a01;
	m.a20 = m.a02;
	m.a21 = m.a12;


	ret = matrixS3_eigen( &m, em, ev );


	return( ret );
}	

