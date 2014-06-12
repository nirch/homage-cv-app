
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
#include "Umath/Ellipse/EllipseType.h"





static void	pln_ellipse( pln_type *pl );

static int	plnA_is_in( plnA_type *apl, pln_type *pl );





int
plnA_adjust_in( plnA_type *apl, plnA_type *bapl, int iFrame )
{
int	i,	j,	ret;
	
	ret = 0;

	for( i = 1 ; i < apl->nA ; i++ )
		pln_ellipse( apl->a[i] );

	if( iFrame < 1 )	return( 0 );


	for( i = 1, j = 1 ; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];

		dPln_type	d;
		pln_distance( apl->a[0], &pl->e->p, &d );

		if( ABS(d.u) > 40 ){
			fprintf( stdout, "I%d", iFrame);
			pln_destroy( pl );
			continue;
		}

		if( plnA_is_in( bapl, pl ) < 0 ){
			fprintf( stdout, "J%d", iFrame);
			pln_destroy( pl );
			continue;
		}


		apl->a[j++] = apl->a[i];

	}

	apl->nA = j;

	return( ret );
}


static int
plnA_is_in( plnA_type *apl, pln_type *pl )
{
	int	i;

	for( i = 1 ; i < apl->nA ; i++ ){
		if(  ellipse_distance( apl->a[i]->e, pl->e ) < 32 )
			return( 1 );
	}

	return( -1 );
}



static void
pln_ellipse( pln_type *pl )
{
	
	pt2dA_type *apt = pt2dA_alloc( pl->len + 10 );

	pln_sampleN( pl, 1, 1, apt );

	pl->e = ellipse_alloc();
	pt2dA_ellipse( apt, 0, apt->nA, pl->e );

	pt2dA_destroy( apt );
}