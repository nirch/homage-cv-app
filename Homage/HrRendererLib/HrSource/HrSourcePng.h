
#ifndef _HR_SOURCE_PNG_H
#define _HR_SOURCE_PNG_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"
#include "Uvl/IntType.h"


#include "HrSourceI.h"



class CHrSourcePng : public CHrSourceI
{
public:
	
	CHrSourcePng();

	virtual ~CHrSourcePng();

	void DeleteContents();



	int Init( char *inFile );

	int Init( char *inFile, int aFrame[], int nF );

	int	ReadFrame( int iFrame, long long timeStamp, image_type **im );

	int	Close();

private:

//	int	m_iFrame;

	char m_dir[256];

	intA_type	*m_af;

	image_type *m_im;




};



#endif


