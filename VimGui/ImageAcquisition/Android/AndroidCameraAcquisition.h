/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _ANDROID_CAMEAR_ACQUISITION_
#define _ANDROID_CAMEAR_ACQUISITION_



//#include "UThread/VMutex.h"
#include "UThread/VConditionMutex.h"

#include	"Uigp/igp.h"
#include "Utime/GpTime.h"

#include "../ImageAcquisition.h"

struct image_type;




class CAndroidCameraAcquisition: public CImageAcquisition
{
public:
	CAndroidCameraAcquisition();
	virtual ~CAndroidCameraAcquisition();	

	int		Open( char *ip, int portm, int raw = 0 );

	void	Close();


	
	int Get( image_type **im, int iFrame = -1 );


	void SetFrameSize( int width, int height );

	 
	void SetOrientation( int orientation ) { m_orientation = orientation; }


	int LogStatus();

	gpTime_type *GetTime()	{ return( &m_gTime ); }



	int PreviewCallBack( int width, int height, u_char *data );



protected:

	int WaitToImage();

	int m_capture;


	//CVMutex *m_mutex;
	CVConditionMutex *m_mutex;

	int m_orientation;
private:
	image_type *m_yuv;

	gpTime_type	m_gTime;
	
	gpTime_type	m_tCapture;

};

#endif




