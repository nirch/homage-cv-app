
#ifndef _BACKGROUND_REMOVAL_H_
#define _BACKGROUND_REMOVAL_H_






#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"
#include "Uln/Cln/ClnType.h"

#include "ImageLabel/SbType.h"
#include "PlnTracker/Pose2dType.h"


typedef struct unDarkPrm_type {
	int T0;
	int	T1;
	float F0;
	float F1;
} unDarkPrm_type;


typedef struct unPrm_type {
	int	straightLine;


	int fillBlob1;


	unDarkPrm_type	d;

} unPrm_type;




class CUnBackground 
{
public:
	
	CUnBackground();

	~CUnBackground();

	void DeleteContents();



	int	Init( char *xmlFile, char *ctrFile, int width, int height );

	int	ReadPrm( char *inFile );


	int	ReadMask( char *inFile, int width, int height );

	int Process( image_type *sim, int iFrame );

	int GetState()	{ return( m_state ); }
	int GetHead()	{ return( m_iHead ); }
	int GetSimilarity()	{ return( m_similarity ); }

	image_type *	GetBim( image_type *bim  );

	void	Trace( FILE *fp )	{}
private:
	int	ProcessBlob( image_type *sim );

	int	ProcessBlobB();

	int	ProcessBlobC();

	int	ProcessLineInit( char *prmFile );
	int ProcessLine( int k0, int k1 );
	int	ProcessLineMask( struct imageLabel_type *abw, int k0, int k1, vl2f_type *vl );

	int	ProcessBlobSide( struct imageLabel_type *abw, int iB, int x0, int y0, int j0, int j1, sbA_type *as );
	int	ProcessBlobSide_test( imageLabel_type *abw, int iB, int j0, int j1, sbA_type *as );

	int	ProcessBlobSide_gap( imageLabel_type *abw, int iB, int j0, int j1, sbA_type *as );

	int	ProcessBlobSide_overlap_up( imageLabel_type *abw, int iB, int i0, int j0, int j1, sbA_type *as );

	int	ProcessBlobSide_overlap_down( imageLabel_type *abw, int iB, int i0, int j0, int j1, sbA_type *as );

	int	ProcessFill();

	int	TestDrakness( image_type *sim );

	int	ProcessState();



	int	ProcessSilhouette();

	int	ProcessBlobB_TestBlob( int i0, int i1 );;

	int	TestSilhouette( sbA_type *as, imageLabel_type *abw );
	int TestSilhouetteHead( sbA_type *as, int i1, imageLabel_type *abw, float nw[] );

	int	TestShadowIn();




	int TestSilhouetteDeviation();
	sbA_type SelectBiggerContour(sbA_type contourArray[4]);
	//old
	int	ProcessInitBackground( image_type *sim, image_type *mim, int fState );

	int	ProcessInitBackground_lern( image_type *sim, image_type *m_bim0, int a0[], int a1[], struct segC_type ac0[], struct segC_type ac1[], int step );



	int	ProcessBlobD();

	int	ProcessBlobD_figure_blob( imageLabel_type *abw, sbA_type *as );

	pt2dA_type * ProcessBlobD_point( imageLabel_type *abw, pt2dA_type *apt );


	int	ProcessCloseUp();

private:
	int	m_iFrame;

	unPrm_type *m_prm;


	// silhouette 
	clnA_type *m_ac;
	int	m_nM;
	image_type	*m_mim[4];
	sbA_type	m_asB[4];
	int m_sWeight[4];



	vec2f_type m_hp;
	float m_hd[4];
	box2f_type	m_hbox[4];

	int	m_iHead;


	image_type *m_yim;  // Greylevel image of the current image
	image_type *m_bim;  // Background estimation
	image_type *m_bimD; // Edge image of 8X8 pixels (only X)



	box2i_type	m_roi;  // relevant box - today it is all the image
	int	m_N;

	class CUnEdge  *m_ue;


	struct imageLabel_type *m_abw;


	sbA_type	m_as0,	m_as1;


	sbA_type	m_asFigure;

public:
	pt2dA_type *m_apt;
	image_type *m_imMask;

	int	m_state;

	int	m_similarity;

	int	m_closeUp;

	pose2d_type m_head;



	gpTime_type m_gTime;

	gpTime_type m_tEdge;


};


unPrm_type *	unPrm_alloc();

void	unPrm_destroy( unPrm_type *prm );



#endif


