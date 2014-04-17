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


#include	"Uigp/igp.h"

#include "../ImageAcquisition.h"

#import "TargetConditionals.h"

#if TARGET_OS_IPHONE
#import "CameraControllerIOS.h"
#else
#import "CameraControllerMac.h"
#endif

struct image_type;


#define HANDSET_IPHONE_4			0
#define HANDSET_IPHONE_4S		    1
#define HANDSET_IPHONE_5			2

#define HANDSET_IPAD_4              10
#define HANDSET_IPAD_5              11



class CCameraAcquisitionMac: public CImageAcquisition
{
public:
	CCameraAcquisitionMac();
	virtual ~CCameraAcquisitionMac();	

	int		OpenWithPreview( int iCamera = 0, CGRect *previewLayerRect = NULL);
    int		Open( int iCamera = 0 );

	void	Close()	{ m_cameraOpen = false; }

	
	int Get( image_type **im, int iFrame = -1 );
    
    
    
    void SetFoucusPixel( int x, int y );
    

    void SetResolution( int resolution );
    
    int SetHandset( int handset );
   
    int AddPreviewLayer( CGRect *rect );
    
#if TARGET_OS_IPHONE
    AVCaptureVideoPreviewLayer * GetPreviewLayer();
#endif

    float   GetCcdWidth()   { return(m_ccdWidth); }
    float   GetFocalLength()  { return( m_focalLength ); }

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
    
  
    int GetPlatform();

protected:
    char* platformString();         // must delete the returned value after using

private:
    int m_cWidth;
    int m_cHeight;
    
#if TARGET_OS_IPHONE    
    CameraControllerIOS *camera;
#else
    CameraControllerMac *camera;
#endif
    
    image_type *temp_img;
    
    int m_iCamera;
    int m_resolution;
    
    int m_protrait;
    
    bool m_cameraOpen;
    
    int   m_handset;
    float m_ccdWidth;
    float m_focalLength;
};

#endif




