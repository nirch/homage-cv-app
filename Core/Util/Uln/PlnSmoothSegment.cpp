/*********************************
 ***	PlnSmoothSegment.cpp   ***
 *********************************/
#include	<stdio.h>   
#include	<math.h>

#include	"Uigp/igp.h"
#include	"UGeoApp/GeoApp.h"

#include "PlnType.h"




static float	gapp_get_e2( gapp_type *gapp, int i0, int i1 );



int 
pln_smooth_segment( pln_type *pl, float minLen, gapp_type *gapp, float ag[], int *nG )
{
int	n;
int	i,	i0,	i1;


	pln_to_gapp( pl, 1.0, gapp );

	gapp_smooth( gapp );

	*nG = 0;
	for( i = 0, n =0 ; i < gapp->no ; i = i1+1 ){

		if( gapp_smooth_next( gapp, i, &i0, &i1 ) < 0 )
			break;

		if( i1 - i0 < minLen )
			continue;


		gapp_smooth_extend_end( gapp, &i0, &i1 );

		ag[(*nG)++] = i0;
		ag[(*nG)++] = i1;

		n++;
//		lr->e2 = gapp_get_e2( gapp, i0, i1 );

	}

	

	return( n );
}




static float
gapp_get_e2( gapp_type *gapp, int i0, int i1 )
{
int	n,	i;
float	a,	b,	c;
vec2d	nv;
float	desc,	e1,	e2;

	a = b = c = 0;
	n = 0;

	for( i = i0 ; i <= i1 ; i++ ){

		nv.x =  gapp->tang[i].y;
		nv.y = -gapp->tang[i].x;

		a += nv.x * nv.x;
		b += nv.x * nv.y;

		c += nv.y * nv.y;

		n++;
	}

	a /= n;
	b /= n;
	c /= n;


	desc = hypot( a-c, 2*b );
	e1 = ((a+c) + desc ) * 0.5;
	e2 = ((a+c) - desc ) * 0.5;

	return( e2 );
}

