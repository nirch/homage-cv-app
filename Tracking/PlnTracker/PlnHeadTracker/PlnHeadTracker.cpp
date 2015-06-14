
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif



#include "PlnHeadTracker.h"

//#include "PlnAdjust/PlnAdjust.h"



static int	pln_adjust_head( pln_type *pl, cln_type *cln, int iFrame, pln_type **spl, lnFit_type *f );

static int plnA_adjust_head( plnA_type *apl, cln_type *cln, int iFrame, pln_type **spl, lnFit_type *f );



CPlnHeadTracker::CPlnHeadTracker()
{
	m_cln = NULL;
//	m_pl = NULL;

	m_pose.smooth = 0;
}


CPlnHeadTracker::~CPlnHeadTracker()
{
	DeleteContents();

}



void CPlnHeadTracker::DeleteContents()
{

	if( m_cln != NULL ){
		cln_destroy( m_cln );
		m_cln = NULL;
	}


}


int CPlnHeadTracker::Init( cln_type *cln )
{
	DeleteContents();

	m_cln = cln;

	lt2_similarity_set( &m_lf.lt, m_cln->ctr.x, m_cln->ctr.y, 0.0F, 1.0F );

	pln_translate( m_cln->a[0], -m_cln->ctr.x, -cln->ctr.y );


	pln_box( m_cln->a[0], &m_box );

	m_r = 0.5*(m_box.y1 - m_box.y0);

	m_p0.x = m_box.x0;
	m_p0.y = 0.5*(m_box.y1 + m_box.y0);

	return( 1 );
}




void	CPlnHeadTracker::SetPose( float x, float y, float angle, float scale )
{
	
		lt2_similarity_set( &m_lf.lt, x, y, angle, scale );

		m_lf.cover = 1;
}

int	CPlnHeadTracker::GetPose( float *x, float *y, float *angle, float *scale )
{
	if( m_lf.cover <= 0 )
		return( -1 );

	lt2_similarity_get( &m_lf.lt, x, y, angle, scale );

	return( 1 );
}

pln_type*	CPlnHeadTracker::GetHeadPln( headPose_type *h )
{
lt2_type lt;

	if( h == NULL )
		h = &m_pose;

	lt2_similarity_set( &lt, h->p.x, h->p.y, h->angle, h->scale );


	pln_type *pl =  pln_affine_lt( m_cln->a[0], &lt, NULL );

	return( pl );
}

int	CPlnHeadTracker::Process( plnA_type *apl, headPose_type *pose0, vec2f_type ap[], int nP, int iFrame )
{
lnFit_type lf,	clf;

	lf.cover = 0;


	if( ProcessPrev( apl, pose0, &clf, iFrame ) > 0 ){
		if( clf.cover > lf.cover )
			lf = clf;
	}


	int	i;
	for( i = 0 ; i < nP ; i++ ){
		if( ProcessPrediction( apl, pose0, &ap[i], &clf, iFrame ) > 0 ){
			if( clf.cover > lf.cover )
				lf = clf;
		}
	}


	if( lf.cover <= 0 ){
		m_pose.state = 0;
		m_pose.qulity = 0;
		return( -1 );
	}


	m_lf = lf;



	m_pose.iFrame = iFrame;

	lt2_similarity_get( &m_lf.lt, &m_pose.p.x, &m_pose.p.y, &m_pose.angle, &m_pose.scale );

	m_pose.qulity = m_lf.cover;

	m_pose.state = 1;
	GPLOG(("%.4f\t%.4f\t%.4f\n", m_lf.cover, m_pose.angle, m_pose.scale ) );


	return( 1 );

}


int	CPlnHeadTracker::ProcessPrev( plnA_type *apl, headPose_type *pose0, lnFit_type *lf, int iFrame )
{
	lf->cover = 0;

	if( pose0->state < 1 )
		return( -1 );


	SetPose( pose0->p.x, pose0->p.y, pose0->angle, pose0->scale );

	int ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0  && m_lf.cover > lf->cover )
		*lf = m_lf; 


	if( lf->cover == 0 )
		return( -1 );


	return( 1 );
}



