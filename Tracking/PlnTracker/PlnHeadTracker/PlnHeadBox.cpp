
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#include "Umath/EigenType.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif



#include "PlnHeadTracker.h"



static void	pln_box_x( pln_type *pl, float x0, float x1, box2f_type *b );


int CPlnHeadTracker::HeadBox( pln_type *pl, float len, box2f_type *b )
{


	vec2f_type p;
	HeadPose( pl, &p );


	pln_box_x( pl, p.x, p.x + len, b );


	return( 1 );
}





static void
pln_box_x( pln_type *pl, float x0, float x1, box2f_type *b )
{
	float	gt;
	int	n;

	vec2f_type	p;

	for( gt = 0, n = 0 ; gt < pl->len ; gt += 1.0 ){
		pln_gt2p( pl, gt, &p );

		if( p.x < x0 || p.x > x1 )	continue;

		if( n == 0 ){
			BOX2D_INIT( *b, p.x, p.y )
		}
		else {
			BOX2D_UPDATE( *b, p.x, p.y );
		}

		n++;
	}
}
