// WndInner.cpp : implementation file
//

#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Uigp/igp.h"


#include "PlnType.h"




float pln_in_box( pln_type *pl, box2f_type *b )
{

	float gt;
	int n;
	vec2f_type	p;

	for( gt = 0,  n = 0 ; gt < pl->len ; gt += 4 ){
		pln_gt2p( pl, gt, &p );

		if( BOX2D_IS_IN( *b, p.x, p.y) != 0 )
			n++;
	}

	return( n*4 );
}






