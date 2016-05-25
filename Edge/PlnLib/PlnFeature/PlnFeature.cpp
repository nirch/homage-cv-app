/****************************
	RegAgpFeatureFilter.c
****************************/

#include	<math.h>

#include	"Uigp/igp.h"
#include "Ulog/Log.h"


#include "Uln/PlnType.h"

#include "PlnFeature.h"



static int	plnA_feature_1( plnA_type *apl, int i0, plnFeaturePrm_type *prm );

static int	plnA_feature_corner( plnA_type *apl, int i0, plnFeaturePrm_type *prm );



int
plnA_feature( plnA_type *apl, plnFeaturePrm_type *prm )
{

int	i,	n;

	plnA_set_groupId( apl, 0 );

	plnA_set_box( apl );


	for( i = 0, n = 0  ; i < apl->nA ; i++ ){
		if( plnA_feature_1( apl, i, prm  ) > 0 ){
			apl->a[i]->group = 1;
			n++;
		}

	}

	return( n );
}



static int
plnA_feature_1( plnA_type *apl, int i0, plnFeaturePrm_type *prm )
{


	pln_type *bpl = apl->a[i0];

	if( bpl->len > prm->featureLen )
		return( 1 );


	if( plnA_feature_corner( apl, i0, prm ) > 0 )
		return( 1 );

	return( -1 );

}


static int
plnA_feature_corner( plnA_type *apl, int i0, plnFeaturePrm_type *prm )
{

int	i,	n;
vec2f_type	p;
float	t;

float maxD;


	maxD = prm->cornerDis;


	pln_type *bpl = apl->a[i0];

	if( bpl->len < prm->cornerLen )
		return( -1 );



	pln_end_point( bpl, &p );
	
	

	vl2f_type	vl0,	vl1;
	pln_pv_d( bpl, 0, 10, 1.0, &vl0 );
	pln_pv_d( bpl, bpl->len-10, bpl->len, 1.0, &vl1 );




	for( i = 0,	n = 0 ; i < apl->nA ; i++ ){
		if( i == i0 )	continue;
		pln_type *pl = apl->a[i];

		if( pl->len < prm->cornerLen )
			continue;

		if( box2f_distance( &bpl->b, &pl->b ) > maxD )
			continue;

		dPln_type	d;
		if( pln_distance( pl, &bpl->ctr, &d  ) >= 0 && ABS(d.u) < maxD ){
			
	
			vl2f_type vl2;
			pln_app_vl( pl, d.gt, 5, &vl2 );
			t = VEC2D_INNER( vl0.v, vl2.v );

			
			if( ABS(t) < 0.4 )
				return( 1 );
		}


		if( pln_distance( pl, &p, &d  ) >= 0 && ABS(d.u) < maxD ){


			vl2f_type vl2;
			pln_app_vl( pl, d.gt, 5, &vl2 );
			t = VEC2D_INNER( vl0.v, vl2.v );


			if( ABS(t) < 0.4 )
				return( 1 );
		}
	}

	return( -1 );
}
