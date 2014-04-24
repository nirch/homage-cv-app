
#ifndef _COLOR_BACKGROUND_H_
#define _COLOR_BACKGROUND_H_


#include "Uvec/Vec3d.h"
#include "Utime/GpTime.h"

#include "Uln/Cln/ClnType.h"

#include "ImageType/ImageType.h"


typedef	struct bImage_type {
	int n;
	int	r;
	int g;
	int b;
} bImage_type;	





class CUniformBackground 
{
public:
	
	CUniformBackground();

	~CUniformBackground();

	void DeleteContents();

	void	SetRoi( box2i_type *b );

	int	ReadMask( char *inFile, int width, int height );

	int ProcessBackground( image_type *sim, int iFrame );
	
	int	ProcessInitBackground( image_type *sim, image_type *mim, int fState = 0 );


	int	Process( image_type *sim, int iFrame, image_type **cim );


	void SetDframe( int iFrame )	{ m_dFrame = iFrame; }
	void SetContour( int contour )	{ m_contour = contour; }

	int	WriteContour( char *file );

	int	Write( char *file );

	void	Trace( FILE *fp );

	char * GetProcessLog();

	void ProcessLog();
protected:

	int	ProcessCompare( image_type *sim, image_type **cim );

	int	ProcessBn( image_type *sim, float bnT );

	int	ProcessUpdate( image_type *sim );


	int	ProcessBlob();

	int	ProcessBlob1();

	int	ProcessBlob2();

	int	ProcessThin( int N );

	int	ProcessSmooth();

	int	ProcessContour();

	int	ProcessContourUI();
	






	int	m_iFrame;
	int	m_dFrame;

	float m_T;
	
	float m_bT;

	float m_bnT;

	int m_contour;

	vec2i_type	m_v;

	box2i_type	m_roi;
	int	m_N;

//	image_type	*m_mim;	// silhate

	image_type	*m_bim;
	
	image_type	*m_bimDx;

	image_type	*m_bimC;



	image_type *m_yim;
	image_type *m_bnIm;
	image_type *m_bnImD;

	image_type *m_eim;


	struct imageLabel_type *m_abwBlob;

	struct imageLabel_type *m_abw;

	struct imageLabel_type *m_abwC;
	struct cln_type *m_cln;

	plnF_type	*m_fpl;


public:
	image_type *m_cim;
	image_type *m_dim;

	image_type	*m_cimS;	// smooth m_cim

	image_type	*m_cim8;

	image_type	*m_mim;	// silhate

	int	m_state;



	gpTime_type	m_rTime;
	gpTime_type	m_tCompare;
	gpTime_type	m_tUpdate;
	gpTime_type	m_gTime;
	gpTime_type m_tThin;
	gpTime_type m_tSmooth;

	gpTime_type	m_tBn;
	gpTime_type	m_tCln;

	gpTime_type	m_tOpen;
	gpTime_type	m_tBlobR;
	gpTime_type	m_tBlobF;
	gpTime_type	m_tBlobEdge;
	gpTime_type	m_tBlob;



	char m_processLog[1024];
};


#endif