int	CPlnHeadTracker::ProcessPrediction( plnA_type *apl, headPose_type *pose0, vec2f_type *p, lnFit_type *lf, int iFrame )
{
	int	ret;

	lf->cover = 0;



	if( pose0->state == 1 ){
		SetPose( p->x + m_p0.x*(1 - pose0->scale), p->y + m_p0.y*(1 - pose0->scale), 0, pose0->scale );
		ret = ProcessA( apl, NULL, iFrame );
		if( ret > 0  && m_lf.cover > lf->cover )
			*lf = m_lf;
	}

	SetPose( p->x, p->y, 0, 1.0 );
	ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0  && m_lf.cover > lf->cover )
		*lf = m_lf;  


	SetPose( p->x, p->y, ANGLE_D2R(30), 1.0 );
	ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0 && m_lf.cover > lf->cover )
		*lf = m_lf;



	SetPose( p->x, p->y, ANGLE_D2R(-30), 1.0 );
	ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0 && m_lf.cover > lf->cover )
		*lf = m_lf;


	if( lf->cover <= 0 )
		return( -1 );


	return( 1 );
}




int	CPlnHeadTracker::ProcessTranslate( plnA_type *apl, headPose_type *pose0, int iFrame )
{


	if( pose0->state < 1 )
		return( -1 );


	//SetPose( pose0->p.x, pose0->p.y, pose0->angle, pose0->scale );


	pln_type *bpl = m_cln->a[0];

	lnFit_type lf;

	lt2_similarity_set( &lf.lt, pose0->p.x, pose0->p.y, pose0->angle, pose0->scale );
	lf.cover = 0;

	int ret = plnA_fitT( apl, bpl, 0, bpl->len, 10, 4.0, &lf );

	if( ret < 0 )
		return( ret );

	if( lf.cover < 0.4 )
		return( -1 );



	m_pose.iFrame = iFrame;

	lt2_similarity_get( &lf.lt, &m_pose.p.x, &m_pose.p.y, &m_pose.angle, &m_pose.scale );

	m_pose.qulity = m_lf.cover;

	m_pose.state = 1;


	*pose0 = m_pose;

	return( 1 );
}


int	CPlnHeadTracker::Process( plnA_type *apl, vec2f_type *p, pln_type **spl, int iFrame )
{
	

	lnFit_type lf;
	lf.cover = 0;


	int ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0  && m_lf.cover > lf.cover )
		lf = m_lf;  

	if( m_pose.state == 1 ){
		//SetPose( p->x, p->y, 0, m_pose.scale );
		SetPose( p->x + m_p0.x*(1 - m_pose.scale), p->y + m_p0.y*(1 - m_pose.scale), 0, m_pose.scale );
		ret = ProcessA( apl, NULL, iFrame );
		if( ret > 0  && m_lf.cover > lf.cover )
			lf = m_lf;
	}

	/*	float s = 1.5;
	SetPose( p->x + m_p0.x*(1 - s), p->y + m_p0.y*(1 - s), 0, s );
	ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0  && m_lf.cover > lf.cover )
	lf = m_lf;*/ 

	SetPose( p->x, p->y, 0, 1.0 );
	ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0  && m_lf.cover > lf.cover )
		lf = m_lf;  


	SetPose( p->x, p->y, ANGLE_D2R(30), 1.0 );
	ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0 && m_lf.cover > lf.cover )
		lf = m_lf;



	SetPose( p->x, p->y, ANGLE_D2R(-30), 1.0 );
	ret = ProcessA( apl, NULL, iFrame );
	if( ret > 0 && m_lf.cover > lf.cover )
		lf = m_lf;


	m_lf = lf;

	m_pose.iFrame = iFrame;
	m_pose.qulity = m_lf.cover;

	if( m_lf.cover > 0 ){
		lt2_similarity_get( &m_lf.lt, &m_pose.p.x, &m_pose.p.y, &m_pose.angle, &m_pose.scale );

		m_pose.state = 1;
		GPLOG(("%.4f\t%.4f\t%.4f\n", m_lf.cover, m_pose.angle, m_pose.scale ) );
	}
	else m_pose.state = 0;


	if( spl != NULL )
		*spl =  pln_affine_lt( m_cln->a[0], &m_lf.lt, NULL );


	if(m_pose.state == 0 )
		return( -1 );

	return( 1 );

}

