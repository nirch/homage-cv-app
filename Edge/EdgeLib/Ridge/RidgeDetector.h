/*********************
 ***  VaHolding.h  ***
 *********************/


#ifndef _RIDGE_DETECTOR_H_
#define _RIDGE_DETECTOR_H_



#include "Uln/PlnType.h"
#include "ImageType/ImageType.h"
#include "pRidgeType.h"
#include "plRidgeType.h"




class CRidgeDetector
{
public:

	CRidgeDetector();

	virtual ~CRidgeDetector();

	void DeleteContents();

	void Init( char *prmFile );

	void Init( pRidgePrm_type *prm );


	int Detect( image_type *sim, image_type *mim, box2i *box );

	int Detect( image_type *sim, image_type *mim );


	int SetMask( image_type *sim, image_type *mim, int add = 0 );

	int Union( float lenT, float dT, float aT );

	image_type *GetRimage( image_type *im );

	plnA_type * Component( float dT, float sLen, float mLen );


	int Write( char *file );


	void Draw( struct gpw_type *gpw, int fLine, int fCircle );

	plnA_type * Get( float tLen, float tLm, int fData );



	void Trace( LogCallback logCallback, vec2f_type *p, float D);

	void TraceStatus( LogCallback logCallback );

	void Trace( FILE *fp );
public:


private:
	

public:


	pRidgePrm_type	*m_prm;

	box2i m_box;


	image_type *m_rim;

	int m_nRmim;
	image_type *m_rmim[4];

	plRidgeA_type *m_ar;


	gpTime_type	m_tDetector;
	gpTime_type m_tLine;
};










#endif