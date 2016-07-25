
#ifndef _UNIFORMA_BACKGROUND_H_
#define _UNIFORMA_BACKGROUND_H_



#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "MattingLibD.lib" )
#else
#pragma comment( lib, "MattingLib.lib" )
#endif



#include "Uvec/Vec3d.h"
#include "Utime/GpTime.h"

#include "Uln/Cln/ClnType.h"

#include "ImageType/ImageType.h"


typedef struct ubHeadPrm_type {
	int	on;
} ubHeadPrm_type;


typedef struct ubPrm_type {
	int thin;

	int fillBlob;
	int fillBlobAll;


	int enableEdge;

	float in_d;

	int smooth;

	int contour;

	ubHeadPrm_type	head;

} ubPrm_type;








class CUniformBackground 
{
public:
	
	CUniformBackground();

	~CUniformBackground();

	void DeleteContents();

	void	SetRoi( box2i_type *b );

	void	SetFlip( int flip )	{ m_flip = flip; }

	int	Init( char *xmlFile, char *ctrFile, int width, int height );

	int	InitHeadTracker( int iHead );


	int	ReadMask( char *inFile, int width, int height );

	int	ResetBackground();


	int ProcessBackgroundState( image_type *sim, int iFrame );

	int ProcessBackground( image_type *sim, int iFrame );
	
	int	ProcessInitBackground( image_type *sim );

	int	ProcessInitBackground( image_type *sim, image_type *mim, int fState = 0 );

	int	ProcessInitBackground_lern( image_type *sim, image_type *m_bim0, int a0[], int a1[],
				struct segC_type ac0[], struct segC_type ac1[], int step );


	int ProcessPl(  image_type *sim, int iFrame, plnA_type *apl );


	int	Process( image_type *sim, int iFrame, image_type **cim );

	int	DynamicPose( char *outFile );


	void SetDframe( int iFrame )	{ m_dFrame = iFrame; }
	void SetContour( int contour );//	{ m_contour = contour; }

	int	WriteContour( char *file );

	int	Write( char *file );

	int	WritePlf( char *outFile );

	void	Trace( FILE *fp );

	char * GetProcessLog();

	void ProcessLog();

	image_type *GetImage(  int color, image_type *im );

	image_type *GetImage(  image_type *bim, image_type *im );
	image_type * GetImageAlpha( image_type *im );

	int GetProcessBackgroundSimilarity();

	int	GetHeadPose( vec2f_type *p, float *angle, float *scale );

protected:

	int	ProcessCompare( image_type *sim );

	int	ProcessBn( image_type *sim, float bnT );

	int	ProcessUpdate( image_type *sim );


	int	ProcessBlob();

	int	ProcessBlob1();

	int	ProcessBlob2();

	int	ProcessThin( int N );

	int	ProcessSmooth();

	int	ProcessContour();
public:
	int	ProcessContourAdjust( plnA_type *apl, int width, int height );

private:
	int	ProcessEdgeContourInit();
	int	ProcessEdgeContour();




	int	ProcessDynamicMaskInit();
	int	ProcessDynamicMask();



	
	int	ReadPrm( char *inFile );

	int	ReadHeadTagPrm( struct xmlTag_type *pTag, ubHeadPrm_type *h );



	class CUnBackground *m_unBackground;

	class CPlnHeadTracker *m_headTracking;

	int m_headBoxF;
	box2f_type m_headBox;

	int	m_iHead;

	ubPrm_type *m_prm;

	int	m_iFrame;
	int	m_dFrame;

	int m_flip;

	float m_T;  // threshold for grey level difference for compare
	
	float m_bT;

	float m_bnT;

	int m_contour;



	box2i_type	m_roi;
	int	m_N;


	image_type	*m_bim;


	image_type	*m_bimC;


	image_type *m_sim;

	image_type *m_yim;
	image_type *m_bnIm;
	image_type *m_bnImD;

	image_type *m_eim;


	struct imageLabel_type *m_abwBlob;

	struct imageLabel_type *m_abw;

	struct imageLabel_type *m_abwC;
	struct cln_type *m_cln;

	plnF_type	*m_fpl;
	plnF_type	*m_fplH;
	plnF_type	*m_fplEdge;

	plnA_type *m_aplEdge;


public:
	class CRidgeDetector *m_dr;

	class CDynamicMask	*m_dm;

	image_type *m_cim;
	image_type *m_dim;

	image_type	*m_cimS;	// smooth m_cim

	image_type	*m_cim8;


	clnA_type *m_ac;
	int	m_nM;
	image_type	*m_mim[4];	// silhate

	int	m_state;


	vec2f_type m_mp;


	gpTime_type	m_rTime;
	gpTime_type	m_tCompare;
	gpTime_type	m_tUpdate;
	gpTime_type	m_gTime;
	gpTime_type m_tThin;
	gpTime_type m_tSmooth;

	gpTime_type m_tEdge;

	gpTime_type	m_tBn;
	gpTime_type	m_tCln;

	gpTime_type	m_tOpen;
	gpTime_type	m_tBlobR;
	gpTime_type	m_tBlobF;
	gpTime_type	m_tBlobEdge;
	gpTime_type	m_tBlob;



	char m_processLog[1024];
};



ubPrm_type *	ubPrm_alloc();

void	ubPrm_destroy( ubPrm_type *prm );


#endif