int	CPlnHeadTracker::ProcessA( plnA_type *apl, pln_type **spl, int iFrame )
{

	if( apl == NULL || apl->nA ==  0 )
		return( -1 );

	int ret = plnA_adjust_head( apl, m_cln, iFrame, spl, &m_lf );


	return( ret );
}


int	CPlnHeadTracker::Process( plnA_type *apl, pln_type **spl, int iFrame )
{

	if( apl->nA ==  0  )
		return( -1 );

	int ret = pln_adjust_head( apl->a[0], m_cln, iFrame, spl, &m_lf );


	return( ret );
}


int	CPlnHeadTracker::Process( pln_type *pl, pln_type **spl, int iFrame )
{

	int ret = pln_adjust_head( pl, m_cln, iFrame, spl, &m_lf );


	return( ret );
}




pln_type *	CPlnHeadTracker::GetPln()
{
	pln_type *pl;

	pl =  pln_affine_lt( m_cln->a[0], &m_lf.lt, NULL );

	return( pl );
}


int	CPlnHeadTracker::HeadRange( plnA_type *apl, float *gt0, float *gt1 )
{

	*gt0 = *gt1 = -1;
	if( m_cln == NULL || m_lf.cover < 0 )
		return( -1 );


//	pln_adjust_head_range( apl, m_cln->a[0], &m_lf, gt0, gt1 );


	vec2f_type p,	tp;
	dPln_type d;

	pln_type *bpl =  m_cln->a[0];

	pln_gt2p( bpl, 10, &p );
	tp.x = LT2_F1( m_lf.lt, p.x, p.y );
	tp.y = LT2_F2( m_lf.lt, p.x, p.y );


	pln_distance( apl->a[0], &tp, &d );
	*gt0 = d.gt;


	pln_gt2p( bpl, bpl->len - 10, &p );
	tp.x = LT2_F1( m_lf.lt, p.x, p.y );
	tp.y = LT2_F2( m_lf.lt, p.x, p.y );
	pln_distance( apl->a[0], &tp, &d );
	*gt1 = d.gt;

	return( 1 );
}









static int
pln_adjust_head( pln_type *pl, cln_type *cln, int iFrame, pln_type **spl, lnFit_type *f )
{

	int cycle = 10;
	int side = 0;
	float	a,	b,	angle,	scale;

	lnFit_type f0 = *f;

	pln_type *bpl = cln->a[0];



	if( f->cover <= 0 )
		lt2_similarity_set( &f->lt, cln->ctr.x, cln->ctr.y, 0, 1.0 );


	int ret = pln_fit( pl, bpl, 0, bpl->len, side, cycle,  4.0, f );

	if( ret < 0 )
		return( ret );


	lt2_similarity_get( &f->lt, &a, &b, &angle, &scale );
	if( scale > 5 ){
		ret = -1;
		f->cover = 0;
		lt2_similarity_set( &f->lt, a, b, 0, 1.0 );
		return( -1 );
	}

	if( ABS(angle) > 0.5 ){
		lt2_similarity_get( &f0.lt, &a, &b, &angle, &scale );
		lt2_similarity_set( &f0.lt, a, b, 0, scale );
		int ret = pln_fit( pl, bpl, 0, bpl->len, side, cycle,  4.0, &f0 );
		if( ret > 0 && f0.cover > f->cover )
			*f = f0;
	}

	if( f->cover < 0.5 ){
		lt2_similarity_get( &f->lt, &a, &b, &angle, &scale );
		lt2_similarity_set( &f0.lt, a, b, 0, 1.0 );
		int ret = pln_fit( pl, bpl, 0, bpl->len, side, cycle,  4.0, &f0 );
		if( ret > 0 && f0.cover > f->cover )
			*f = f0;
	}

	lt2_similarity_get( &f->lt, &a, &b, &angle, &scale );

	if( ret > 0 && f->cover < 0.25 ){
		f->cover = 0;
		return( -1 );
	}


	if( ret > 0 ){
		if( spl != NULL )
			*spl =  pln_affine_lt( bpl, &f->lt, NULL );

		pln_type *tpl = pln_affine_lt( bpl, &f->lt, NULL );
		PLN_DUMP( tpl, "head", iFrame, "fit" );
		pln_destroy( tpl );
	}


	return( ret );
}



