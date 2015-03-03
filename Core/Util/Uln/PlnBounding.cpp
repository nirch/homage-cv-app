/*******************
 ***  PlnTool.c  ***
 *******************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"

#ifdef _DEBUG
#define _DUMP
#endif



#include "Uln/PlnType.h"



pln_type * 
plnA_bounding( plnA_type *apl, float radius )
{

	float len = plnA_length( apl );

	pt2dA_type	*apt;
	apt = pt2dA_alloc( len / 2 + apl->nA *2 );

	apt->axis = PT2D_AXIS_XY;
	apt->type = PT2D_4V;

	int	i;
	for( i = 0 ; i < apl->nA ; i++ ){
		pln_sampleN( apl->a[i], 2, radius, apt );
	}


	PT2DA_DUMP( apt, "circle", 1, NULL );


	pln_type * pl = pt2dA_bounding_contour( apt );

	PLNA_DUMP( apl,  "pl", 1, NULL );


	pt2dA_destroy( apt );

	pln_type *spl = pln_smooth( pl );
	pln_destroy( pl );

	return( spl );
}