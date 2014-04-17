


#include "Uigp/igp.h"
#include "Ulog/Log.h"
#pragma comment( lib, "msacm32.lib" )
//#pragma comment( lib, "strmiids-v6.lib" )
#pragma comment( lib, "strmiids.lib" )


#include <atlcomcli.h>

//#include <Vidcap.h>


#include "CameraGrabber.h"





#define CAPTURE_PIN  L"Capture" 
#define STILL_PIN	 L"Still" 
#define INPUT_PIN	 L"Input" 
#define OUTPUT_PIN	 L"Output" 


CCameraGrabber::CCameraGrabber()
{
	// This must be callled before 
	// interfaces can be accessed
	CoInitialize(NULL);


	m_pMediaControl = NULL;
	m_sampleGrabber = NULL;

	m_graphBuilder = NULL;

	m_filterGraph = NULL;

	m_pVideoProcAmp = NULL;

#ifdef _VIDEOCAP
	m_pIKsTopologyInfo = NULL;
	
	m_pIVideoProcAmp = NULL;
#endif
	m_im = NULL;


}


CCameraGrabber::~CCameraGrabber()
{
	DeleteContent();


	CoUninitialize();	
}


void CCameraGrabber::DeleteContent()
{

//	ReleaseGraph();

	if( m_sampleGrabber != NULL ){
		m_sampleGrabber.Release();
		m_sampleGrabber = NULL;
	}



	if( m_pMediaControl != NULL ){
		m_pMediaControl->Stop();

		m_pMediaControl = NULL;

		Sleep( 100 );
	}
		

	if( m_filterGraph != NULL ){
		m_filterGraph.Release();
		m_filterGraph = NULL;
	}

	if( m_pVideoCaptureFilter != NULL ){
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
	}

	if( m_graphBuilder != NULL ){
		m_graphBuilder.Release();
		m_graphBuilder = NULL;
	}




	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}

}

  
int CCameraGrabber::Open( WCHAR cameraType[120],
								   int cameraIndex,
								   int preferredWidth, 
								   int preferredHeight )
{
HRESULT hr;


	// Create the main object that runs the graph

	hr = m_graphBuilder.CoCreateInstance(CLSID_CaptureGraphBuilder2);


	hr = m_filterGraph.CoCreateInstance(CLSID_FilterGraph);

	if(  m_graphBuilder->SetFiltergraph(m_filterGraph) != S_OK )  
		return(-1);



	// bind to chosen CAMERA
	if( bindChosenCameraCapDevice( &m_pVideoCaptureFilter, cameraIndex ) == S_FALSE )
		return( -1 );



	//GetRightLight( m_pVideoCaptureFilter, 1 );
	//SetRightLight( m_pVideoCaptureFilter, 0 );
	//GetRightLight( m_pVideoCaptureFilter, 1 );

	//SetRightLight( m_pVideoCaptureFilter, 0 );

 
	// add camera to graph
	hr = m_filterGraph->AddFilter( m_pVideoCaptureFilter, cameraType );



	IAMVideoControl *pVideoControl; 
	if( m_pVideoCaptureFilter->QueryInterface(IID_IAMVideoControl, (void **)&pVideoControl) != S_OK )
		return( -1 );



	// get camera property controls
	hr = pVideoControl->QueryInterface(IID_IAMVideoProcAmp, (void **)&m_pVideoProcAmp); 
//	hr = pVideoControl->QueryInterface(IID_IAMCameraControl, (void **)&m_cameraControl);
	if( pVideoControl->QueryInterface(IID_IAMCameraControl, (void **)&m_cameraControl) != S_OK ){
		GPLOG( ("IAMCameraControl do not support.  %d\n", m_cameraControl== NULL ));
		m_cameraControl = NULL;
	}


	


	// Create the Sample Grabber which we will use
	CComPtr<IBaseFilter> grabberFilter;
	grabberFilter.CoCreateInstance(CLSID_SampleGrabber);
	grabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&m_sampleGrabber );





	// We have to set the 24-bit RGB desire here
	// So that the proper conversion filters
	// Are added automatically.
	AM_MEDIA_TYPE desiredType;
	memset(&desiredType, 0, sizeof(desiredType));
	desiredType.majortype = MEDIATYPE_Video;
	desiredType.subtype = MEDIASUBTYPE_RGB24;
	desiredType.formattype = FORMAT_VideoInfo;


	m_sampleGrabber->SetMediaType(&desiredType);
	hr = m_sampleGrabber->SetOneShot(TRUE);
	hr = m_sampleGrabber->SetBufferSamples(TRUE);

	m_filterGraph->AddFilter(grabberFilter, L"Sample Grabber");




	// Use pin connection methods instead of 
	// ICaptureGraphBuilder::RenderStream because of
	// the SampleGrabber setting we're using.
	if( ConnectPinsCaputre(m_pVideoCaptureFilter, CAPTURE_PIN, grabberFilter, INPUT_PIN ) == S_FALSE )
		return( -1 );     





	//// A Null Renderer does not display the video
	//// But it allows the Sample Grabber to run
	//// And it will keep proper playback timing
	//// Unless specified otherwise.

	CComPtr<IBaseFilter> nullRenderer;
	hr = nullRenderer.CoCreateInstance(CLSID_NullRenderer);

	hr = m_filterGraph->AddFilter( nullRenderer, L"Null Renderer");

	if( ConnectPins(grabberFilter, OUTPUT_PIN, nullRenderer, L"In" ) == S_FALSE )
		return( -1 );


	CameraConfigurationLog( m_pVideoCaptureFilter );

	// set camera resolution
	if( SetCameraConfiguration( m_pVideoCaptureFilter, preferredWidth, preferredHeight ) == S_FALSE )
		return( -1 );



