
#ifndef _PLN_TRACKER_H_
#define _PLN_TRACKER_H_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnTrackerD.lib" )
#else
#pragma comment( lib, "PlnTracker.lib" )
#endif


#include "Uvec/Vec3d.h"


#include "../HeadPose/HeadPose.h"


#include	"Uln/Cln/ClnType.h"

#include "..\Pose2dType.h"


//typedef struct pose2f_type {
//
//	union {
//		struct {
//			float x;
//			float y;
//		};
//		vec2f_type p;
//	};
//
//	float scale;
//	float angle;
//
//	float qulity;
//
//
//} pose2f_type;





class CPlnTracker 
{
public:
	
	CPlnTracker();

	virtual ~CPlnTracker();

	void DeleteContents();

	int Init( cln_type *cln, int fCopy = 1 );


	void	SetPose( float x, float y, float angle, float scale );
	
	int	GetPose( float *x, float *y, float *angle, float *scale );

	pln_type*	GetHeadPln( headPose_type *h );


	int	Process( plnA_type *apl, pose2f_type *pose0, pose2f_type *pose, int iFrame );




	pln_type*	GetPln( pose2f_type *h );
	pln_type *	GetPln();

	pose2f_type *GetPose()	{ return( &m_pose ); }


	float GetCover()	{ return( m_lf.cover);}
	float GetCover1()	{ return( m_lf.cover1);}


protected:

	int	m_iFrame;


	cln_type *m_cln;

	box2f_type	m_box;
	float m_r;
	vec2f_type m_p0;


	lnFit_type m_lf;


	pose2f_type	m_pose;

	int	m_cycle;

public:
	

};


#endif


