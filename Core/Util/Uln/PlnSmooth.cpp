/*******************
 ***  PlnTool.c  ***
 *******************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"

#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif
#include "Uigp/GpMemoryLeak.h"


#include "PlnType.h"


int
plnA_smooth( plnA_type *apl )
{
	int	i;

	for( i = 0 ; i < apl->nA ; i++ ){

		pln_type *pln = pln_smooth( apl->a[i] );
		pln_destroy( apl->a[i] );
		apl->a[i] = pln;
	}
	return( 1 );
}

pln_type *
pln_smooth( pln_type *pl )
{
	pt2dA_type *apt = pt2dA_alloc( pl->len + 10 );

	pln_sampleN( pl, 1, 1, apt );

	pt2dA_type *tapt = pt2dA_smooth( apt,  1, NULL );

	PT2DA_DUMP( apt, "aa", 1, NULL );

	PT2DA_DUMP( tapt, "aa", 1, "SMOOTH" );

	pln_type *spl;
	pln_approximateA( tapt, 1, &spl );

	pt2dA_destroy( apt );
	pt2dA_destroy( tapt );

	return( spl );
}