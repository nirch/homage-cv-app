
#ifndef _FINGER_H_
#define _FINGER_H_




#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"
#include "Uvl/Vl2fType.h"
#include "Ujet1/Jet1Type.h"
#include "Uln/PlnType.h"
#include "Image2/BwLabel.h"

#include "Lk/LkType.h"


typedef struct FingerPrm_type {

	int	fingerSize;

	float	redF;


	float	skinT;

} FingerPrm_type;



typedef struct fingerModel_type {

	float	scale;

	pln_type	*pl;

	float		gt;

	float		len;

	float		gt0,	gt1;

} fingerModel_type;



typedef struct FingerPose_type {


	int	state;

	vl2f_type	vl;

	vl2f_type	vl0;
	vl2f_type	vl1;

	jet1_type	jet;



	vec2f_type	p;
	vec2f_type	v;

	vec2f_type	p0;


	float	scale;


} FingerPose_type ;



typedef  struct fingetFit_type {
	int	state;

	float	c0;
	float	c1;
	float	dis;

	lt2_type	lt;

} fingetFit_type;



class CFinger 
{
public:
	
	CFinger();

	~CFinger();


	int		Init( char *prm, int iLen );
	int		InitPrm( char *file );

	int		SetMask( image_type *mim );




	int		Process( image_type *sim, box2i_type *b, int orient, int iFrame );

	int		Process( image_type *sim, int iFrame = -1 );


	int		CaliberatePoint( vec2f_type *p );



	int	GetEdgeMask( image_type **im );

	void Draw( struct gpw_type *gpw );


	void	SetDumpFrame( int dFrame )	{ m_dFrame = dFrame; }
	void	SetBg( int flag )	{ m_bg = flag; }



	int SetFinger( FingerPose_type *pose );
	int GetFinger( FingerPose_type **pose );
	int GetFinger( FingerPose_type *pose );

	int GetFinger( vec2f_type *p, vec2f_type *v );



	void	Trace( FILE *fp );




private:
	int		ProcessBacground( image_type *sim );

	int		ProcessSkin( image_type *sim );

	int		ProcessRed( image_type *sim );

	int		ProcessEdge( image_type *sim );

		
	int		ProcessLine( image_type *sim );


	int		ProcessRidge( image_type *sim );

	int		ProcessRidge_final();
	int		PrepareRidgeImage( image_type *sim, int fSkin, image_type **eim, image_type **mim );
	int		SetFingerRidge( FingerPose_type *pose );
	int		DrawRidge( gpw_type *gpw );


	int		ProcessFitInit( char *inFile  );
	int		ProcessFit( image_type *sim );

	int		ProcessFit( plnA_type *apl, pln_type *pl, fingetFit_type *ft );

	int		ProcessFit( plnA_type *apl, fingetFit_type *ft0, fingetFit_type *ft );

	int		ProcessFit( plnA_type *apl, fingerModel_type *fm, vec2f_type *mp, fingetFit_type *ft );

	int		ProcessFit( pln_type *pl, fingerModel_type *fm, vec2f_type *mp, fingetFit_type *ft );

	int		ProcessFitChoose();


	int		SetFingerFit( FingerPose_type *pose );
	int		GetFingerFit( vec2f_type *p, vec2f_type *v, vec2f_type *p0 );
	int		DrawFit( gpw_type *gpw );
	int		FindLine();

	void	DrawBackground( gpw_type *gpw );

	
	int		ProcessLine_end( image_type *im );

	void	ScaleUpLine( float scale );


	int		ProcessLkSet( image_type *sim, FingerPose_type *pose );

	int		ProcessLkInit( image_type *sim, vec2f_type *p );

	int		ProcessLk( image_type *im, vec2f_type *dv, vec2f_type *p, int iFrame );

	int		SetFingerLk( FingerPose_type *pose );

	int		DrawLk( gpw_type *gpw );


private:

public:

	FingerPrm_type	m_prm;

	int	m_iFrame;
	int	m_dFrame;

	int m_orient;

	image_type	*m_sim;
	image_type	*m_yim;
	image_type	*m_bim;


	int	m_bg;
	image_type	*m_dim;
	image_type	*m_mim;	// mask
	image_type	*m_im;	// silatt


	image_type	*m_ebim;
	image_type	*m_eim;


	// skin
	image_type	*m_skinIm;



	// Ridge
	class CRidgeDetector *m_dr;

	plnA_type *m_apl;

	int	m_scaleL;
	float	m_scale;
	float	m_scale_inv;
	

	// track
	fingerModel_type	m_fm[4];
	vec2f_type m_bp;



	class CLkPointTracker *m_lkp;
	lkState_type	m_lkS,	m_lkS0;



	lt2_type m_lt;

	
	fingetFit_type	m_ft;

	int	m_nFt;
	fingetFit_type	m_aFt[16];

	int	m_state;

	FingerPose_type m_pose;

	gpTime_type		m_gTime;
	gpTime_type		m_gtBg;
	gpTime_type		m_gtRed;
	gpTime_type		m_gtRidge;
	gpTime_type		m_gtFit;
	gpTime_type		m_gtSkin;



	bwLabel_type m_abw[16];
	int	m_nBw;


};


#endif


