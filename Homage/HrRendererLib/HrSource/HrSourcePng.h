
#ifndef _HR_SOURCE_PNG_H
#define _HR_SOURCE_PNG_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrSourceI.h"



class CHrSourcePng : public CHrSourceI
{
public:
	
	CHrSourcePng();

	~CHrSourcePng();

	void DeleteContents();



	int Init( char *inFile );

	 int	ReadFrame( int iFrame, image_type **im );

	int	Close();

private:

	int	m_iFrame;

	char m_dir[256];

	image_type *m_im;


};



#endif


