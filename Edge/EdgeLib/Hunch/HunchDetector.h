/*********************
 ***  VaHolding.h  ***
 *********************/


#ifndef _HUNCH_DETECTOR_H_
#define _HUNCH_DETECTOR_H_



#include "ImageType/ImageType.h"
#include "pHunchType.h"





class CHunchDetector
{
public:

	CHunchDetector();

	~CHunchDetector();

	void DeleteContents();

	void Init( char *prmFile );

	void Init( pHunchPrm_type *prm );

	void Init();


	int Detect( image_type *sim, box2i_type *box );

	int DetectL( image_type *sim, box2i_type *box );

//	int DetectL( image_type *yim, box2i_type *box, pHunchA_type *ah );


	int Write( char *file );


	void Draw( struct gpw_type *gpw, int fCircle );

	
	void Trace( LogCallback logCallback, vec2f_type *p, float D);

	void TraceStatus( LogCallback logCallback );
public:


private:
	

public:


	pHunchPrm_type	*m_prm;

	box2i m_box;


	pHunchA_type *m_ah;

	struct jet2App_type *m_ja[4];
};










#endif