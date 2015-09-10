#pragma once

#include "IImageAcquisitionFileReader.h"
#include "XTR3D_SDK_Types.h"
#include "Xtr3dSync.h"
#include "FfmpegReader.h"
#include "OS_PerformanceLogger.h"
#include "ImageAcquisitionBase.h"

#define MIN_FRAME_SKIP_FACTOR   1           // frame skip factor of 1 means no skip. this is the default.


class IMAGE_ACQUISITION_API CImageAcquisitionFileReader : 
	public CImageAcquisitionBase, 
	public IImageAcquisitionFileReader
{

public:
	DECLARE_CLASS_LOGGER();

	CImageAcquisitionFileReader();
	virtual ~CImageAcquisitionFileReader();

	virtual XTR3D_ERROR_e Init();
	virtual XTR3D_ERROR_e Close();	// close the reader
	virtual XTR3D_ERROR_e Start();  // start reading from file  
	virtual XTR3D_ERROR_e Stop();  // stop reading from file   
	virtual XTR3D_ERROR_e SetImageParameters(SImageInfo*) {return XTR3D_ERROR_OK;};
	virtual int GetHeight();
	virtual int GetWidth();
	virtual int GetBitsPerPixel();
	virtual int GetImageSize();

	// called on each sample read
	HRESULT OnReadSample( long long int, unsigned char*);
	virtual XTR3D_ERROR_e ReadASample(SImageHeader* imageHeader);

	virtual XTR3D_ERROR_e SetSourceFileName (const wchar_t* src);
	virtual XTR3D_ERROR_e SetSourceFileName (const char* src);

	virtual XTR3D_ERROR_e SetPlaybackFrameRate (int playbackFrameRate);
    virtual XTR3D_ERROR_e SetPlaybackFrameSkip (int playbackFrameSkipFactor);

	XTR3D_ERROR_e ReadSamples();	// read samples (called from worker thread)
	unsigned long GetFourCC();
	XTR3D_ERROR_e Pause();
	XTR3D_ERROR_e Resume();

private:
	// private methods
	static unsigned int Worker(void* param );	// thread worker method
	void OnCapturedImage(SImageHeader* imageHeader);
	XTR3D_ERROR_e ReadASampleAndCallback();
    void PrepareOutputImage(unsigned char* pImageOut);
    XTR3D_ERROR_e OnReceivedNewImage(long long int timeStamp, unsigned char* spSample, SImageHeader* imageHeader);
	void SetImageHeader(SImageHeader* imageHeader);
	
    XTR3D_ERROR_e ReadSampleFromFile(long long int *timeStamp, unsigned char** spSample);
	XTR3D_ERROR_e ReadSampleAndExtractImageAndTimeStamp(long long int *timeStamp, unsigned char** spSample);
	bool ReadSampleAccordingToTimeStamp(long long int *timeStamp, unsigned char** spSample, SImageHeader** userBufferChosen);
	XTR3D_ERROR_e ReadSampleAccordingToFrameRate(long long int *timeStamp, unsigned char** spSample);

	// get source media type data to pass to sink writer
	//CWMFReader* m_wmfReader;	// WMF Reader encapsulates 
	HANDLE m_hThread;	// thread handle to wait upon
    unsigned int m_dwThreadID;	// thread id of worker thread
	HANDLE m_eventDone;// flag for starting / stopping reader
	HANDLE m_eventResume;
	HANDLE m_eventReadSample;
	wchar_t m_source[MAX_PATH];

	bool m_useFrameRate;
	bool m_isFirstTimeStamp;
	bool m_isEndOfFile;
	long long int  m_desiredFrameRate;
	long long int  m_firstImprintedTimeStamp;
	long long int  m_firstTimeStamp;
	long long int  m_nextDesiredTimestamp;
	long long int  m_firstSentFrameTicCount;
	long long int  m_nextDesiredTickCount;
    int      m_frameSkipFactor;
	SImageHeader* m_userBuffer1;
	SImageHeader* m_userBuffer2;
	SImageHeader* m_userBufferCurr;
	SImageHeader* m_userBufferPrev;

	CFfmpegReader*				m_ffmpegReader;
	CPerformanceLogger* m_logTimeStamp;
};
