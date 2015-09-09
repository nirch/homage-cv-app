
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif

#include	"Uigp/igp.h"
#include	"Utime\GpTime.h"
#include	"Ulog/Log.h"


#include "Vl2fType.h"	





static int	vl2fA_principal_direction_1( vl2fA_type *avl, float aT, vl2f_type *bvl, vl2f_type *rvl );


int
vl2fA_principal_direction( vl2fA_type *avl, vl2f_type *rvl )
{
int	i;

	float	aT = cos( ANGLE_D2R(5.0) );

	int iMax = -1;
	for( i = 0 ; i < avl->nA ; i++ ){
		vl2f_type	tvl;

		vl2fA_principal_direction_1( avl, aT, &avl->a[i], &tvl );

		if( iMax < 0 || rvl->d < tvl.d ){
			iMax = i;
			*rvl = tvl;
		}
	}

	

	return( iMax );
}



static int
vl2fA_principal_direction_1( vl2fA_type *avl, float aT, vl2f_type *bvl, vl2f_type *rvl )
{
	int	i,	n;

	vec2f_type bu;
	VEC2D_RIGHT( bvl->v, bu );

	float	t0,	t1,	u0,	u1;
	for( i = 0, n =  0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];


		float a = VEC2D_INNER( bvl->v, vl->v );
		if( ABS(a) < aT )	continue;

		n++;

		vec2f_type	p;
		p.x = vl->p.x - bvl->p.x;
		p.y = vl->p.y - bvl->p.y;

		float t = VEC2D_INNER( bvl->v, p );
		float u = VEC2D_INNER( bu, p );

		if( n == 1 ){
			t1 = t + vl->d;
			t0 = t - vl->d;

			u0 = u1 = u;
			continue;
		}

		if( t-vl->d < t0 )	t0 = t-vl->d;
		if( t+vl->d > t1 )	t1 = t+vl->d;

		if( u < u0 )	u0 = u;
		if( u > u1 )	u1 = u;
	}

	float t = (t1 + t0)/2;
	float u = (u1 + u0)/2;

	*rvl = *bvl;
	rvl->p.x += t*rvl->v.x + u*bu.x;
	rvl->p.y += t*rvl->v.y + u*bu.y;

	rvl->d = (t1 - t0 )/2;
	rvl->e = ( u1 - u0 )/2;

	return( 1 );
}
			

