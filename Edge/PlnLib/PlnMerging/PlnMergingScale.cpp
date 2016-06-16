
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif

#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"Ulog/Log.h"

#include "Uln/PlnType.h"
#include "Uln//CrPlnType.h"


#include "PlnMerging.h"




static int	plnA_filter_parallel_in_1( plnA_type *apl, pln_type *bpl, plnMergingPrm_type *prm );




plnA_type * 
plnA_merging_scale( plnA_type *apl, plnA_type *tapl, plnMergingPrm_type *prm )
{
	int	i,	n;

	if( tapl == NULL )
		return( apl );


	for( i = 0, n = 0 ; i < apl->nA ; i++ ){
		//PLNA_DUMP( tapl, "PLN", i, "BASE-S" );
		n = plnA_filter_parallel_in_1( tapl, apl->a[i], prm );

		//PLNA_DUMP( tapl, "PLN", i, "BASE-D" );

		plnA_merging( tapl, apl->a[i], prm );// prm->dU );
	}
		

	plnA_decrease( tapl );

	apl->nA = 0;
	plnA_destroy( apl );


	return( tapl );
}







static int
plnA_filter_parallel_in_1( plnA_type *apl, pln_type *bpl, plnMergingPrm_type *prm )
{
	crPlnA_type *ac;

	float	dR1,	dR2;
	int	i,	n;

	dR1 = dR2 = prm->dU;


	pln_box( bpl, &bpl->b );

	ac = crPlnA_alloc( 64 );
	for( i = 0, n = 0 ; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];

		if( pl == bpl|| apl->a[i] == NULL )	continue;

		pln_box( pl, &pl->b );

		if( box2f_distance( &bpl->b, &pl->b) > 2*prm->dU )
			continue;

		if( pl->len > bpl->len )	continue;



		ac->nA = crPln_create( bpl, pl, i, dR1, dR2, 0, ac->a );

		if( ac->nA == 0 )	continue;

		if( ABS(ac->a[0].u) > prm->dU )
			continue;

		crPln_type *cr = &ac->a[0];

		if( cr->d[0].sgt < prm->dT && cr->d[1].sgt > pl->len - prm->dT ){
			plnA_destroy_pl( apl, i );
			n++;
		}

	}

	crPlnA_destroy( ac );

	return( n );
}