#ifdef _VIDEOCAP
	QueryIVideoProcAmpInterface();
#endif
	// Tell the whole graph to start sending video
	// Apart from making sure the source filter can load
	// This is the only failure point we care about unless
	// You need to do more extensive development and debugging.
	/*CComPtr<IMediaControl>	m_pMediaControl = NULL;*/
	m_filterGraph->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );






	for( int i = 0 ; i < 5 ; i++ ){
		if( m_pMediaControl->Run() == S_OK ){

			CComQIPtr< IMediaEvent, &IID_IMediaEvent > pEvent( m_filterGraph );
			long EvCode = 0;

			hr = pEvent->WaitForCompletion( 2000, &EvCode );

			GPLOG(("Run Graph code: %d  (i = %d)\n", EvCode, i  ) );
			GPLOG_FLUSH();

			wprintf(L"runGraph evCode=%d\n", 
				EvCode);

			if( EvCode != S_OK )
				continue;


			return( 1 );
		}
		Sleep( 10 );
	}







	return( -1);
}


int CCameraGrabber::GrabImage( image_type **im )
{
static long	size;
int	width,	height;

	if( m_im == NULL ){
		if( m_sampleGrabber->GetCurrentBuffer( &size, NULL  ) !=  S_OK ){
			*im = NULL;
			return( -1 );
		}

		AM_MEDIA_TYPE pType;
		m_sampleGrabber->GetConnectedMediaType( &pType );

		VIDEOINFOHEADER* infoHeader = (VIDEOINFOHEADER*)pType.pbFormat;
		width = infoHeader->bmiHeader.biWidth;
		height= infoHeader->bmiHeader.biHeight;


		GPLOG(("GrabImage buffer size: %d  width: %d   height: %d\n", size, width, height  ) );
		GPLOG_FLUSH();




		u_char *data = ( u_char *)malloc( size+16 );
		m_im = image_create( height, width, 3, 1, data );
	}

	
	if( m_sampleGrabber->GetCurrentBuffer( &size, (long *)m_im->data  ) != S_OK ){
		*im = NULL;
		return( -1 );
	}

	m_im->timeStamp = vTime();
	*im = m_im;
	return( 1 );
}






HRESULT CCameraGrabber::ConnectPinsCaputre(IBaseFilter* outputFilter,
									WCHAR *outputPinName,
									IBaseFilter* inputFilter,
									WCHAR *inputPinName ) 
{
	IPin *inputPin;
	IPin *outputPin;


	PrintPin( outputFilter, PINDIR_OUTPUT );
	PrintPin( inputFilter, PINDIR_INPUT );


//	if( FindPin(outputFilter, PINDIR_OUTPUT, outputPinName, &outputPin) == S_FALSE )
//		return S_FALSE;

	if( FindPinCaputre(outputFilter, &outputPin )== S_FALSE )
		return S_FALSE;

#ifdef _TRCAE
	wprintf(L"OUTOUT %s\n",outputPinName );
#endif



	if( FindPin(inputFilter, PINDIR_INPUT, inputPinName, &inputPin) == S_FALSE )
		return S_FALSE;

#ifdef _TRCAE
	wprintf(L"INPUT %s\n",inputPinName );
#endif

	if( m_filterGraph->Connect(outputPin, inputPin) == S_FALSE )
		return( S_FALSE );

	return( S_OK );

}


