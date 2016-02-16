
#ifndef _MATTING_CONTOUR_H
#define _MATTING_CONTOUR_H


#include "Uvec/Vec3d.h"
#include "Utime/GpTime.h"

#include "Uln/Cln/ClnType.h"

#include "ImageType/ImageType.h"



typedef struct mcPrm_type {
	int thin;

	int fillBlob;
	int fillBlobAll;


	int enableEdge;

	float in_d;

	int smooth;


} mcPrm_type;








class CMattingContour 
{
public:
	
	CMattingContour();

	~CMattingContour();

	void DeleteContents();


	int	Init( char *xmlFile, char *ctrFile, int iHead, int width, int height );

	int	InitHeadTracker( int iHead );


	int	Read( char *inFile );

	int	Process( int iFrame );

	int	Process( plnA_type *apl, int iFrame );


	int	ProcessHead( plnA_type *apl, int iFrame );

	int	Write( char *file );

	void	Trace( FILE *fp );

	char * GetProcessLog();

	void ProcessLog();

	image_type *GetAlpha();



public:
	int	ProcessAdjust( plnA_type *apl );

	int	ProcessAlpha( plnA_type *apl );

private:


	int	ReadPrm( char *inFile );


	class CPlnHeadTracker *m_headTracking;

	int m_headBoxF;
	box2f_type m_headBox;

	int	m_width;
	int	m_height;



	mcPrm_type *m_prm;

	int	m_iFrame;
	int	m_dFrame;



	
	plnF_type	*m_fpl;


	struct cln_type *m_cln;

//	plnF_type	*m_fpl;
	plnF_type	*m_fplH;
	plnF_type	*m_fplEdge;

	plnA_type *m_aplEdge;


public:





	image_type	*m_cim;	// smooth m_cim
	image_type	*m_cimS;	// smooth m_cim


	clnA_type *m_ac;


	int	m_state;


	vec2f_type m_mp;


	gpTime_type	m_gTime;


	gpTime_type	m_tCln;


	char m_processLog[1024];
};



mcPrm_type *	mcPrm_alloc();

void	mcPrm_destroy( mcPrm_type *prm );


#endif


