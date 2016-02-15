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

pln_type *	pt2dA_bounding_contourNN( pt2dA_type *apt );


pln_type * 
pln_bounding_convex( pln_type *bpl, float radius )
{
	plnA_type *apl = plnA_from_pl( bpl );


	pln_type *pl = plnA_bounding_convex(  apl, radius );

	apl->nA = 0;
	plnA_destroy( apl );

	return( pl );
}


pln_type * 
plnA_bounding_convex( plnA_type *apl, float radius )
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

	pln_interior_force_right( spl );

	return( spl );
}



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

	PLNA_DUMP( apl, "circle", 1, NULL );
	PT2DA_DUMP( apt, "circle", 1, NULL );



	pln_type * pl = pt2dA_bounding_contourNN( apt );


	PLNA_DUMP( apl,  "pl", 1, NULL );


	pt2dA_destroy( apt );

	//return( pl );

	pln_type *spl = pln_smooth( pl );
	pln_destroy( pl );


	pln_interior_force_right( spl );

	return( spl );
}





pln_type * 
	plnA_boundingG( plnA_type *apl, float radius, float radius1 )
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

	PLNA_DUMP( apl, "circle", 1, NULL );
	PT2DA_DUMP( apt, "circle", 1, NULL );


	pln_type * pl = pt2dA_bounding_contourN( apt, radius, radius1 );
	
	if( pl == NULL ){
		pl = pt2dA_bounding_contour( apt ); 
		pt2dA_destroy( apt );
		return( pl );
	}

	if( pl == NULL )
		return( NULL );

	PLNA_DUMP( apl,  "pl", 1, NULL );


	pt2dA_destroy( apt );

	pln_type *spl = pln_smooth( pl );
	pln_destroy( pl );


	pln_interior_force_right( spl );

	return( spl );
}