HRESULT CCameraGrabber::ConnectPins(IBaseFilter* outputFilter,
							   WCHAR *outputPinName,
							   IBaseFilter* inputFilter,
							   WCHAR *inputPinName ) 
{
IPin *inputPin;
IPin *outputPin;


	PrintPin( outputFilter, PINDIR_OUTPUT );
	PrintPin( inputFilter, PINDIR_INPUT );




   if( FindPin(outputFilter, PINDIR_OUTPUT, outputPinName, &outputPin) == S_FALSE )
	   return S_FALSE;

#ifdef _TRCAE
   wprintf(L"OUTOUT %s\n",outputPinName );
#endif



	if( FindPin(inputFilter, PINDIR_INPUT, inputPinName, &inputPin) == S_FALSE )
		return S_FALSE;

#ifdef _TRCAE
	wprintf(L"INPUT %s\n",inputPinName );
#endif

	 if( m_filterGraph->Connect(outputPin, inputPin) == S_FALSE )
		 return( S_FALSE );

	 return( S_OK );

}









HRESULT CCameraGrabber::FindPin(IBaseFilter* baseFilter,
								PIN_DIRECTION direction,
								WCHAR *pinName,
								IPin	**destPin ) 
{
	CComPtr<IEnumPins> enumPins;

	*destPin = NULL;


	if( baseFilter->EnumPins(&enumPins) != S_OK )
		return( S_FALSE );


	ULONG numFound;
	IPin *tmpPin;
	PIN_INFO pinInfo;



	while( enumPins->Next(1, &tmpPin, &numFound) != S_FALSE ) 
	{
		PIN_DIRECTION pinDirection;

		tmpPin->QueryDirection(&pinDirection);
		if (pinDirection == direction) {

			tmpPin->QueryPinInfo(&pinInfo);
			pinInfo.pFilter->Release();
			pinInfo.pFilter = 0;

			if (wcscmp(pinInfo.achName, pinName) == 0 ) {
				*destPin = tmpPin;
				enumPins.Release();



				return( S_OK );
			}
		}
		tmpPin->Release();
	}

	return( S_FALSE );
}



HRESULT CCameraGrabber::FindPinCaputre(IBaseFilter* outputFilter, IPin **pPin )
{
	HRESULT hr;

	hr = m_graphBuilder->FindPin(
		outputFilter,                   // Pointer to the filter to search.
		PINDIR_OUTPUT,          // Search for an output pin.
		&PIN_CATEGORY_CAPTURE,  // Search for a preview pin.
		&MEDIATYPE_Video,       // Search for a video pin.
		TRUE,                   // The pin must be unconnected. 
		0,                      // Return the first matching pin (index 0).
		pPin);                // This variable receives the IPin pointer.
	//if (SUCCESS(hr))
	//{
	//	/* ... */
	//	pPin->Release();
	//}

	return( hr );


}


HRESULT CCameraGrabber::PrintPin(IBaseFilter* baseFilter,
									PIN_DIRECTION direction ) 
{
	CComPtr<IEnumPins> enumPins;

	
	if( baseFilter->EnumPins(&enumPins) != S_OK )
		return( S_FALSE );


	ULONG numFound;
	IPin *tmpPin;
	PIN_INFO pinInfo;

#ifdef _TRCAE
	wprintf(L"Pin  is:  ");
#endif
	while( enumPins->Next(1, &tmpPin, &numFound) != S_FALSE ) 
	{
		PIN_DIRECTION pinDirection;

		tmpPin->QueryDirection(&pinDirection);
		if (pinDirection == direction) {

			tmpPin->QueryPinInfo(&pinInfo);
			pinInfo.pFilter->Release();
			pinInfo.pFilter = 0;
#ifdef _TRCAE
			wprintf(L"%s   ", pinInfo.achName );
#endif
		}
		tmpPin->Release();
	}
#ifdef _TRCAE
	wprintf(L"\n");
#endif
	enumPins.Release();
	return( S_FALSE );
}