static int
	plnA_adjust_head( plnA_type *apl, cln_type *cln, int iFrame, pln_type **spl, lnFit_type *f )
{

	int cycle = 10;
	int side = 0;
	float	a,	b,	angle,	scale;

	lnFit_type f0 = *f;

	pln_type *bpl = cln->a[0];



	if( f->cover <= 0 )
		lt2_similarity_set( &f->lt, cln->ctr.x, cln->ctr.y, 0, 1.0 );


	//int ret = pln_fit( pl, bpl, 0, bpl->len, side, cycle,  4.0, f );
	int ret = plnA_fit( apl, bpl, 0, bpl->len, cycle, 4.0, f );

	if( ret < 0 )
		return( ret );


	lt2_similarity_get( &f->lt, &a, &b, &angle, &scale );
	if( scale > 5 ){
		ret = -1;
		f->cover = 0;
		lt2_similarity_set( &f->lt, a, b, 0, 1.0 );
		return( -1 );
	}

	if( ABS(angle) > 0.5 ){
		lt2_similarity_get( &f0.lt, &a, &b, &angle, &scale );
		lt2_similarity_set( &f0.lt, a, b, 0, scale );
	//	int ret = pln_fit( pl, bpl, 0, bpl->len, side, cycle,  4.0, &f0 );
		int ret = plnA_fit( apl, bpl, 0, bpl->len, cycle,  4.0, &f0 );
		if( ret > 0 && f0.cover > f->cover )
			*f = f0;
	}

	if( f->cover < 0.5 ){
		lt2_similarity_get( &f->lt, &a, &b, &angle, &scale );
		lt2_similarity_set( &f0.lt, a, b, 0, 1.0 );
		//int ret = pln_fit( pl, bpl, 0, bpl->len, side, cycle,  4.0, &f0 );
		int ret = plnA_fit( apl, bpl, 0, bpl->len, cycle, 4.0, &f0 );
		if( ret > 0 && f0.cover > f->cover )
			*f = f0;
	}

	lt2_similarity_get( &f->lt, &a, &b, &angle, &scale );

	if( ret > 0 && f->cover < 0.25 ){
		f->cover = 0;
		return( -1 );
	}

	//fprintf( stdout, "h" );

	if( ret > 0 ){
		if( spl != NULL )
			*spl =  pln_affine_lt( bpl, &f->lt, NULL );

		pln_type *tpl = pln_affine_lt( bpl, &f->lt, NULL );
		PLN_DUMP( tpl, "head", iFrame, "fit" );
		pln_destroy( tpl );
	}


	return( ret );
}


int	CPlnHeadTracker::GetHeadPose( vec2f_type *p, float *r )
{

	if( m_cln == NULL || m_lf.cover <= 0 )
		return( -1 );



	pln_type *bpl =  m_cln->a[0];

	p->x = LT2_F1_C( m_lf.lt );
	p->y = LT2_F2_C( m_lf.lt );


	float scale = hypot( m_lf.lt.a0, m_lf.lt.b0 );
	*r = 0.75 *m_r * scale ;

	return( 1 );
}