
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"
#include "Uln/PlnType.h"

#include "Umath/EigenType.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif



#include "PlnHeadTracker.h"



static int	pln_x0( pln_type *pl, float x0, float *gt );






int CPlnHeadTracker::HeadPose( pln_type *pl, pln_type **spl, vec2f_type *p )
{
	box2f_type	box;
	float gt;

	pln_box( pl, &box );


	if( pln_x0( pl, box.x0, &gt ) < 0 )
		return( -1 );

	pln_gt2p( pl, gt, p );


	float dt = 75;

	eigen2d_type e;
	pln_eigenS(pl, gt - dt, gt+dt, 1.0, &e );

	
	vec2f_type dp;
	float t = (p->x - e.p.x) * e.v2.x + (p->y - e.p.y) * e.v2.y;
	dp.x = e.p.x + t * e.v2.x;
	dp.y = e.p.y + t*e.v2.y;


	float a = 0;
	if( ABS(e.v2.x) > 0.8 ){
//		*p = dp;

//		a = atan2( e.v1.x, e.v1.y ) - M_PI;
	}



	lt2_type lt;
	lt2_similarity_set( &lt, p->x - m_p0.x, p->y - m_p0.y, a, 1.0F );

	p->x = p->x - m_p0.x;
	p->y = p->y - m_p0.y;

	*spl =  pln_affine_lt( m_cln->a[0], &lt, *spl );

	//lt2_similarity_set( &m_lf.lt, p->x, p->y, a, 1.0F );

	//m_lf.lt = lt;
	//m_lf.cover = 1.0;

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
