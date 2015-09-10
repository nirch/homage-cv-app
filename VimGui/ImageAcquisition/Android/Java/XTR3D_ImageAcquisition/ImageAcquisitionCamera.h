//
//  ImageAcquisition.h
//  ImageAcquisition
//
//  Created by Admin on 5/7/13.
//
//

// ImageAcquisitionCamera.h : Declares CImageAcquisitionCamera class
#pragma once

#include "ImageAcquisitionBase.h"
#include "IImageAcquisitionCamera.h"
#include "ColorConversions.h"
#include "CameraImpl.hpp"
#include "OS_PerformanceLogger.h"

#define IAMVideoProcAmp_OFFSET 100

typedef enum tagCameraConfigAction_e
	{	
		CG_NOT_MODIFY =  0x0,
		CG_AUTO		 = 	0x1,
		CG_MANUAL    =	0x2,
		CG_AUTO_FIX	 = 	0x3,
		CG_READ_FIX  =	0x4

	} CameraConfigAction_e;

typedef enum tagCameraConfigFields_e
	{			 
		 CAM_CNG_EXPOSURE		=	0, 
		 CAM_CNG_FOCUS			=	1,		 		 
		 CAM_CNG_SHARPNESS      =   2,  
		 CAM_CNG_WHITE_BALANCE  =   3,	
		 CAM_CNG_ANTI_FLIKER    =   4
	} CameraConfigFields_e;

#define CAM_CNG_NUM_FIELDS          5

#define CAMERA_ANTI_FLICKER_OFF		0
#define CAMERA_ANTI_FLICKER_50		1
#define CAMERA_ANTI_FLICKER_60		2

const char cameraConfigFieldsStr[CAM_CNG_NUM_FIELDS][25] = {{"CAM_CNG_EXPOSURE"},{"CAM_CNG_FOCUS"},{"CAM_CNG_SHARPNESS"},{"CAM_CNG_WHITE_BALANCE"},{"CAM_CNG_ANTI_FLIKER"}};

#define MAX_CAMERA_NUM 10
#define MAX_CAMERA_NAME_LENGTH 256

typedef struct _CALIB_OVERRIDE 
{
	CameraConfigAction_e		state;	
	int							value;
} CALIB_OVERRIDE;

typedef enum tagCameraProperties_e
    {	
		CAMERA_PROPERTIES_PAN	 =	 0,
		CAMERA_PROPERTIES_TILT	 =   1,
		CAMERA_PROPERTIES_ROLL	 =   2, 
		CAMERA_PROPERTIES_ZOOM	 =   3,
		CAMERA_PROPERTIES_EXPOSURE = 4,
		CAMERA_PROPERTIES_IRIS	 =   5,
		CAMERA_PROPERTIES_FOCUS	 =   6, 
		
		
		CAMERA_PROPERTIES_BRIGHTNESS				= 100, // IAMVideoProcAmp_ENUM
		CAMERA_PROPERTIES_CONTRAST					= 101,
		CAMERA_PROPERTIES_HUE						= 102,
		CAMERA_PROPERTIES_SATURATION				= 103,
		CAMERA_PROPERTIES_SHARPNESS					= 104,
		CAMERA_PROPERTIES_GAMMA						= 105,
		CAMERA_PROPERTIES_COLORENABLE				= 106,
		CAMERA_PROPERTIES_WHITE_BALANCE				= 107,	
		CAMERA_PROPERTIES_BACKLIGHT_COMPENSATION	= 108,
		CAMERA_PROPERTIES_GAIN						= 109		
    } 	CameraProperties_e;

typedef 
enum tagCameraControlProperty
    {	CameraControl_Pan	= 0,
	CameraControl_Tilt	= ( CameraControl_Pan + 1 ) ,
	CameraControl_Roll	= ( CameraControl_Tilt + 1 ) ,
	CameraControl_Zoom	= ( CameraControl_Roll + 1 ) ,
	CameraControl_Exposure	= ( CameraControl_Zoom + 1 ) ,
	CameraControl_Iris	= ( CameraControl_Exposure + 1 ) ,
	CameraControl_Focus	= ( CameraControl_Iris + 1 ) 
    } 	CameraControlProperty;

typedef 
enum tagCameraControlFlags
    {	CameraControl_Flags_Auto	= 0x1,
	CameraControl_Flags_Manual	= 0x2
    } 	CameraControlFlags;

// Maps configuration fields from xml file to appropriate camera property.
const CameraProperties_e mapConfFieldCameraProperties[CAM_CNG_NUM_FIELDS-1] = { CAMERA_PROPERTIES_EXPOSURE  ,
																				CAMERA_PROPERTIES_FOCUS     ,
																				CAMERA_PROPERTIES_SHARPNESS ,
                                                                                CAMERA_PROPERTIES_WHITE_BALANCE};

/*
	Image Acquisition Camera Class - Definition for iOS
*/
class IMAGE_ACQUISITION_API CImageAcquisitionCamera : 
	public CImageAcquisitionBase, 
	public IImageAcquisitionCamera
{		                                                       
public:
    DECLARE_CLASS_LOGGER();

	CImageAcquisitionCamera();
    virtual ~CImageAcquisitionCamera() {};

	// IImageAcquisition implementation
	virtual XTR3D_ERROR_e Init();     
	virtual XTR3D_ERROR_e Close();
	virtual XTR3D_ERROR_e Start();    // Starts camera video acquisition
	virtual XTR3D_ERROR_e Stop();     // Stops camera video acquisition
	virtual XTR3D_ERROR_e ReadASample(SImageHeader* imageHeader);
	virtual XTR3D_ERROR_e SetImageParameters(SImageInfo* info) {return XTR3D_ERROR_OK;}
	virtual int GetHeight() {return 480;}
	virtual int GetWidth() {return 640;}
	virtual int GetBitsPerPixel() {return 24;}
    virtual int GetImageSize() {return 640*480*24/3;}
    virtual unsigned long GetFourCC() {return 0;}


    virtual HRESULT CalibratePowerLineFrequency(long newValue) {return S_OK;}
    virtual XTR3D_ERROR_e SetCameraSymbolicLink(wchar_t* cameraSymbolicLink) {return XTR3D_ERROR_OK;}

	//TODO: Hack to allow StartPreviewAsync to work, by providing the UI element from app which is responsible for renderering the preview stream:
	//Start HACK
	XTR3D_ERROR_e SetRenderer(IUnknown* rendererFromApp);
	//End HACK

protected:
    void PrepareOutputImage( unsigned char* pImage, unsigned char* pImageOut );

	CALIB_OVERRIDE	m_calibOverrideArr[CAM_CNG_NUM_FIELDS];
	bool			m_isRunning;         // Logical flag notes if capturing process is running.
	bool			m_isCalibrating;

	long			m_nRefCount;  
	int				m_calibrationDuration;
	int				m_exposureOffset; // Used in special case to tune auto exposure of the webcam. 
									  // In Logitech C310 reading of exposure value and writing it back fails probably due to different 
									  // granuality of the parameter scale...
	ColorConversion m_colorConversion;
	unsigned char	m_ImageTmp[640*480*4];
	int				m_frameId;

private:
    static void OnReadSample(void*, unsigned char*, long long int);
	CameraImpl* m_cameraImpl;
	CPerformanceLogger* m_logTimeStamp;
};

