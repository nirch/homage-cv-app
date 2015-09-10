#include <stdlib.h>
#include "ImageAcquisitionFileReader.h"
#include "Macros.h"
#include "ColorConversions.h"
#include "XTR3D_SDK_Macros.h"
#include "Xtr3DSystem.h"
#include "Xtr3DThread.h"
#include "Xtr3DSync.h"
#include <string>

IMPLEMENT_CLASS_LOGGER(CImageAcquisitionFileReader,"XTR3D_ImageAcquisition.CImageAcquisitionFileReader");

// ctor
CImageAcquisitionFileReader::CImageAcquisitionFileReader():
	CImageAcquisitionBase(IAT_FILE, false),
	m_hThread(NULL),
	m_dwThreadID(0),
	m_eventDone(NULL),
	m_eventResume(NULL),
	m_eventReadSample(NULL),
	m_useFrameRate(false),
    m_frameSkipFactor(MIN_FRAME_SKIP_FACTOR),
	m_isFirstTimeStamp(true),
	m_isEndOfFile(false),
	m_userBufferCurr(NULL),
	m_userBufferPrev(NULL),
	m_ffmpegReader(NULL)
{
	LOG_TRACE("Starting");
	m_videoFormat = new SImageHeader();
	m_videoFormat->m_frameId = 0;
    memset(m_videoFormat,0,sizeof(SImageHeader));
    m_videoFormat->m_size = sizeof(SImageHeader);
	
	m_userBuffer1 = new SImageHeader();
	m_userBuffer2 = new SImageHeader();

	wmemset(m_source,0,MAX_PATH);
	m_ffmpegReader = new CFfmpegReader();
	m_logTimeStamp = new CPerformanceLogger("T0: (Image Acquisition to Manager) ");
	LOG_INFO("Completed successfully");
}

// destructor
CImageAcquisitionFileReader::~CImageAcquisitionFileReader()
{
	LOG_TRACE("Starting");
	if(m_currentState != NOT_INITIALIZED)
	{
		Close();	
	}
	//::MFShutdown();		// de-initialize the WMF system
	SAFE_DELETE(m_userBuffer1);
	SAFE_DELETE(m_userBuffer2);
	SAFE_DELETE(m_ffmpegReader);
	SAFE_DELETE(m_logTimeStamp);
	LOG_INFO("Completed successfully");
}

// initialize the reader & callback
/* Params:
  [in] src  - source file path
  [in] callbackSample     - address of callback function for each sample - NULL for pull!
  [in] callbackSample     - address of callback function for complete - NULL for pull!
*/
XTR3D_ERROR_e CImageAcquisitionFileReader::SetSourceFileName (const char* src)
{
	std::string str(src);
	std::wstring ws(str.begin(), str.end());
	wchar_t* wc = (wchar_t*)ws.c_str();
	SetSourceFileName(wc);
}