//
HRESULT CCameraGrabber::bindChosenCameraCapDevice( IBaseFilter **ppCap, int iCamera )
{
	HRESULT hr;


	*ppCap = NULL;
	
	// Create an enumerator
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	
	if(!pCreateDevEnum)
		return S_FALSE;

	// Enumerate video capture devices
	CComPtr<IEnumMoniker> pEm;
	pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);

	if( pEm == NULL  )
		return( S_FALSE );


	pEm->Reset();

	// Go through and find  capture device
	//


	ULONG ulFetched;
	CComPtr<IMoniker> pM;
	int i;


	for( i = 0 ; i < iCamera ; i++ ){
		if( pEm->Next(1, &pM, &ulFetched) != S_OK )
			return( -1 );

		pM.Release();
	}

	if( pEm->Next( 1, &pM, &ulFetched) != S_OK )
		return( S_FALSE );


	// Bind  moniker to an object.
	IPropertyBag *pPropBag;
	if( pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag ) != S_OK ){
		pM.Release();
		return( S_FALSE );
	}



	// To retrieve the filter's friendly name, 
	VARIANT varName;
	VariantInit(&varName);


	if( pPropBag->Read(L"FriendlyName", &varName, 0 ) == S_OK ){
		wprintf(L"Open Video Input Device is : %s\n", varName.bstrVal);
	
	}


	hr = pM->BindToObject(NULL, NULL, IID_IBaseFilter, (void **)ppCap);
	pM.Release();


	char	cameraName[512];
	wcstombs( cameraName, (wchar_t*)varName.bstrVal, 256 );
	GPLOG(("Open camera:  %s\n", cameraName ) );

	VariantClear(&varName);

	return( hr );
}


HRESULT CCameraGrabber::CameraConfigurationLog( CComPtr<IBaseFilter> sourceFilter )
{
	HRESULT hr;


	// set camera resolution
	CComPtr<IAMStreamConfig>    pConfig;

	hr = m_graphBuilder->FindInterface( &PIN_CATEGORY_CAPTURE, 
		&MEDIATYPE_Video, 
		sourceFilter, 
		IID_IAMStreamConfig, 
		(void**)&pConfig );

	int nCount = 0;
	int nSize = 0;

	hr = pConfig->GetNumberOfCapabilities(&nCount, &nSize);


	VIDEO_STREAM_CONFIG_CAPS    objVideoStreamConfig;
	AM_MEDIA_TYPE*              pMediaTypeConfig;


	int i;
	for( i = 0 ; i < nCount ; i++ ){

		if( FAILED( pConfig->GetStreamCaps(i, &pMediaTypeConfig, (BYTE*)&objVideoStreamConfig)) )
			return S_FALSE;


		if (pMediaTypeConfig->formattype != FORMAT_VideoInfo ){
			CoTaskMemFree(pMediaTypeConfig->pbFormat);
			continue;
		}


		VIDEOINFOHEADER* infoHeader = (VIDEOINFOHEADER*)pMediaTypeConfig->pbFormat;
		int width = infoHeader->bmiHeader.biWidth;
		int height = infoHeader->bmiHeader.biHeight;
		int bit = infoHeader->bmiHeader.biBitCount;


		GPLOG(("Configuration bit: %d  width: %d   height: %d\n", bit, width, height ) );


		CoTaskMemFree(pMediaTypeConfig->pbFormat);
	}



	return( (HRESULT)-1L ); // demanded resolution isn't supported
}


