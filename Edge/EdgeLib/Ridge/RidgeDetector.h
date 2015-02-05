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

	~CRidgeDetector();

	void DeleteContents();

	void Init( char *prmFile );

	void Init( pRidgePrm_type *prm );


	int Detect( image_type *sim, image_type *mim, box2i *box );

	int Detect( image_type *sim, image_type *mim );


	int SetMask( image_type *sim, image_type *mim );

	int Union( float lenT, float dT, float aT );



	int Write( char *file );


	void Draw( struct gpw_type *gpw, int fLine, int fCircle );

	plnA_type * Get( float tLen );



	void Trace( LogCallback logCallback, vec2f_type *p, float D);

	void TraceStatus( LogCallback logCallback );
public:


private:
	

public:


	pRidgePrm_type	*m_prm;

	box2i m_box;


	image_type *m_rim;

	image_type *m_rmim;

	plRidgeA_type *m_ar;
};










#endif