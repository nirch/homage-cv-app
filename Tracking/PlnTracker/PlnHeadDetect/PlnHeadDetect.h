
#ifndef _PLN_HEAD_DETECT_H_
#define _PLN_HEAD_DETECT_H_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnTrackerD.lib" )
#else
#pragma comment( lib, "PlnTracker.lib" )
#endif

#include "Uvec/Vec3d.h"

#include "Umath/LT2Type.h"


#include "../HeadPose/HeadPose.h"


#include	"Uln/Cln/ClnType.h"


typedef	struct  plnHeadDetectPrm_type {

	float dt;

	float	angle;


	float	aT;

	float du;


	// Filetr
	float scale;
	float qulity;

}  plnHeadDetectPrm_type;



class CPlnHeadDetect 
{
public:
	
	CPlnHeadDetect();

	virtual ~CPlnHeadDetect();

	void DeleteContents();


	int Init( cln_type *cln, char *prmFile );

	int Process( plnA_type *apl );

	pln_type*	GetHeadPln( headPose_type *h );

	int	Process( plnA_type *apl, vec2f_type ap[], int nP, int iFrame );

	int SetFittingMeasure( plnA_type *apl, headPose_type *h );

	int Filter( headPoseA_type *ah );

	int Get( headPose_type *h0, headPose_type *h );


	pln_type *	GetPln( headPose_type *h );

	int	Write( char *outFile );

	void Trace( FILE *fp );


	plnA_type *	CropApl( plnA_type *apl, headPose_type *h, float margin );
private:




protected:

	plnHeadDetectPrm_type *m_prm;


	float m_size;

	int	m_iFrame;



	vec2i_type	m_v;


	cln_type *m_cln;

	box2f_type	m_box;
	float m_r;
	vec2f_type m_p0;

	lt2_type	m_lt;


public:
		headPoseA_type	*m_ah;

};


int	pln_head_detcet( plnA_type *apl,  plnHeadDetectPrm_type *prm );



plnHeadDetectPrm_type *	plnHeadDetectPrm_alloc();

void	plnHeadDetectPrm_set( plnHeadDetectPrm_type *prm, float dT, float angle );

void	plnHeadDetectPrm_aux( plnHeadDetectPrm_type *prm );

int	plnHeadDetectPrm_read(  plnHeadDetectPrm_type *prm, char *file );

int	plnHeadDetectPrm_read(  plnHeadDetectPrm_type *prm, struct xmlTag_type *pTag );



#endif