HRESULT CCameraGrabber::SetCameraConfiguration( CComPtr<IBaseFilter> sourceFilter, int preferredWidth, int preferredHeight )
{
HRESULT hr;


	// set camera resolution
	CComPtr<IAMStreamConfig>    pConfig;

	hr = m_graphBuilder->FindInterface( &PIN_CATEGORY_CAPTURE, 
											&MEDIATYPE_Video, 
											sourceFilter, 
											IID_IAMStreamConfig, 
											(void**)&pConfig );

	int nCount = 0;
	int nSize = 0;

	hr = pConfig->GetNumberOfCapabilities(&nCount, &nSize);


	VIDEO_STREAM_CONFIG_CAPS    objVideoStreamConfig;
	AM_MEDIA_TYPE*              pMediaTypeConfig;


	int i;
	for( i = 0 ; i < nCount ; i++ ){

		if( FAILED( pConfig->GetStreamCaps(i, &pMediaTypeConfig, (BYTE*)&objVideoStreamConfig)) )
			return false;


		if (pMediaTypeConfig->formattype != FORMAT_VideoInfo ){
			CoTaskMemFree(pMediaTypeConfig->pbFormat);
			continue;
		}


		VIDEOINFOHEADER* infoHeader = (VIDEOINFOHEADER*)pMediaTypeConfig->pbFormat;
		m_width = infoHeader->bmiHeader.biWidth;
		m_height = infoHeader->bmiHeader.biHeight;
		m_bit = infoHeader->bmiHeader.biBitCount;


		if ((m_width == preferredWidth) &&  (m_height == preferredHeight) ){

			pConfig->SetFormat(pMediaTypeConfig);

			CoTaskMemFree(pMediaTypeConfig->pbFormat);

			pConfig = NULL;

//			printf( "width: %d   height: %d   bit: %d\n", m_width, m_height, bit);
			GPLOGF(("Set Configuration bit: %d  width: %d   height: %d\n", m_bit, m_width, m_height  ) );
			
			return( S_OK );
		}

		CoTaskMemFree(pMediaTypeConfig->pbFormat);
	}



	return( S_FALSE );
}




HRESULT CCameraGrabber::ReleaseGraph()
{
//	CAutoLock cObjectLock(&m_CSec);

	HRESULT hr = m_pMediaControl->Stop();

	// Enumerate the filters in the graph.
	IEnumFilters *pEnum = NULL;
	hr = m_filterGraph->EnumFilters(&pEnum);
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		int	no = 0;
		while (S_OK == pEnum->Next(1, &pFilter, NULL))
		{
			FILTER_INFO f_info;
			HRESULT hr_f = pFilter->QueryFilterInfo(&f_info);

			// Remove the filter.
			hr = m_filterGraph->RemoveFilter(pFilter);

			if(SUCCEEDED(hr_f))
			{
				f_info.pGraph->Release();
			}

			// Reset the enumerator.
			no++;
			if( no < 2 )
				pEnum->Reset();
			pFilter->Release();
		}
		pEnum->Release();
	}

	m_pMediaControl.Release();
	m_filterGraph.Release();


	m_pMediaControl = NULL;
	m_graphBuilder = NULL;


	return(S_OK);

}


