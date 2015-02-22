
#ifndef _HR_SOURCE_I_H
#define _HR_SOURCE_I_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"





class CHrSourceI 
{
public:
	
	CHrSourceI();

	~CHrSourceI();

	void DeleteContents();



	virtual int	ReadFrame( int iFrame, image_type **im ) = 0;

	virtual int Close() = 0;

	void SetAlpha( image_type *im );

	void MergeAlpha( image_type *sim );

private:

	int	m_iFrame;

	image_type *m_alphaIm;




};


image_type *	image3_green_alpha( image_type *sim, image_type *im );


#endif


