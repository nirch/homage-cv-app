
#ifndef _PLN_HEAD_TRACKER_H_
#define _PLN_HEAD_TRACKER_H_


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnTrackerD.lib" )
#else
#pragma comment( lib, "PlnTracker.lib" )
#endif

#include "Uvec/Vec3d.h"


#include "../HeadPose/HeadPose.h"


#include	"Uln/Cln/ClnType.h"


class CPlnHeadTracker 
{
public:
	
	CPlnHeadTracker();

	virtual ~CPlnHeadTracker();

	void DeleteContents();

	int Init( cln_type *cln );


	void	SetPose( float x, float y, float angle, float scale );
	
	int	GetPose( float *x, float *y, float *angle, float *scale );

	pln_type*	GetHeadPln( headPose_type *h );

	int	Process( plnA_type *apl, headPose_type *pose0, vec2f_type ap[], int nP, int iFrame );

	int	Process( plnA_type *apl, vec2f_type *p, pln_type **spl, int iFrame );

	//int	Process( plnA_type *apl, headPose_type *hp, pln_type **spl, int iFrame );
	int	Process( plnA_type *apl, headPose_type *hp, vec2f_type *p, int iFrame );


	int	ProcessTranslate( plnA_type *apl, headPose_type *pose0, int iFrame );

	int	ProcessPrev( plnA_type *apl, headPose_type *pose0, lnFit_type *lf, int iFrame );

	int	ProcessPrediction ( plnA_type *apl, headPose_type *pose0, vec2f_type *p, lnFit_type *lf, int iFrame );



	int	Fit( plnA_type *apl, headPose_type *hp, headPose_type *h, int iFrame );

	int	ProcessA( plnA_type *apl, pln_type **spl, int iFrame );

	int	Process( plnA_type *apl, pln_type **spl, int iFrame );


	int	Process( pln_type *pl, pln_type **spl, int iFrame );


	int	HeadRange( plnA_type *apl, float *gt0, float *gt1 );

	int	GetHeadPose( vec2f_type *p, vec2f_type *v, float *r );
	int	GetHeadPose( vec2f_type ap[4] );

	plnA_type *	CropApl( plnA_type *apl, headPose_type *h, float margin );


	int HeadPose( plnA_type *apl, headPose_type *h );

	int HeadPose( pln_type *pl, pln_type **spl, vec2f_type *p );

	int HeadPose( pln_type *pl, vec2f_type *p0, pln_type **spl, vec2f_type *p );

	int HeadPose( plnA_type *apl, vec2f_type *p0, pln_type **spl, vec2f_type *p );


	int HeadBox( pln_type *pl, float len, box2f_type *b );


	int HeadPose_peak( pln_type *pl, float gt,  pln_type **spl, vec2f_type *p );





	static int HeadPose( pln_type *pl, vec2f_type *p );

	pln_type *	GetPln();

	headPose_type *GetPose()	{ return( &m_pose ); }


	float GetCover()	{ return( m_lf.cover);}
	float GetCover1()	{ return( m_lf.cover1);}


protected:

	int	m_iFrame;



	vec2i_type	m_v;


	cln_type *m_cln;

	box2f_type	m_box;
	float m_r;
	vec2f_type m_p0;


	lnFit_type m_lf;


	headPose_type	m_pose;

	//pln_type *m_pl;


public:
	

};


#endif


