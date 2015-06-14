/*************************
 ***   Pt2dGroup.cpp   ***
 *************************/
#include	<math.h>


#include "Ucamera/Pt2dType.h"



void
pt2dA_mask( pt2dA_type *apt, pt2dA_type *mapt, float dis )
{
pt2d_type	*pt,	*mpt;
int	i,	j,	k;
float	x,	y,	dis2;


	dis2 = dis*dis;

	k = 0;
	for( i = 0; i < apt->nP; i++ ){
		pt = &apt->p[i];

		for( j = 0; j < mapt->nP; j++ ){
			mpt = &mapt->p[j];

			x = pt->p.x - mpt->p.x;
			y = pt->p.y - mpt->p.y;

			if( x*x + y*y < dis2 )	
				break;
		}

		if( j < mapt->nP )
			continue;

		apt->p[k++] = apt->p[i];
	}

	apt->nP = k;
}
