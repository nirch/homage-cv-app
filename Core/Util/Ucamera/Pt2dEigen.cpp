/***********************************
 ***    Pt2ApproximateLinePv.c   ***
 ***********************************/


#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"

#include	"Umath/Matrix2Type.h"

#include	"Umath/EigenType.h"


#include	"Ucamera/Pt2dType.h"



int pt2A_eigen( pt2dA_type *apt, int i00, int ni, int cycle, eigen2d_type *e )
{
	int	i;

	eigen2dAux_type	a;
	eigen2dAux_init( &a, 1 );



	int i0 = i00 - ni;
	

	int i1 = i00 + ni+1;
	


	if( cycle == 0 ){
		if( i0 < 0 )	i0 = 0;
		if( i1 > apt->nA )	i1 = apt->nA;
	}
	else {
		if( i0 < 0 )	i0 += apt->nA;
		if( i1 > apt->nA )	i1 -= apt->nA;
	}



	for( i = i0 ; i != i1 ; i++ ){
		if( i >= apt->nA )	i = 0;
	
		pt2d_type	*pt = &apt->a[i];


		EIGEN2D_UPDATE( a, pt->p.x, pt->p.y );

		if( i == i1 )	break;
	}

	if( a.n < 2 )
		return( -1 );

	eigen2dAux_final( &a, e );



	vec2f_type p;

	if( i00 > 0 ){
		p.x = apt->a[i00].p.x - apt->a[i00-1].p.x;
		p.y = apt->a[i00].p.y - apt->a[i00-1].p.y;
	}
	else {
		p.x = apt->a[i00+1].p.x - apt->a[i00].p.x;
		p.y = apt->a[i00+1].p.y - apt->a[i00].p.y;
	}

	if( VEC2D_INNER( p, e->v1) < 0 )
		VEC2D_MINUS( e->v1 );

	return( 1 );
}


int pt2A_eigen_T( pt2dA_type *apt, int i0, float T, eigen2d_type *e )
{
	int	i;

	eigen2dAux_type	a;
	eigen2dAux_init( &a, 1 );


	float T2 = T*T;


	pt2d_type	*pt0 = &apt->a[i0];

	for( i = 0 ; i < apt->nA ; i++ ){
		if( i0 == i )	continue;

		pt2d_type	*pt = &apt->a[i];

		vec2f_type	p;
		p.x = pt->p.x - pt0->p.x;
		p.y = pt->p.y - pt0->p.y;

		if( p.x *p.x + p.y*p.y > T )
			continue;


		EIGEN2D_UPDATE( a, pt->p.x, pt->p.y );
	}

	if( a.n < 2 )
		return( -1 );

	eigen2dAux_final( &a, e );

	return( 1 );
}
