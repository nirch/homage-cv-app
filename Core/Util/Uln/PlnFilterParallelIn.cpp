
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif

#include	"Uigp/igp.h"
#include	"Utime\GpTime.h"
#include	"Ulog/Log.h"

#include "PlnType.h"
#include "CrPlnType.h"



int			plnA_filter_parallel_in( plnA_type *apl, float dT );

//static int	plnA_filter_parallel_in_1( plnA_type *apl, int i0, float dT );




int
plnF_filter_parallel_in( plnF_type *fpl, float dT )
{
	int i;

	for( i = 0 ; i < fpl->nA ; i++ ){
		if( fpl->a[i] == NULL )	continue;
		plnA_filter_parallel_in( fpl->a[i], dT );
	}

	return( 1 );
}


int
plnA_filter_parallel_in( plnA_type *apl, float dT )
{
	int i;

	plnA_set_box( apl );
	for( i = 0 ; i < apl->nA ; i++ ){
		if( apl->a[i] == NULL )	
			continue;

		plnA_filter_parallel_in_1( apl, i, dT );
	}


	int n = apl->nA;
	plnA_decrease( apl );


	return( 1 );
}



int
plnA_filter_parallel_in_1( plnA_type *apl, int i0, float dT )
{
crPlnA_type *ac;

float	dR1,	dR2;
int	i,	n;

	dR1 = dR2 = dT;

	pln_type *bpl = apl->a[i0];


	ac = crPlnA_alloc( 64 );
	for( i = 0, n = 0 ; i < apl->nA ; i++ ){
		if( i == i0 || apl->a[i] == NULL )	continue;
		pln_type *pl = apl->a[i];

		if( box2f_distance( &bpl->b, &pl->b) > 2*dT )
			continue;

		if( pl->len > bpl->len )	continue;


		ac->nA = crPln_create( bpl, pl, i, dR1, dR2, 0, ac->a );

		if( ac->nA == 0 )	continue;

		
		crPln_type *cr = &ac->a[0];

		if( cr->d[0].sgt < 10 && cr->d[1].sgt > pl->len - 10 ){
			plnA_destroy_pl( apl, i );
			n++;


		}

	}

	crPlnA_destroy( ac );

	return( n );
}

