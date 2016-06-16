


#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#include	<string.h>


//#ifdef _DEBUG
//#define _DUMP 
//#endif







#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"Ulog/Log.h"




#include "Uln/PlnType.h"

#include "PlnConnect.h"





int
	pln_connect( plnA_type *apl, plnConnectPrm_type *prm )
{
int	i,	n;

	for( i = 0, n = 0 ; i < apl->nA ; i++ ){
		pln_type *bpl = apl->a[i];
		if( bpl == NULL )	continue;

		n += pln_connect( bpl, prm, apl );
	}

	plnA_decrease( apl );


	GPLOG(("append: %d\n", n ));

	return( 1 );
}

int
pln_connect( pln_type *bpl, plnConnectPrm_type *prm, plnA_type *apl )
{
	int n = 0;


	while( pln_connect_connect_1( bpl, F_END,prm, apl ) > 0 ){
		n++;
	}


	pln_inverse( bpl );

	while( pln_connect_connect_1( bpl, F_END,prm, apl ) > 0 ){
		n++;
	}

	return( n );
}


int
pln_connect_connect_1( pln_type *bpl, int direct, plnConnectPrm_type *prm, plnA_type *apl )
{
	dPlnConnect_type dc;

	if(  pln_connect_find( bpl, direct, prm, apl, &dc  ) < 0 )
		return( -1 );

	if( dc.d > prm->gapC )
		return( -1 );


	pln_type *pl = apl->a[dc.i ];
	apl->a[dc.i] = NULL;

	if( dc.direct == F_END )
		pln_inverse( pl );

	pln_append( bpl, pl );

	return( 1 );
}

int
pln_connect_find( pln_type *bpl, int direct, plnConnectPrm_type *prm, plnA_type *apl, dPlnConnect_type *dc  )
{


	dc->direct = direct;

	if( direct == F_BEGIN ){
		pln_pv_d( bpl, 0, prm->dt , 1.0, &dc->bvl );
		VEC2D_MINUS( dc->bvl.v );
	}
	else 
		pln_pv_d( bpl, bpl->len - prm->dt, bpl->len , 1.0, &dc->bvl );


	if( pln_connect_find( bpl, &dc->bvl, prm, apl, dc ) < 0 )
		return( -1 );

	//fprintf( stdout, "%d %d  %.2f\n", dc->i, dc->direct, dc->d );

	return( 1 );
}



int
pln_connect_find( pln_type *bpl, vl2f_type *bvl, plnConnectPrm_type *prm, plnA_type *apl, dPlnConnect_type *dc )
{
int	i;


	dc->i = -1;
	dc->d = 0;
	dc->direct = -1;

	vec2f_type	bp;

	bp.x = bvl->p.x + bvl->d*bvl->v.x;
	bp.y = bvl->p.y + bvl->d*bvl->v.y;

	for( i = 0 ; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];

		if( pl == NULL || pl == bpl )
			continue;



		vl2f_type vl;
		int direct = 0;
		pln_pv_d( pl, 0, prm->dt, 1.0, &vl );


		float a = VEC2D_INNER( bvl->v, vl.v );
		if( VEC2D_INNER( bvl->v, vl.v ) < 0 ){
			pln_pv_d( pl, pl->len - prm->dt, pl->len, 1.0, &vl );
			VEC2D_MINUS( vl.v );
			direct = 1;
		}


		a = VEC2D_INNER( vl.v, bvl->v );

		if( a < prm->aT )
			continue;

		vec2f_type	p;
		p.x = vl.p.x - vl.d*vl.v.x;
		p.y = vl.p.y - vl.d*vl.v.y;


		//dPln_type dPln;
		//if( pln_distance( pl, &bp, &dPln ) > 0 ){
		//	if( ABS(dPln.u) < prm->nd && dPln.gt > 0 && dPln.gt < pl->len ){
		//		float d = ABS(dPln.u);
		//		if( dc->i >= 0 && d > dc->d )
		//			continue;

		//		if( direct == F_BEGIN && dPln.gt > 2*prm->nd   || direct == F_END && pl->len - dPln.gt > 2*prm->nd )
		//			continue;


		//		dc->i = i;
		//		dc->d = d;
		//		dc->direct = direct;
		//		dc->vl = vl;
		//		continue;
		//	}
		//}


		vec2f_type	dp;
		dp.x = p.x - bp.x;
		dp.y = p.y - bp.y;

		float d = dp.x*dp.x + dp.y*dp.y;

		d = hypot( dp.x, dp.y );

		if( d > prm->r )	continue;

		if( dc->i >= 0 && d > dc->d )
			continue;

		if( d < prm->r0 ){
			dc->i = i;
			dc->d = d;
			dc->direct = direct;
			dc->vl = vl;
			continue;
		}

		dp.x /= d;
		dp.y /= d;
		a = VEC2D_INNER( dp, bvl->v );

		if( a < prm->aT )
			continue;


		//a = VEC2D_INNER( bvl->v, vl.v );
		//if( a < prm->aT )
		//	continue;



		float t0 = dp.x * -bvl->v.y + dp.y * bvl->v.x;
		float t1 = dp.x * -vl.v.y + dp.y * vl.v.x;

		if( (t0 < 0) == (t1 < 0 ) && ABS(t0) > 0.5* prm->aT && ABS(t1) > 0.5* prm->aT )
			continue;


		dc->i = i;
		dc->d = d;
		dc->direct = direct;
		dc->vl = vl;

	}



	return( dc->i );
}








		
		


