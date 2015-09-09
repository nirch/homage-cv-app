
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#include "Umath/EigenType.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif



#include "PlnHeadTracker.h"



static int	pln_x0( pln_type *pl, float x0, float *gt );


static int		pln_min_x0( pln_type *pl, box2f_type *b, float *x0, float *gt0 );



int CPlnHeadTracker::HeadPose( plnA_type *apl, headPose_type *h )
{
	vec2f_type p;

	int ret = HeadPose( apl->a[0], NULL, &p );

	if( ret < 0 && apl->nA > 1 )
		ret = HeadPose( apl->a[1], NULL, &p );

	if( ret < 0 )	return( -1 );

	float scale = 1.0;
	headPose_set( h, -1, 0, p.x, p.y, scale, 0.0 );

	return( ret );
}




int CPlnHeadTracker::HeadPose( pln_type *pl, pln_type **spl, vec2f_type *p )
{
	box2f_type	box;
	float gt;

	pln_box( pl, &box );


	if( pln_x0( pl, box.x0, &gt ) < 0 )
		return( -1 );

	if( pl->state == PLN_OPEN && gt < 4 || gt > pl->len - 4 )
		return( -1 );

	float ret = HeadPose_peak( pl, gt, spl, p );
	
	return( ret );
}



int CPlnHeadTracker::HeadPose( plnA_type *apl, vec2f_type *p0, pln_type **spl, vec2f_type *p )
{
	box2f_type	box;
	float gt,	x0;



	box.x0 = p0->x - 100;
	box.x1 = p0->x + 100;

	box.y0 = p0->y - 100;
	box.y1 = p0->y + 100;


	vec2f_type ap[16];
	int	n;

	int	i;
	for( i = 0, n = 0 ; i < apl->nA ; i++ ){
		if( pln_min_x0( apl->a[i], &box, &x0, &gt ) < 0 )
			continue;


		if( HeadPose_peak( apl->a[i], gt, NULL, &ap[n] ) > 0 )
			n++;
	}

	if( n <= 0 )
		return( -1 );

	//if( n == 1 ){
	//	*p = ap[0];
	//	return( 1 );
	//}

	float	tMin;
	int	i0 = -1;
	for( i = 0 ; i < n ; i++ ){
		vec2f_type	dp;
		dp.x = ap[i].x - p0->x;
		dp.y = ap[i].y - p0->y;
		float t = dp.x*dp.x + dp.y*dp.y;
		if( i0 < 0 || t < tMin ){
			i0 = i;
			tMin = t;
		}
	}


	*p = ap[i0];


	lt2_type lt;
	lt2_similarity_set( &lt, p->x, p->y, 0, 1.0F );

	if( spl != NULL )
		*spl =  pln_affine_lt( m_cln->a[0], &lt, *spl );

	return( 1 );


	
}


int CPlnHeadTracker::HeadPose( pln_type *pl, vec2f_type *p0, pln_type **spl, vec2f_type *p )
{
	box2f_type	box;
	float gt,	x0;

	pln_box( pl, &box );

	box.x0 = p0->x - 100;
	box.x1 = p0->x + 100;

	box.y0 = p0->y - 100;
	box.y1 = p0->y + 100;

	if( pln_min_x0( pl, &box, &x0, &gt ) < 0 )
		return( -1 );


	float ret = HeadPose_peak( pl, gt, spl, p );

	return( ret );
}


int CPlnHeadTracker::HeadPose_peak( pln_type *pl, float gt,  pln_type **spl, vec2f_type *p )
{
	//box2f_type	box;
	//float gt;

	//pln_box( pl, &box );


	//if( pln_x0( pl, box.x0, &gt ) < 0 )
	//	return( -1 );

	pln_gt2p( pl, gt, p );


	float dt = 75;

	eigen2d_type e;
	pln_eigenS(pl, gt - dt, gt+dt, 1.0, &e );

	
	vec2f_type dp;
	float t = (p->x - e.p.x) * e.v2.x + (p->y - e.p.y) * e.v2.y;
	dp.x = e.p.x + t * e.v2.x;
	dp.y = e.p.y + t*e.v2.y;



//	if( ABS(e.v2.x) > 0.8 ){
////		*p = dp;
//
////		a = atan2( e.v1.x, e.v1.y ) - M_PI;
//	}


	p->x = p->x - m_p0.x;
	p->y = p->y - m_p0.y;

	
	if( spl != NULL ){
		float a = 0;
		lt2_type lt;
		lt2_similarity_set( &lt, p->x, p->y, a, 1.0F );



		*spl =  pln_affine_lt( m_cln->a[0], &lt, *spl );
	}

	return( 1 );
}



int CPlnHeadTracker::HeadPose( pln_type *pl, vec2f_type *p )
{
box2f_type	box;
float gt;

		pln_box( pl, &box );


		if( pln_x0( pl, box.x0, &gt ) < 0 )
			return( -1 );

		pln_gt2p( pl, gt, p );

		return( 1 );
}



static int	
pln_x0( pln_type *pl, float x0, float *gt0 )
{
float	gt;
vec2f_type	p;
float	tMin;

	tMin = -1;
	



	for( gt = 0 ; gt < pl->len ; gt += 2.0 ){

		pln_gt2p( pl, gt, &p );

		float t = p.x - x0;
		if( t < 0 )	t = -t;

		if( tMin < 0 || t < tMin ){

			*gt0 = gt;
			tMin = t;
		}
	}


	if( tMin < 0 )
		return( -1 );

	return( 1 );
}



static int	
pln_min_x0( pln_type *pl, box2f_type *b, float *x0, float *gt0 )
{
	float	gt;
	vec2f_type	p;

	*gt0 = -1;


	for( gt = 0 ; gt < pl->len ; gt += 2.0 ){

		pln_gt2p( pl, gt, &p );

		if( BOX2D_IS_IN( *b, p.x, p.y) == 0 )
			continue;


		if( *gt0 < 0 || p.x < *x0 ){
			*x0 = p.x;
			*gt0 = gt;
		}
	}


	if( *gt0 < 0 )
		return( -1 );

	return( 1 );
}

