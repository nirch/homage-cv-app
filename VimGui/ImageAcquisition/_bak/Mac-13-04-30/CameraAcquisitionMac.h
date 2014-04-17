/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _CAMEAR_ACQUISITION_MAC_
#define _CAMEAR_ACQUISITION_MAC_


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "ImageAcquisitionD.lib" )
#else
#pragma comment( lib, "ImageAcquisition.lib" )
#endif

#import <UIKit/UIKit.h>


#include	"Uigp/igp.h"

#include "../ImageAcquisition.h"

#import "CameraControllerMac.h"

struct image_type;




class CCameraAcquisitionMac: public CImageAcquisition
{
public:
	CCameraAcquisitionMac();
	virtual ~CCameraAcquisitionMac();	

	int		Open( int iCamera = 0 );

	void	Close()	{ m_cameraOpen = false; }

	
	int Get( image_type **im, int iFrame = -1 );
    
    
    
    void SetFoucusPixel( int x, int y );
    

    void SetResolution( int resolution );    
   
    int AddPreviewLayer( CGRect *rect );
    
    AVCaptureVideoPreviewLayer * GetPreviewLayer();



	int GetWhiteBalance( long *value, long *type );

	int SetWhiteBalance( long value, long type );



	int GetFocus( long *value, long *type );

	int SetFocus( long value, long type );
    
    int SetAutoFocus( float x, float y );   
    int SetContinuousAutoFocus( float x, float y );

    
    int GetFocusState();



	int GetSharpness( long *value, long *type );

	int SetSharpness( long value, long type );



	int GetPowerLineFrequency( long *value, long *type );

	int SetPowerLineFrequency( long value, long type );



	int GetExposure( long *value, long *type );

	int SetExposure( long value, long type );
    
    void SetProtrait( int flag ) { m_protrait = flag; }


	int LogStatus();
    
  

protected:

private:
    int m_cWidth;
    int m_cHeight;
    
    CameraControllerMac *camera;
    image_type *temp_img;
    
    int m_iCamera;
    int m_resolution;
    
    int m_protrait;
    
    bool m_cameraOpen;
};

#endif




