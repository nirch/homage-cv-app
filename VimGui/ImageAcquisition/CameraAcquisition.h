/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _CAMEAR_IMAGE_ACQUISITION_
#define _CAMEAR_IMAGE_ACQUISITION_


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "ImageAcquisitionD.lib" )
#else
#pragma comment( lib, "ImageAcquisition.lib" )
#endif



#include	"Uigp/igp.h"

#include "ImageAcquisition.h"

struct image_type;




class CCameraAcquisition: public CImageAcquisition
{
public:
	CCameraAcquisition();
	virtual ~CCameraAcquisition();	

	int		Open( int iCamera = 0 );

	void	Close()	{}

	
	int Get( image_type **im, int iFrame = -1 );


	int Calibrate();
	int CalibrateWhiteBalance();	
	int CalibrateSharpness();
	int CalibrateFocus();

	int CalibrateRightLigt();


	int CalibrateExposure();


	int CalibratePowerLineFrequency();



	int GetWhiteBalanceRange( long *min, long *max );

	int GetWhiteBalance( long *value, long *flag );

	int SetWhiteBalance( long value, long flag );

	int GetGainRange( long *min, long *max );

	int GetGain( long *value, long *flag );

	int SetGain( long value, long flag );



	int GetExposureRange( long *min, long *max );

	int GetExposure( long *value, long *flag );

	int SetExposure( long value, long flag );



	int LogStatus();

	int LogStatus( char *fname );


protected:

#ifdef WINCE
	class CCameraGrabberWM	*m_cameraGrabber;
#else
	class CCameraGrabber	*m_cameraGrabber;
#endif

//	image_type *m_image;

//	int	m_rWidth;
//	int m_rHeight;
private:


};

#endif




