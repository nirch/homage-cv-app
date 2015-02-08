
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



private:

	int	m_iFrame;





};


#endif


