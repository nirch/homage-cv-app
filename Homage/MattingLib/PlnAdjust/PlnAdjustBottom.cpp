
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif


#include	"Uigp/igp.h"
#include	"Ulog/Log.h"


#include "Uln/PlnType.h"



static float	pln_is_in_apl( pln_type *bpl, float gt0, float gt1, plnA_type *apl, float T );



int	pln_rangeH( pln_type *pl, float h, float ag[] );


int
plnA_adjust_bottom( plnA_type *apl, plnA_type *eapl, float height, int fillAll, int iFrame )
{
	int	i;

	float ag[100];
	int n =	pln_rangeH(  apl->a[0], height, ag );

	if( n < 2 )
		return( -1 );

	if( ag[n-1] - ag[n-2] < 16 )
		n -= 2;

	int n0 = 0;
//	if( ag[1] - ag[0] < 16 )
//		n0 = 2;

	if( n - n0 <= 2 )
		return( -1 );


	pln_type *pl = apl->a[0];

	for( i = n-2 ; i > n0; i -= 2 ){

		float gt0 = ag[i-1];
		float gt1 = ag[i];
		float t = pln_is_in_apl( pl, gt0, gt1, eapl, 2.0 );

		if( fillAll == 0 && t > 0.4 )	continue;

		vec2f_type	p0,	p1;
		pln_gt2p( pl, gt0, &p0 );
		pln_gt2p( pl, gt1, &p1 );

		if( p0.y < p1.y )
			continue;

		pln_type *pl0 = pln_copy_sub( pl, gt1, pl->len );

		pln_trim( pl, F_END, pl->len - gt0 );

		fprintf( stdout, "B" );

		pln_append( pl, pl0 );


	}


	PLN_DUMP( pl, "BB", 1, NULL );

	return( 1 );
}





static float
pln_is_in_apl( pln_type *bpl, float gt0, float gt1, plnA_type *apl, float T )
{
	int	n;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt,	s;


	n = 0;

	pln_type *spl;

	dt = 2.0;
	s = 0;
	for( gt = gt0 ; gt < gt1 ; gt += dt, n++ ){

		pln_gt2p( bpl, gt, &p );

		if( plnA_distance( apl, &p, 2*T, &spl, &d ) < 0 )
			continue;


		if( ABS(d.u) < T )
			s += dt;
	}


	return( s / ( gt1-gt0) );

}