XTR3D_ERROR_e CImageAcquisitionFileReader::SetSourceFileName (const wchar_t* src)
{
	LOG_DEBUG("Setting source file " << src);

	if((m_currentState == RUNNING) || (m_currentState == PAUSED))
	{
		LOG_ERROR("SetSourceFileName - Acquisition already running. Can't update source filename ");	
		return XTR3D_ERROR_GENERIC;
	}
	if (src == NULL) 
	{
		LOG_ERROR("SetSourceFileName - src is NULL");	
		return XTR3D_ERROR_GENERIC;
	}
	wcscpy(m_source,src);
	LOG_INFO("Source file name was successfully set to " << src);
	return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionFileReader::SetPlaybackFrameRate (int playbackFrameRate)
{
	LOG_DEBUG("Setting playback frame rate " << playbackFrameRate);
	
	if((m_currentState == RUNNING) || (m_currentState == PAUSED))
	{
		LOG_ERROR("SetPlaybackFrameRate - Acquisition already running. Can't update frame rate ");	
		return XTR3D_ERROR_GENERIC;
	}
	XTR3D_ERROR_e result = XTR3D_ERROR_OK;
	if(playbackFrameRate <= 0)
	{
		m_useFrameRate = false;
		LOG_WARN("SetPlaybackFrameRate - Input frame rate zero or negative. No fixed frame rate is set");	
		return XTR3D_ERROR_OK;
	}
	m_useFrameRate = true;
	m_desiredFrameRate = playbackFrameRate;
	LOG_INFO("Playback frame rate was successfully set to " << playbackFrameRate);
	return result;
}

XTR3D_ERROR_e CImageAcquisitionFileReader::SetPlaybackFrameSkip (int playbackFrameSkipFactor)
{
    LOG_DEBUG("Setting frame skip factor to " << playbackFrameSkipFactor);

    if((m_currentState == RUNNING) || (m_currentState == PAUSED))
    {
        LOG_ERROR("SetPlaybackFrameSkip - Acquisition already running. Skipping frame skip update.");	
        return XTR3D_ERROR_GENERIC;
    }

    if (playbackFrameSkipFactor < MIN_FRAME_SKIP_FACTOR)
    {
        m_frameSkipFactor = MIN_FRAME_SKIP_FACTOR;
        LOG_WARN("SetPlaybackFrameSkip - Input frame skip factor zero or negative. No frame skip is set");	
    }
    else
    {
        m_frameSkipFactor = playbackFrameSkipFactor;
        LOG_INFO("Playback frame skip factor was successfully set to " << m_frameSkipFactor);
    }
    return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionFileReader::Init()
{
	LOG_DEBUG("initializing");

	if(m_eventDone != NULL)
	switch (m_currentState)
    {
		case INITIALIZED: // fall through
		case PAUSED: // fall through
		case RUNNING:	
			{
				return XTR3D_ERROR_OK;
			}
    }
	
	m_useFrameRate = false;
	m_desiredFrameRate = -1;

	LOG_INFO("initialized successfully");
	SetImageAcquisitionState(INITIALIZED);
	return XTR3D_ERROR_OK;
}

// close the writer
XTR3D_ERROR_e CImageAcquisitionFileReader::Close()
{
	LOG_DEBUG("Close()");

	Stop(); // close the handle of the kernel thread object used for callback
	
	m_dwThreadID = 0;
	m_newImageCallback = NULL;	
	m_completionCallback = NULL;

	SAFE_DELETE_ARRAY(m_userBuffer1->m_imageData);
	SAFE_DELETE_ARRAY(m_userBuffer2->m_imageData);
	Xtr3DSyncCloseEvent(m_eventDone);
	Xtr3DSyncCloseEvent(m_eventResume);
	Xtr3DSyncCloseEvent(m_eventReadSample);
	Xtr3DSyncCloseEvent(m_hThread);

	LOG_INFO("Close complete successfully");
	SetImageAcquisitionState(NOT_INITIALIZED);
	return XTR3D_ERROR_OK;
}

// static thread worker method
/* Params:
  [in] param  - this object instance pointer
*/
unsigned int CImageAcquisitionFileReader::Worker(void* param )
{
	HRESULT hr;
	CImageAcquisitionFileReader* reader = (CImageAcquisitionFileReader*)param; // get the instance as a param
		
	LOG_INFO("Worker thread started");
	hr = reader->ReadSamples();	// read samples
	if (hr != XTR3D_ERROR_OK) 
	{
		LOG_ERROR("ReadSamples failed - " << hr);			
		return (1);
	}
	LOG_INFO("Worker thread completed successfully");
	return 0;
}

// start processing  - if there is a callback registered, start up a thread that will push
XTR3D_ERROR_e CImageAcquisitionFileReader::Start()
{
	LOG_DEBUG("starting");
	
	switch (m_currentState)
	{
		case RUNNING:		
			{
				return XTR3D_ERROR_OK;
			}
		case NOT_INITIALIZED:	
			{
				LOG_ERROR("Start() called without Init()");
				return XTR3D_ERROR_NOT_INITIALIZED;
			}
	}

	if(m_newImageCallback == NULL)
	{
		LOG_ERROR("Callback is NULL. Check if Start() was called before setCallback()");	
		return XTR3D_ERROR_NOT_INITIALIZED;
	}
	
	XTR3D_ERROR_e result = m_ffmpegReader->InitReader(m_source);
	if (result != XTR3D_ERROR_OK) 
	{
		LOG_ERROR("Could not open file. Please check your path");	
		return XTR3D_ERROR_BAD_FILENAME;
	}
	m_ffmpegReader->GetFormat(m_videoFormat);

    //TODO: Camera and FileReader use different structs to pass to caller. For now, just convert from one to other and use m_videoFormat:
	memcpy(m_userBuffer1,m_videoFormat,sizeof(SImageHeader));
	m_userBuffer1->m_frameId = 0;
	m_userBuffer1->m_size = sizeof(SImageHeader);
	m_userBuffer1->m_imageData = new unsigned char[m_userBuffer1->m_sizeImage];
	memcpy(m_userBuffer2,m_videoFormat,sizeof(SImageHeader));
	m_userBuffer2->m_frameId = 0;
	m_userBuffer2->m_size = sizeof(SImageHeader);
	m_userBuffer2->m_imageData = new unsigned char[m_userBuffer2->m_sizeImage];

	m_userBufferCurr = m_userBuffer1;
	m_userBufferPrev = m_userBuffer2;
	
	m_isEndOfFile = false;
	m_isFirstTimeStamp = true;
	m_firstImprintedTimeStamp = 0;
	m_firstTimeStamp = 0;
	m_nextDesiredTimestamp = 0;
	m_firstSentFrameTicCount = 0;
	m_nextDesiredTickCount = 0;

	m_eventDone = Xtr3DSyncCreateEvent(NULL, TRUE, FALSE,NULL);
	if(m_eventDone == NULL)
	{
		LOG_ERROR("Error creating done event");
		return XTR3D_ERROR_GENERIC;
	}
	
	m_eventReadSample = Xtr3DSyncCreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_eventReadSample == NULL)
	{
		LOG_ERROR("Error creating ReadSample event");
		return XTR3D_ERROR_GENERIC;
	}

	m_hThread = Xtr3DThreadCreate(Worker, (void*)this);	// spin up a thread
	if (NULL == m_hThread)
	{
		LOG_ERROR("Failed to create thread");	
		return XTR3D_ERROR_GENERIC;
	}

	m_canReadSample = true;
	SetImageAcquisitionState(RUNNING);
	LOG_INFO("started successfully");
	return XTR3D_ERROR_OK;
}

// read a sample - push method
XTR3D_ERROR_e CImageAcquisitionFileReader::ReadASampleAndCallback( )
{
	HRESULT hr = S_OK;
	long long int  timeStamp;
	unsigned char* spSample = new unsigned char[m_userBufferCurr->m_sizeImage];
	XTR3D_ERROR_e readResult;
	if (!m_useFrameRate)
	{
		readResult = ReadSampleAndExtractImageAndTimeStamp(&timeStamp, &spSample);
		if(readResult == XTR3D_ERROR_OK)
		{
			memcpy(m_userBuffer->m_imageData, m_userBufferCurr->m_imageData, m_userBufferCurr->m_sizeImage);
		}
		else
		{
			return readResult;
		}
	}
	else
	{
		readResult = ReadSampleAccordingToFrameRate(&timeStamp, &spSample);
	}

	if (SUCCEEDED(hr) && (readResult == XTR3D_ERROR_OK)) 
	{
		OnReadSample(timeStamp, spSample);	// read a sample
	}
		
	if (FAILED(hr)) 
	{
		LOG_ERROR("OnReadSample failed - " << hr);
		Xtr3DSyncSetEvent(m_eventDone);
		return XTR3D_ERROR_GENERIC;
	}
	SAFE_DELETE(spSample);
	return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionFileReader::ReadSampleAccordingToFrameRate(long long int *timeStamp, unsigned char** spSample)
{
	SImageHeader* userBufferChosen = m_userBufferCurr;
	long long int  currTicCount;
	long long int  timeFromLastFrame;
	bool readAtLeastOneFrame = false;
	XTR3D_ERROR_e readResult;
	
	long long int  desiredTimeBetweenFrames = (long long int )(1000 / SAMPLE_TIME_TO_ENGINE_TIME_FACTOR) / m_desiredFrameRate ;
	if (m_isFirstTimeStamp)
	{
		readResult = ReadSampleAndExtractImageAndTimeStamp(timeStamp, spSample);
		if (readResult == XTR3D_ERROR_OK) 
			readAtLeastOneFrame = true;
		m_firstSentFrameTicCount = Xtr3DSystemGetTickCount();
		m_nextDesiredTickCount = m_firstSentFrameTicCount + (long long int )(desiredTimeBetweenFrames * SAMPLE_TIME_TO_ENGINE_TIME_FACTOR);
		m_nextDesiredTimestamp = m_firstTimeStamp + desiredTimeBetweenFrames;
	}
	else
	{
		readAtLeastOneFrame = ReadSampleAccordingToTimeStamp(timeStamp, spSample, &userBufferChosen);
		if (m_isEndOfFile)
			return XTR3D_ERROR_OK;	// stop reading on completion
		currTicCount = Xtr3DSystemGetTickCount();
		timeFromLastFrame = currTicCount - m_firstSentFrameTicCount;
		if (currTicCount < m_nextDesiredTickCount)
		{
			sleep(m_nextDesiredTickCount - currTicCount);
		}
		else
		{
			int numFramesMissed = (int)((currTicCount - m_nextDesiredTickCount) / (desiredTimeBetweenFrames * SAMPLE_TIME_TO_ENGINE_TIME_FACTOR));
			if (numFramesMissed > 0)
			{
				m_nextDesiredTickCount += (long long int )(numFramesMissed * desiredTimeBetweenFrames * SAMPLE_TIME_TO_ENGINE_TIME_FACTOR);
				m_nextDesiredTimestamp += (numFramesMissed * desiredTimeBetweenFrames);
				*spSample = NULL;
				readAtLeastOneFrame = ReadSampleAccordingToTimeStamp(timeStamp, spSample, &userBufferChosen);
			}
		}
		m_nextDesiredTickCount += (long long int )(desiredTimeBetweenFrames * SAMPLE_TIME_TO_ENGINE_TIME_FACTOR);
		m_nextDesiredTimestamp += desiredTimeBetweenFrames;
	}
	if (m_isEndOfFile)
		return XTR3D_ERROR_OK;	// stop reading on completion
	if (!readAtLeastOneFrame)
	{
		LOG_ERROR("Required frame rate exceeds movie frame rate");
		m_logTimeStamp->LogError("Required frame rate exceeds movie frame rate");
		if (NULL != m_completionCallback)
		{
			Xtr3DSyncSetEvent(m_eventDone);
			LOG_INFO("Calling completion callback");
			SPlaybackCompletionParams params(m_source,m_videoFormat->m_frameId,XTR3D_ERROR_GENERIC);
			m_completionCallback(&params);
			LOG_INFO("completion callback finished with error code ");
		}

		return XTR3D_ERROR_GENERIC;
	}

	memcpy(m_userBuffer->m_imageData, userBufferChosen->m_imageData, userBufferChosen->m_sizeImage);
	return XTR3D_ERROR_OK;
}

bool CImageAcquisitionFileReader::ReadSampleAccordingToTimeStamp(long long int *timeStamp, unsigned char** spSample, SImageHeader** userBufferChosen)
{
	bool readAtLeastOneFrame = false;
	SImageHeader* tmpUserBuffer;
	XTR3D_ERROR_e res;
	while (m_userBufferCurr->m_timestamp < m_nextDesiredTimestamp && !m_isEndOfFile)
	{
		SAFE_DELETE_ARRAY(*spSample);
		SAFE_DELETE(spSample);
		tmpUserBuffer = m_userBufferPrev;
		m_userBufferPrev = m_userBufferCurr;
		m_userBufferCurr = tmpUserBuffer;
		*spSample = NULL;
		res = ReadSampleAndExtractImageAndTimeStamp(timeStamp, spSample);
		if (res == XTR3D_ERROR_OK) 
			readAtLeastOneFrame = true;
	}
	long long int  diffCurr = abs((long)(m_nextDesiredTimestamp - m_userBufferCurr->m_timestamp));
	long long int  diffPrev = abs((long)(m_nextDesiredTimestamp - m_userBufferPrev->m_timestamp));
	if (diffCurr <= diffPrev)
		*userBufferChosen = m_userBufferCurr;
	else
		*userBufferChosen = m_userBufferPrev;
	*timeStamp = (*userBufferChosen)->m_timestamp;
	return readAtLeastOneFrame;
}

// read a sample and handle errors and end of file
// if error: return value is not XTR3D_ERROR_OK
// if end of file: m_isEndOfFile is true
XTR3D_ERROR_e CImageAcquisitionFileReader::ReadSampleFromFile(long long int *timeStamp, unsigned char** spSample)
{
	XTR3D_ERROR_e res = m_ffmpegReader->GetFrame(*spSample, timeStamp);	// read a sample
    if (res != XTR3D_ERROR_OK) 
    {

        if (NULL != m_completionCallback)
        {
            Xtr3DSyncSetEvent(m_eventDone);
            LOG_INFO("Calling completion callback");
            SPlaybackCompletionParams params(m_source,m_videoFormat->m_frameId,XTR3D_ERROR_GENERIC);
            m_completionCallback(&params);
            LOG_INFO("completion callback finished with error code ");
        }
        return XTR3D_ERROR_GENERIC;	// stop reading on error
    }
	//Note: the above method invokes ffmpeg API, which generates its own timestamp.
    //Such timestamp will be overwritten by the caller of CImageAcquisitionFileReader::ReadSampleFromFile (see: ReadSampleAndExtractImageAndTimeStamp).
    if (res != XTR3D_ERROR_OK) 
    {
        LOG_ERROR("ReadSample failed - " << res);			
        return XTR3D_ERROR_GENERIC;	// stop reading on error
    }

	return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionFileReader::ReadSampleAndExtractImageAndTimeStamp(long long int *timeStamp, unsigned char** spSample)  
{
	unsigned char* pImage = NULL;


    // Read the next sample. Use frame skip mechanism:
    //      (1) first frame of the stream is read with no skips 
    //      (2) for all other frames read N times and use the last sample (skipping N-1 frames).
    const int framesToRead = m_isFirstTimeStamp ? 1 : m_frameSkipFactor;
    for (int i=0; i<framesToRead; i++)
    {
        XTR3D_ERROR_e result = ReadSampleFromFile(timeStamp, spSample);
        if (result != XTR3D_ERROR_OK)
        {
            LOG_ERROR("Failed reading a sample: " << result)
            return result;
        }
        if (m_isEndOfFile)
        {
            LOG_INFO("No more samples.")
            return result;
        }
		// If reading more than 1 frame, we need to call OnCaptureImage for the first (N-1) frames.
		// For the last frame, OnCapturedImage will be called later.
        if ( i < framesToRead - 1 )   
		{	
            OnCapturedImage(m_userBufferCurr);
		}
    }
	memcpy(m_userBufferCurr->m_imageData, *spSample, m_userBufferCurr->m_sizeImage);
    //Note:
    //The following code, extracts the timestamp from the image itself.
    //Since it is invoked BEFORE any image transformations (such as: vertical/horizontal flips/mirror image/BGR<->RGB), 
    //the recorder (usually, the ImageAcquistionCamera), must encode the timestamp in two locations:
    //1. The first 4 bytes of the image BEFORE it undergoes any transformation
    //2. The first 4 bytes of the image AFTER it had undergone all transformation
    //Naturally, the transformations used must be the same for both recorder and reader.	
	DWORD* timeStampAddress = ( DWORD* )(m_userBufferCurr->m_imageData);
	long long int  tmpTimeStamp = (*timeStampAddress);
	if (m_isFirstTimeStamp)
	{
		m_firstImprintedTimeStamp = tmpTimeStamp;
		m_firstTimeStamp = 0;
		m_isFirstTimeStamp = false;
	}
	//TODO: There really isn't a need to convert the timestamp to 100 NS, however SingleClipRunner, as well as other applications, assume the timestamp is in those units.
	//Therefore, this code won't be modified, until ALL apps treat timestamp as milliseconds.
	*timeStamp = (long long int )((tmpTimeStamp - m_firstImprintedTimeStamp) / SAMPLE_TIME_TO_ENGINE_TIME_FACTOR);
	m_userBufferCurr->m_timestamp = *timeStamp;
	
	return XTR3D_ERROR_OK;
}


// read a sample - pull method
XTR3D_ERROR_e CImageAcquisitionFileReader::ReadASample(SImageHeader* imageHeader)  
{
    m_userBuffer = imageHeader;
	Xtr3DSyncSetEvent(m_eventReadSample);

    //TODO: Need to make sure the worker thread is running!

    return XTR3D_ERROR_OK;
}

// read the samples from the source reader
XTR3D_ERROR_e CImageAcquisitionFileReader::ReadSamples( )
{
	LOG_DEBUG("started");
	
	XTR3D_ERROR_e result = XTR3D_ERROR_OK;
	HANDLE waitHandles[] = {m_eventDone, m_eventReadSample};//, m_eventResume};
	DWORD waitResult;	
	bool done = false;
	while(!done)
	{		
		unsigned long lowestIndex;
		waitResult = Xtr3DSyncWaitForMultipleObjects( sizeof( waitHandles ) / sizeof( HANDLE ), waitHandles, FALSE, INFINITE, lowestIndex );
		
		switch(waitResult)
		{
			case Xtr3DSyncEvent::TIMED_OUT:
			{
				done = true;
				LOG_ERROR("WaitForMultipleObjects returned WAIT_TIMEOUT.");
				break;
			}
			case Xtr3DSyncEvent::FAILED:
			{
				done = true;
				DWORD error = GetLastError();
				LOG_ERROR("WaitForMultipleObjects returned WAIT_FAILED. System Error code=" << error);
				break;
			}

			case Xtr3DSyncEvent::SIGNALLED:
			{
				switch(lowestIndex)
				{
					case 0: // m_eventDone was triggered.
					{
						done = true;
						break;				
					}
					case 1: // m_eventReadSample was triggered.
					{
						if(!m_userBuffer)
						{
							LOG_WARN("m_userBuffer is NULL. Waiting 100 milliseconds");
							sleep(100);
							continue;
						}
						result = ReadASampleAndCallback();
						if (result != XTR3D_ERROR_OK)
						{
							LOG_ERROR("ReadASampleAndCallback failed with error " << result );
							done = true;
						}	
						break;
					}
					default:
					{
						LOG_ERROR("Unknown event index: " << lowestIndex);
						break;
					}
				}
				break;
			}
			default:
			{
				LOG_INFO("WaitForMultipleObjects returned an unhandled code:" << waitResult);
				break;
			}
		}
	}
	return result;
}


#pragma warning(disable :4100)
// process a sample 
/* Params:
  [in] hrStatus  - status
  [in] dwStreamIndex  - the stream index
  [in] dwStreamFlags  - the stream flags
  [in] llTimestamp  - the timestamp
  [in] pSample  - the actual sample
*/
HRESULT CImageAcquisitionFileReader::OnReadSample(long long int  llTimestamp, 	unsigned char* pSample)
{	
	m_isReadingSample = true;
	if (!m_canReadSample)
	{
		m_isReadingSample = false;
		LOG_INFO("Not serving. Quitting callback.");
		return S_OK;
	}
	HRESULT hr = OnReceivedNewImage(llTimestamp, pSample, m_userBuffer);
	if(FAILED(hr))
	{
		m_isReadingSample = false;
		LOG_ERROR("OnReceivedNewImage failed. Error code: " << hr);
		Xtr3DSyncSetEvent(m_eventDone);
		return XTR3D_ERROR_GENERIC;
	}
    if (m_newImageCallback == NULL)
    {
		m_isReadingSample = false;
        LOG_FATAL(" OnReadSample called without a user callback - shouldn't happen!");
		Xtr3DSyncSetEvent(m_eventDone);
		return XTR3D_ERROR_GENERIC;
    }
	Xtr3DSyncEvent::ResultType waitResult = Xtr3DSyncWaitForSingleObject(m_eventDone, 0);
	if (waitResult != Xtr3DSyncEvent::SIGNALLED)
	{
		m_logTimeStamp->LogTimeStamp(m_userBuffer->m_frameId);
		hr = m_newImageCallback(m_userBuffer, m_userData);
		if (FAILED(hr)) 
		{
			m_isReadingSample = false;
			LOG_ERROR("callback failed - " << hr);			
			Xtr3DSyncSetEvent(m_eventDone);
			return XTR3D_ERROR_GENERIC;
		}
	}

	m_isReadingSample = false;
    return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionFileReader::OnReceivedNewImage(long long int timeStamp, unsigned char* spSample, SImageHeader* imageHeader)
{
    HRESULT hr;
	//CComPtr<IMFMediaBuffer> pBuffer;	
	long long int  sampleDuration = 0;

	if (imageHeader->m_imageData == NULL)
    {
		LOG_ERROR("OnReceivedNewImage called with NULL user buffer!!!");
        return XTR3D_ERROR_CAM_ACQ_GENERIC;
    }

	if (spSample == NULL)
	{
		LOG_INFO("OnReceivedNewImage called with spSample NULL");
        return XTR3D_ERROR_CAM_ACQ_GENERIC;
	}
	PrepareOutputImage(imageHeader->m_imageData);

	m_videoFormat->m_timestamp = timeStamp;
	m_videoFormat->m_sampleDuration = 30 * 10000; //no way of getting it from ffmpeg
	SetImageHeader(imageHeader);
	OnCapturedImage(imageHeader);
	return XTR3D_ERROR_OK;
}
#pragma warning(default :4100)

void CImageAcquisitionFileReader::PrepareOutputImage(unsigned char* pImageOut)
{	
    if(m_videoFormat->m_stride < 0)
    {
        if (m_OutputBGRImg)
		{
            FlipVerticalHorizontal(pImageOut,GetWidth(), GetHeight(), m_mirrorInputImage);
		}
        else
		{
            FlipVerticalHorizontalRgb(pImageOut,GetWidth(), GetHeight(), m_mirrorInputImage);
		}
    }
    else
    {
        if(m_mirrorInputImage)
        {
            MirrorImage(pImageOut,GetWidth(), GetHeight());
        }
    }
}

// stop processing - close the worker thread if it exists
XTR3D_ERROR_e CImageAcquisitionFileReader::Stop()
{
	LOG_DEBUG("stopping");	

	switch (m_currentState)
	{	
		case NOT_INITIALIZED: 
			{
				LOG_ERROR("Not yet initialized. Check whether Stop() was called before Init()");
				return XTR3D_ERROR_GENERIC;
			}
	}
	//check if eventdone has already been signaled
    Xtr3DSyncEvent::ResultType waitResult = Xtr3DSyncWaitForSingleObject(m_eventDone, 0);
	if (waitResult == Xtr3DSyncEvent::SIGNALLED)
    {
        SetImageAcquisitionState(INITIALIZED);
		LOG_INFO("Already stopped.");
        return XTR3D_ERROR_OK;
    }

	m_canReadSample = false;

	Xtr3DSyncSetEvent(m_eventDone);
	if (NULL != m_hThread) 
	{
		try
		{
			waitResult = Xtr3DSyncWaitForSingleObject(m_hThread, INFINITE);
			Xtr3DSyncCloseEvent(m_hThread);
		}
		catch (std::exception& ex)
		{
			LOG_DEBUG("Handle already closed" << ex.what());
		}
		catch (...)
		{
			LOG_DEBUG("Handle already closed");
		}
	}
	SetImageAcquisitionState(INITIALIZED);
	SAFE_DELETE_ARRAY(m_userBuffer1->m_imageData);
	SAFE_DELETE_ARRAY(m_userBuffer2->m_imageData);
	m_userBuffer = NULL;
	Xtr3DSyncCloseEvent(m_eventDone);
	Xtr3DSyncCloseEvent(m_eventResume);
	Xtr3DSyncCloseEvent(m_eventReadSample);

	LOG_INFO("stopped successfully");
	return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionFileReader::Pause()
{
	// if(m_currentState == RUNNING)
	// {
		// Xtr3DSyncResetEvent(m_eventResume);
		// SetImageAcquisitionState(PAUSED);
		// LOG_INFO("Playback paused");
		// return XTR3D_ERROR_OK;
	// }
	// else
	// {
		// LOG_ERROR("Pause failed, ImageAquisitionFileReader not in running mode");
		// return XTR3D_ERROR_GENERIC;
	// }
	return XTR3D_ERROR_OK;
}

XTR3D_ERROR_e CImageAcquisitionFileReader::Resume()
{
	// if(m_currentState == PAUSED)
	// {
		// Xtr3DSyncSetEvent(m_eventResume);
		// SetImageAcquisitionState(RUNNING);
		// LOG_INFO("Playback resumed");
		// return XTR3D_ERROR_OK;
	// }
	// else
	// {
		// LOG_ERROR("Resume failed, ImageAquisitionFileReader not in paused mode");
		// return XTR3D_ERROR_GENERIC;
	// }
	return XTR3D_ERROR_OK;

}

int CImageAcquisitionFileReader::GetHeight()
{
	CHECK_POINTER_RETURN_ERROR(m_videoFormat);
	return m_videoFormat->m_height;
}

int CImageAcquisitionFileReader::GetWidth()
{
	CHECK_POINTER_RETURN_ERROR(m_videoFormat);
	return m_videoFormat->m_width;
}

int CImageAcquisitionFileReader::GetBitsPerPixel()
{
	CHECK_POINTER_RETURN_ERROR(m_videoFormat);
	return m_videoFormat->m_bitCount;
}

int CImageAcquisitionFileReader::GetImageSize()
{
	if(!m_videoFormat)
	{
		LOG_ERROR("m_videoFormat is NULL please call Start beforehand");			
		return 0;
	}
	return m_videoFormat->m_width * m_videoFormat->m_height * m_videoFormat->m_bitCount / 8;
}

unsigned long CImageAcquisitionFileReader::GetFourCC()
{
	if(!m_videoFormat)
	{
		LOG_ERROR("m_videoFormat is NULL please call Start beforehand");			
		return 0;
	}
	return m_videoFormat->m_fccFormat;
}
void CImageAcquisitionFileReader::OnCapturedImage(SImageHeader* imageHeader)
{
	LOG_TRACE("Starting");
    imageHeader->m_frameId = m_videoFormat->m_frameId++;
	LOG_DEBUG("Setting imageHeader->m_frameId=" << imageHeader->m_frameId);
	LOG_TRACE("Completed successfully");
}
void CImageAcquisitionFileReader::SetImageHeader(SImageHeader* imageHeader)
{
	LOG_TRACE("Starting");
    imageHeader->m_fccFormat = m_videoFormat->m_fccFormat;
    imageHeader->m_frameId = m_videoFormat->m_frameId;
    imageHeader->m_planes = m_videoFormat->m_planes;
    imageHeader->m_size = m_videoFormat->m_size;
    imageHeader->m_sizeImage = m_videoFormat->m_sizeImage;
    imageHeader->m_stride = m_videoFormat->m_stride;
    imageHeader->m_timestamp = m_videoFormat->m_timestamp ;
    imageHeader->m_sampleDuration = m_videoFormat->m_sampleDuration;
    imageHeader->m_height=m_videoFormat->m_height;
    imageHeader->m_width=m_videoFormat->m_width;
    imageHeader->m_bitCount = m_videoFormat->m_bitCount;
	LOG_TRACE("Completed successfully");
}
