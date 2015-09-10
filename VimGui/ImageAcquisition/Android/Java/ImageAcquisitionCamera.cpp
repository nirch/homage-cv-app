#include "ImageAcquisitionCamera.h"
#include "ImageHeader.h"

//#include "FileSystemUtils.h" //For debug!

// On AsusTF201, we get the byte[] as non-mirrored-image, the java Preview View is doing the mirroring in a later stage, which is after we got it 
#define DEFAULT_MIRROR_IMAGE true

//Static members initializations:
IMPLEMENT_CLASS_LOGGER(CImageAcquisitionCamera,"XTR3D_ImageAcquisition.CImageAcquisitionCamera");

CImageAcquisitionCamera::CImageAcquisitionCamera():
	CImageAcquisitionBase( IAT_CAMERA, DEFAULT_MIRROR_IMAGE ),
	m_cameraImpl(NULL), 
	m_frameId(0)
{
	m_logTimeStamp = new CPerformanceLogger("T0: (Image Acquisition to Manager) ");
}

XTR3D_ERROR_e CImageAcquisitionCamera::Init()
{
	LOG_TRACE(__FUNCTION__ << ": entered.");
    if (m_currentState != NOT_INITIALIZED)
    {
        if (m_cameraImpl == NULL)
        {
            LOG_FATAL(__FUNCTION__ << ": not initialized but already allocated!");
            return XTR3D_ERROR_GENERIC;
        }

        LOG_WARN(__FUNCTION__ << ": already initializedl!");
        return XTR3D_ERROR_OK;
    }

    if (m_cameraImpl == NULL)
    {
        m_cameraImpl = new CameraImpl();
        if (m_cameraImpl == NULL)
        {
            LOG_ERROR(__FUNCTION__ << ": failed to init CameraImpl!");
            return XTR3D_ERROR_NULL_VALUE;
        }
    }

    m_colorConversion = decodeYUV420SP; //Assuming Android native NV21 is used...

    m_currentState = INITIALIZED;

    LOG_TRACE(__FUNCTION__ << ": exit.");

    return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionCamera::Close()
{
    m_currentState = NOT_INITIALIZED;

    if (m_cameraImpl != NULL)
    {
        delete m_cameraImpl;
        m_cameraImpl = NULL;
    }

    return XTR3D_ERROR_OK;
}
XTR3D_ERROR_e CImageAcquisitionCamera::Start()
{
    LOG_TRACE(__FUNCTION__ << ": entered.");

    if (m_cameraImpl == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": failed to init CameraImpl!");
        return XTR3D_ERROR_NULL_VALUE;
    }

    m_cameraImpl->SetOnCameraReceivedNewImageCallback(OnReadSample, this);

    if (!m_cameraImpl->Start(false))
    {
        return XTR3D_ERROR_GENERIC;
    }

    m_currentState = RUNNING;

    LOG_TRACE(__FUNCTION__ << ": exit.");

    return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionCamera::Stop()
{
    LOG_TRACE(__FUNCTION__ << ": entered.");
    if(m_currentState != RUNNING)
    {
    	LOG_DEBUG(__FUNCTION__ << ": ImageAcquisitionCamera is not running, exiting.")
    	return XTR3D_ERROR_OK;
    }
    if (m_cameraImpl == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": failed to init CameraImpl!");
        return XTR3D_ERROR_NULL_VALUE;
    }

    m_cameraImpl->SetOnCameraReceivedNewImageCallback(NULL, NULL);

    if (!m_cameraImpl->Stop())
    {
        return XTR3D_ERROR_GENERIC;
    }

    m_currentState = INITIALIZED;

    LOG_TRACE(__FUNCTION__ << ": exit.");

    return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionCamera::ReadASample( SImageHeader* imageHeader )
{
    if (!imageHeader)
    {
        LOG_ERROR(__FUNCTION__ << ": called with NULL!");
        return XTR3D_ERROR_NULL_VALUE;
    }
    if (!m_newImageCallback)
    {
        LOG_ERROR(__FUNCTION__ << ": callback is NULL!");
        return XTR3D_ERROR_CAM_ACQ_GENERIC;
    }
    if ( m_currentState != RUNNING )
    {
        LOG_ERROR(__FUNCTION__ << ": IA not running! Cannot read!");
        return XTR3D_ERROR_CAM_ACQ_GENERIC;
    }

    m_userBuffer = imageHeader;

    return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionCamera::SetRenderer(IUnknown* rendererFromApp)
{
    LOG_TRACE(__FUNCTION__ << ": entered.");
    if (rendererFromApp == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": rendererFromApp is NULL!");
        return XTR3D_ERROR_NULL_VALUE;
    }

    if (m_cameraImpl == NULL)
    {
        m_cameraImpl = new CameraImpl();
        if (m_cameraImpl == NULL)
        {
            LOG_ERROR(__FUNCTION__ << ": failed to init CameraImpl!");
            return XTR3D_ERROR_NULL_VALUE;
        }
    }

    if (!(m_cameraImpl->Init(rendererFromApp->jvm, rendererFromApp->activity, rendererFromApp->cameraView)))
    {
        LOG_ERROR(__FUNCTION__ << ": CameraImpl::Init failed!");
        return XTR3D_ERROR_GENERIC;
    }

    LOG_TRACE(__FUNCTION__ << ": exit.");

    return XTR3D_ERROR_OK;
}
void CImageAcquisitionCamera::OnReadSample(void* context, unsigned char* frame, long long int timestamp)
{
    LOG_TRACE(__FUNCTION__ << ": called.");

    if (context == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": context is NULL!");
        return;
    }

    CImageAcquisitionCamera* thiz = (CImageAcquisitionCamera*)context;
    if (thiz->m_newImageCallback == NULL)
    {
        LOG_WARN(__FUNCTION__ << ": No callback supplied!");
        return;
    }

    if (thiz->m_userData == NULL || thiz->m_userBuffer->m_imageData == NULL)
    {
        LOG_WARN(__FUNCTION__ << ": No user data supplied, or its internal buffer is empty!");
        return;
    }

	thiz->m_logTimeStamp->LogTimeStamp(thiz->m_userBuffer->m_frameId);
	
	thiz->m_userBuffer->m_frameId = thiz->m_frameId++;
        
	thiz->m_userBuffer->m_timestamp = timestamp;

    thiz->PrepareOutputImage(frame, thiz->m_userBuffer->m_imageData);


    //TODO: m_userBuffer
    thiz->m_newImageCallback(thiz->m_userBuffer, thiz->m_userData);

    LOG_TRACE(__FUNCTION__ << ": exited.");
}

