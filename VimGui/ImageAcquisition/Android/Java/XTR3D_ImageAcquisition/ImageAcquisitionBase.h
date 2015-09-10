// ImageAcquisition.h : Declares IImageAcquisitionBase class

#pragma once

#include "ImageAcquisition.h"
#include "ImageHeader.h"

class IMAGE_ACQUISITION_API CImageAcquisitionBase : public IImageAcquisition
{
		DECLARE_CLASS_LOGGER();
public:
	CImageAcquisitionBase(EImageAcquisitionType type, bool mirrorInputImage);
	~CImageAcquisitionBase();

	virtual XTR3D_ERROR_e SetImageAcquisitionCallback(ImageAcquistionCallback* frameCallback, void* userData);
	virtual void GetImageAcquisitionCallback(ImageAcquistionCallback** frameCallback, void** userData);
    XTR3D_ERROR_e SetCompletionCallback(CompletionCallback callbackComplete);
	virtual EImageAcquisitionState GetImageAcquisitionState() { return m_currentState; }
	virtual EImageAcquisitionType GetImageAcquisitionType()  { return m_type; };

protected:
	virtual void SetImageAcquisitionState(EImageAcquisitionState state) {LOG_INFO("Setting" << VAR_VAL(state)); m_currentState = state;}
	
	// Members
	EImageAcquisitionState		m_currentState;
	bool						m_mirrorInputImage;
    bool						m_OutputBGRImg;
    ImageAcquistionCallback*	m_newImageCallback; // pointer to callback function which is called each time the frame is ready.
    CompletionCallback			m_completionCallback; //pointer to callback function which is called when there are no more frames.
    SImageHeader*				m_userBuffer; 
	void*						m_userData;
    SImageHeader*				m_videoFormat; // Contains current frame format, passed with image on callback.
	EImageAcquisitionType		m_type;

	// The following booleans are required in order to stop IA but not harming OnReadSample callbacks (story 1335).
	bool						m_canReadSample;
	bool						m_isReadingSample;
	SImageInfo					m_imageInfo;
};