#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"

#include	"Umath/Matrix2Type.h"
#include	"Umath/Matrix3Type.h"
#include	"Ucamera/Pt2dType.h"





pt2dA_type *
pt2d_triangle_filter( pt2dA_type *apt, float R, float T )
{
int	i;
pt2dA_type	*tapt;

float	x, y,	dx,	dy;
float	l0,	l1,	l2;

	tapt = pt2dA_copy( apt, NULL );

	tapt->nP = 0;


	x = apt->p[1].p.x - apt->p[0].p.x ;
	y = apt->p[1].p.y - apt->p[0].p.y ;
	l0 = hypot( x, y );


	tapt->p[tapt->nP++] = apt->p[0];

	for( i = 1 ; i < apt->nP-1 ; i++ ){

		x = apt->p[i+1].p.x - apt->p[i].p.x ;
		y = apt->p[i+1].p.y - apt->p[i].p.y ;
		l1 = hypot( x, y );

		x = apt->p[i+1].p.x - apt->p[i-1].p.x ;
		y = apt->p[i+1].p.y - apt->p[i-1].p.y ;
		l2 = hypot( x, y );


		dx = 2*apt->p[i].p.x - apt->p[i+1].p.x - apt->p[i-1].p.x;
		dy = 2*apt->p[i].p.y - apt->p[i+1].p.y - apt->p[i-1].p.y;

		if( l2 < R*(l0+l1) || ABS(dx)> 80 || ABS(dy) > 2*T ){

			l0 = l2;
			continue;
		}

		
		tapt->p[tapt->nP++] = apt->p[i];
		l0 = l1;

	}


	return( tapt );
}


