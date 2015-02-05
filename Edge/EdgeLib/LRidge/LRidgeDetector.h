/**************************
 ***  LRidgeDetector.h  ***
 **************************/


#ifndef _LRIDGE_DETECTOR_H_
#define _LRIDGE_DETECTOR_H_



#include "Uln/PlnType.h"
#include "ImageType/ImageType.h"





class CLRidgeDetector
{
public:

	CLRidgeDetector();

	~CLRidgeDetector();

	void DeleteContents();

	void Init( char *prmFile, int width, int height );




	int Detect( image_type *sim, float d, int color, vec2f_type *P, vec2f_type *V, float *f, float *dx );

	float Detect_1( image_type *sim, vec2f_type *p, vec2f_type *v );


	int DetectT( image_type *sim, float d, int color, vec2f_type *P, vec2f_type *V );

	float Detect_2( image_type *sim, vec2f_type *p, vec2f_type *v );



	int Write( char *file );



public:


private:

	struct lkImage_type *m_lkM;

	int	m_width;
	int	m_height;
	image_type *m_im;



	

public:


};










#endif