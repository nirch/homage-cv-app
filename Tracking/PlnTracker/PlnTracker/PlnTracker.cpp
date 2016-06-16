
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif



#include "PlnTracker.h"




CPlnTracker::CPlnTracker()
{
	m_cln = NULL;
	m_cycle = 10;
}


CPlnTracker::~CPlnTracker()
{
	DeleteContents();

}



void CPlnTracker::DeleteContents()
{

	if( m_cln != NULL ){
		cln_destroy( m_cln );
		m_cln = NULL;
	}


}


int CPlnTracker::Init( cln_type *cln, int fCopy )
{
	DeleteContents();

	m_cln = cln;
	if( fCopy )
		m_cln = cln_make_copy( cln );

	lt2_similarity_set( &m_lf.lt, m_cln->p.x, m_cln->p.y, 0.0F, 1.0F );

	pln_translate( m_cln->a[0], -m_cln->p.x, -cln->p.y );


	pln_box( m_cln->a[0], &m_box );

	m_r = 0.5*(m_box.y1 - m_box.y0);

	m_p0.x = m_box.x0;
	m_p0.y = 0.5*(m_box.y1 + m_box.y0);

	return( 1 );
}




void CPlnTracker::SetPose( float x, float y, float angle, float scale )
{
	
		lt2_similarity_set( &m_lf.lt, x, y, angle, scale );

		m_lf.cover = 1;
}

int	CPlnTracker::GetPose( float *x, float *y, float *angle, float *scale )
{
	if( m_lf.cover <= 0 )
		return( -1 );

	lt2_similarity_get( &m_lf.lt, x, y, angle, scale );

	return( 1 );
}

pln_type*	CPlnTracker::GetPln( pose2f_type *h )
{
lt2_type lt;

	if( h == NULL )
		h = &m_pose;

	lt2_similarity_set( &lt, h->p.x, h->p.y, h->angle, h->scale );


	pln_type *pl =  pln_affine_lt( m_cln->a[0], &lt, NULL );

	return( pl );
}



pln_type *	CPlnTracker::GetPln()
{
	pln_type *pl;

	pl =  pln_affine_lt( m_cln->a[0], &m_lf.lt, NULL );

	return( pl );
}








int	CPlnTracker::Process( plnA_type *apl, pose2f_type *pose0, pose2f_type *pose, int iFrame )
{

	SetPose( pose0->p.x, pose0->p.y, pose0->angle, pose0->scale );

	if( apl == NULL || apl->nA ==  0 )
		return( -1 );


	pln_type *bpl = m_cln->a[0];

	int ret = plnA_fit( apl, m_cln->a[0], 0, bpl->len, m_cycle, 4.0, &m_lf );

	if( ret < 0 )
		return( -1 );


	lt2_similarity_get( &m_lf.lt, &m_pose.p.x, &m_pose.p.y, &m_pose.angle, &m_pose.scale );

	m_pose.quality = m_lf.cover1;

//	m_pose.state = 1;

	*pose = m_pose;

	return( ret );
}




