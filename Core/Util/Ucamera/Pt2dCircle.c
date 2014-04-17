
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"


#include "Pt2dType.h"



pt2dA_type *
pt2dA_circle( vec2f_type *ctr, float r, float dt, pt2dA_type *apt )
{
int	i,	n;
pt2d_type	*pt;
double	a;


	n = 2*M_PI*r/ dt + 0.5;

	apt = pt2dA_realloc( apt, n );

	for( i = 0 ; i < n ; i++ ){
		pt = &apt->p[i];

		a = 2*M_PI * i / n;

		pt->n.x = cos( a );
		pt->n.y = sin( a );

		pt->p.x = ctr->x + r * pt->n.x;
		pt->p.y = ctr->y + r * pt->n.y;
		
	}
	apt->nP = i;

	return( apt );

}
