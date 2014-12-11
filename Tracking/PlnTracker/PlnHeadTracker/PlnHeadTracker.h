
#ifndef _PLN_HEAD_TRACKER_H_
#define _PLN_HEAD_TRACKER_H_


#include "Uvec/Vec3d.h"



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

	int	ProcessS( plnA_type *apl, vec2f_type *p, pln_type **spl, int iFrame );

	int	ProcessA( plnA_type *apl, pln_type **spl, int iFrame );

	int	Process( plnA_type *apl, pln_type **spl, int iFrame );


	int	Process( pln_type *pl, pln_type **spl, int iFrame );


	int	HeadRange( plnA_type *apl, float *gt0, float *gt1 );

	int	GetHeadPose( vec2f_type *p, float *r );


	int HeadPose( pln_type *pl, pln_type **spl, vec2f_type *p );

	static int HeadPose( pln_type *pl, vec2f_type *p );

	pln_type *	GetPln();


	float GetCover()	{ return( m_lf.cover);}


protected:

	int	m_iFrame;



	vec2i_type	m_v;


	cln_type *m_cln;

	box2f_type	m_box;
	float m_r;
	vec2f_type m_p0;


	lnFit_type m_lf;

	//pln_type *m_pl;


public:
	

};


#endif


