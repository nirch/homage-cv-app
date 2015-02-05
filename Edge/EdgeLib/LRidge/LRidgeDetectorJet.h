/*****************************
 ***  LRidgeDetectorJet.h  ***
 *****************************/


#ifndef _LRIDGE_DETECTOR_JET_H_
#define _LRIDGE_DETECTOR_JET_H_




#include "ImageType/ImageType.h"





class CLRidgeDetectorJet
{
public:

	CLRidgeDetectorJet();

	~CLRidgeDetectorJet();

	void DeleteContents();

	void Init( char *prmFile, int width, int height );




	int Detect( image_type *sim, float d, int color, vec2f_type *P, vec2f_type *V, float *f, float *dx );


	float	DetectStep( struct jet2App2_type *ja, image_type *sim, vec2f_type *p, vec2f_type *v );



	int Write( char *file );



public:


private:

	struct jet2App2_type *m_ja;

	int	m_width;
	int	m_height;
	image_type *m_im;



	

public:


};










#endif