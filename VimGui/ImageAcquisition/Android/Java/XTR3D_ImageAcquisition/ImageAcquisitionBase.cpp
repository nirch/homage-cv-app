#include "ImageAcquisitionBase.h"
#include "Macros.h"

IMPLEMENT_CLASS_LOGGER( CImageAcquisitionBase, "XTR3D_ImageAcquisition.CImageAcquisitionBase" );

CImageAcquisitionBase::CImageAcquisitionBase(EImageAcquisitionType type, bool mirrorInputImage):
	m_newImageCallback(NULL),
    m_completionCallback(NULL),
	m_mirrorInputImage(mirrorInputImage),
    m_OutputBGRImg(false),
    m_userBuffer(NULL),
	m_videoFormat(NULL),
	m_currentState(NOT_INITIALIZED),
	m_type(type),
	m_userData(NULL),
	m_canReadSample(false),
	m_isReadingSample(false)
{
	m_videoFormat = new SImageHeader();
	m_videoFormat->m_frameId = 0;
    memset(m_videoFormat,0,sizeof(SImageHeader));
    m_videoFormat->m_size = sizeof(SImageHeader);
	LOG_INFO("Completed successfully");
};

CImageAcquisitionBase::~CImageAcquisitionBase()
{
    SAFE_DELETE(m_videoFormat);
	LOG_INFO("Completed successfully");
}

void CImageAcquisitionBase::GetImageAcquisitionCallback(ImageAcquistionCallback** frameCallback, void** userData)
{
	*frameCallback = m_newImageCallback;
	*userData = m_userData;
}

XTR3D_ERROR_e CImageAcquisitionBase::SetImageAcquisitionCallback(ImageAcquistionCallback* frameCallback, void* userData)
{
	LOG_TRACE("Starting");
    switch (m_currentState)
    {
        case NOT_INITIALIZED: 
            {
                LOG_ERROR( "Acquisition not yet initialized. Can't register a callback ");
                return XTR3D_ERROR_GENERIC;
            }
    }

    m_newImageCallback = frameCallback; 
	m_userData = userData;
	
	LOG_TRACE("Completed successfully");
    return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionBase::SetCompletionCallback(CompletionCallback callbackComplete) 
{
	LOG_TRACE("Starting");
    switch (m_currentState)
    {
        case NOT_INITIALIZED: 
            {
                LOG_ERROR( "Acquisition not initialized. Can't register a completion callback ");
                return XTR3D_ERROR_GENERIC;
            }
        case RUNNING: 
            {
                LOG_ERROR( "Acquisition already started. Can't register a completion callback ");
                return XTR3D_ERROR_GENERIC;
            }
    }
    m_completionCallback = callbackComplete;

	LOG_TRACE("Completed successfully");
    return XTR3D_ERROR_OK;
}