void CImageAcquisitionCamera::PrepareOutputImage( unsigned char* pImage, unsigned char* pImageOut )
{
    if ( m_colorConversion )
    {
        LOG_DEBUG(__FUNCTION__ << ": Convert image via func: " << (unsigned long)m_colorConversion << " dimensions (wxh): " <<  GetWidth() << "x" << GetHeight());
        m_colorConversion( pImageOut, pImage, GetWidth(), GetHeight() );
        if ( m_OutputBGRImg )
        {
            LOG_DEBUG(__FUNCTION__ << ": Need to output BGR...");
            // m_colorConversion created an RGB image, turn it to BGR (function name is misleading)
            BgrToRgb( pImageOut, GetWidth(), GetHeight() );
        }
    }
    else
    {
        memcpy( pImageOut, pImage, m_userBuffer->m_sizeImage ); //640*480*3
    }

    if ( m_userBuffer->m_stride < 0 )
    {
        LOG_DEBUG(__FUNCTION__ << ": negative stride. Need to flip...");
        if ( m_OutputBGRImg )
        {
            FlipVerticalHorizontal( pImageOut, GetWidth(), GetHeight(), m_mirrorInputImage );
        }
        else
        {
            FlipVerticalHorizontalRgb( pImageOut, GetWidth(), GetHeight(), m_mirrorInputImage );
        }
    }
    else
    {
        if ( m_mirrorInputImage )
        {
            LOG_DEBUG(__FUNCTION__ << ": Need to mirror...");
            MirrorImage( pImageOut, GetWidth(), GetHeight() );
        }
    }
}