int  CCameraGrabber::GetWhiteBalance( long *value, long *flag )
{
HRESULT hr;


	hr = m_pVideoProcAmp->Get( VideoProcAmp_WhiteBalance, value, flag );
	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::SetWhiteBalance( long value, long flag )
{
HRESULT hr;


	if( flag == CG_AUTO ){
		hr = m_pVideoProcAmp->Set( VideoProcAmp_WhiteBalance,  0, VideoProcAmp_Flags_Auto );

		return( ( hr == S_OK )? 1 : -1 );
	}


	long pMin,	pMax, pSteppingDelta,	pDefault, pCapsFlags;	
	hr = GetRangeWhiteBalance( &pMin, &pMax, &pSteppingDelta, &pDefault, &pCapsFlags );

	value = (value / pSteppingDelta) * pSteppingDelta;


	hr = m_pVideoProcAmp->Set( VideoProcAmp_WhiteBalance,  value, VideoProcAmp_Flags_Manual );

	if( hr != S_OK )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::GetRangeWhiteBalance( long *pMin, long *pMax, long *pSteppingDelta, long *pDefault, long *pCapsFlags )
{
	HRESULT hr;
	hr = m_pVideoProcAmp->GetRange( VideoProcAmp_WhiteBalance,
		pMin,
		pMax,
		pSteppingDelta,
		pDefault,
		pCapsFlags);

	return( ( hr == S_OK )? 1 : -1 );
}

int  CCameraGrabber::GetGain( long *value, long *flag )
{
	HRESULT hr;


	hr = m_pVideoProcAmp->Get( VideoProcAmp_Gain, value, flag );
	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::SetGain( long value, long flag )
{
	HRESULT hr;


	if( flag == CG_AUTO ){
		hr = m_pVideoProcAmp->Set( VideoProcAmp_Gain,  0, VideoProcAmp_Flags_Auto );

		return( ( hr == S_OK )? 1 : -1 );
	}


	//long pMin,	pMax, pSteppingDelta,	pDefault, pCapsFlags;	
	//hr = GetRangeGaine( &pMin, &pMax, &pSteppingDelta, &pDefault, &pCapsFlags );

	//value = (value / pSteppingDelta) * pSteppingDelta;


	hr = m_pVideoProcAmp->Set( VideoProcAmp_Gain,  value, VideoProcAmp_Flags_Manual );

	if( hr != S_OK )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::GetRangeGain( long *pMin, long *pMax, long *pSteppingDelta, long *pDefault, long *pCapsFlags )
{
	HRESULT hr;
	hr = m_pVideoProcAmp->GetRange( VideoProcAmp_Gain,
		pMin,
		pMax,
		pSteppingDelta,
		pDefault,
		pCapsFlags);

	return( ( hr == S_OK )? 1 : -1 );

}

int  CCameraGrabber::GetSharpness( long *value, long *flag )
{
	HRESULT hr;


	hr = m_pVideoProcAmp->Get( VideoProcAmp_Sharpness, value, flag );
	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::SetSharpness( long value, long flag )
{
	HRESULT hr;


	flag = ( flag == CG_AUTO )? VideoProcAmp_Flags_Auto : VideoProcAmp_Flags_Manual;


	hr = m_pVideoProcAmp->Set( VideoProcAmp_Sharpness,  value, flag );

	if( FAILED(hr) )	return( -1 );

	return( 1 );


}


int  CCameraGrabber::SetAntiFliker( long value, long flag )
{
	//HRESULT hr;


	//flag = ( flag == CG_AUTO )? VideoProcAmp_Flags_Auto : VideoProcAmp_Flags_Manual;

	// value 0:Disabled      1=50  2=60
	// flag   VideoProcAmp_Flags_Auto or  VideoProcAmp_Flags_Manual ;   VideoProcAmp_Flags_Auto ignore   value

//	hr = m_pVideoProcAmp->put_PowerlineFrequency( value, flag );

	//if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::GetAntiFliker( long *value, long *flag )
{
	//HRESULT hr;

	// value 0:Disabled      1=50  2=60
	// flag   VideoProcAmp_Flags_Auto or  VideoProcAmp_Flags_Manual ;   VideoProcAmp_Flags_Auto ignore   value

//	hr = m_pVideoProcAmp->get_PowerlineFrequency( value, flag );

	//if( FAILED(hr) )	return( -1 );

	return( 1 );

}



int  CCameraGrabber::GetFocus( long *value, long *flag )
{
	HRESULT hr;

	if( m_cameraControl == NULL )
		return( -1 );


	hr = m_cameraControl->Get( CameraControl_Focus, value, flag );
	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::SetFocus( long value, long flag )
{
	HRESULT hr;
	if( m_cameraControl == NULL )
		return( -1 );

	flag = ( flag == CG_AUTO )? CameraControl_Flags_Auto : CameraControl_Flags_Manual;


	hr = m_cameraControl->Set( CameraControl_Focus,  value, flag );

	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::GetExposureN( long *value, long *flag )
{
	HRESULT hr;

	if( m_cameraControl == NULL )
		return( -1 );

	hr = m_cameraControl->Get( CameraControl_Exposure, value, flag );
	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::SetExposureN( long value, long flag )
{
	HRESULT hr;

	if( m_cameraControl == NULL )
		return( -1 );

	flag = ( flag == CG_AUTO )? CameraControl_Flags_Auto : CameraControl_Flags_Manual;


	hr = m_cameraControl->Set( CameraControl_Exposure,  value, flag );

	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int  CCameraGrabber::GetRangeExposureN( long *pMin, long *pMax, long *pSteppingDelta, long *pDefault, long *pCapsFlags )
{
	if( m_cameraControl == NULL )
		return( -1 );

	HRESULT hr;
	hr = m_cameraControl->GetRange( CameraControl_Exposure,
		pMin,
		pMax,
		pSteppingDelta,
		pDefault,
		pCapsFlags);

	return( ( hr == S_OK )? 1 : -1 );
}










