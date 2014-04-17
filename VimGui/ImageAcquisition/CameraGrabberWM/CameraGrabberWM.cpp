


#include "Uigp/igp.h"

//#pragma comment( lib, "msacm32.lib" )
#pragma comment( lib, "msacmce.lib" )

#pragma comment( lib, "strmiids.lib" )



#ifdef _DEBUG
#pragma comment(lib, "SampleRendererFilterD.lib")
#else
#pragma comment(lib, "SampleRendererFilter.lib")
#endif



#include <atlcomcli.h>

#include <initguid.h>

#include "CameraGrabberWM.h"

#include "CPropertyBag.h"



extern void doMsg( char *msg );
extern void doMsg2( char *msg, char *msg1 );


#define CAPTURE_PIN  L"Capture" 
#define STILL_PIN	 L"Still" 
#define INPUT_PIN	 L"Input" 
#define OUTPUT_PIN	 L"Output" 


CCameraGrabberWM::CCameraGrabberWM()
{
	CoInitialize(NULL);


	m_pMediaControl = NULL;
	m_sampleGrabber = NULL;

	m_graphBuilder = NULL;

	m_filterGraph = NULL;

	m_im = NULL;

	m_tim = NULL;
}


CCameraGrabberWM::~CCameraGrabberWM()
{
	DeleteContent();


	CoUninitialize();	
}



void CCameraGrabberWM::DeleteContent()
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



int CCameraGrabberWM::Open( WCHAR cameraType[120],
								   int cameraIndex,
								   int preferredWidth, 
								   int preferredHeight )
{
HRESULT hr;



	RegisterDLL( TEXT("SampleRendererFilter.dll") );


	// Create the main object that runs the graph
	hr = m_graphBuilder.CoCreateInstance(CLSID_CaptureGraphBuilder);


	hr = m_filterGraph.CoCreateInstance(CLSID_FilterGraph);
	if ( FAILED(hr))  return(-1);

//	doMsg( "4" );
	hr = m_graphBuilder->SetFiltergraph(m_filterGraph);
	if ( FAILED(hr))  return(-1);
//	doMsg( "5" );




	if( FAILED( CreateVideoCaptureFilter() ) )
		return( -1 );
//doMsg( "6" );

 
	// add camera to graph
	hr = m_filterGraph->AddFilter( m_pVideoCaptureFilter, cameraType );
	if( FAILED( hr ) )	return( -1 );

//doMsg( "7" );





	CComPtr<IBaseFilter> grabberFilter;
	if( FAILED(  grabberFilter.CoCreateInstance(CLSID_SampleRendererFilter) ) ){
		return( -1 );
	}


//		doMsg( "8" );


	hr = grabberFilter->QueryInterface(IID_ISampleRendererFilter, (void**)&m_sampleGrabber );

	if( FAILED( hr ) )	return( -1 );

//		doMsg( "9" );

	m_filterGraph->AddFilter(grabberFilter, L"SampleRender");

	if( FAILED(ConnectPins(m_pVideoCaptureFilter, CAPTURE_PIN, grabberFilter, L"In" )) )
		return( -1 );  


//	doMsg( "10" );




	// set camera resolution
//	if( FAILED(SetCameraConfiguration( m_pVideoCaptureFilter, preferredWidth, preferredHeight ) ) )
//		if( FAILED(SetCameraConfiguration( m_pVideoCaptureFilter, 320, 240 ) ) )
	if( FAILED(SetCameraConfiguration( m_pVideoCaptureFilter, 640, 480 ) ) )

		return( -1 );


//doMsg( "15" );


	// Tell the whole graph to start sending video
	// Apart from making sure the source filter can load
	// This is the only failure point we care about unless
	// You need to do more extensive development and debugging.
	/*CComPtr<IMediaControl>	m_pMediaControl = NULL;*/
	m_filterGraph->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );
	if (FAILED(m_pMediaControl->Run()))
		return( -1 );


	return( 1);
}


int
gpFile_write_bufferN(char *file, char *data, int bytes )
{
	FILE *fp;

	if( (fp = fopen(file, "wb")) == NULL )
		return( -1 );

	fwrite( data, bytes, 1, fp);


	fclose(fp);

	return( 1 );
}

int CCameraGrabberWM::GrabImage( image_type **im )
{
char	msg[256];


	if( m_tim == NULL )
//		m_tim = image_create( m_height, m_width, 3, 1, NULL );
		m_tim = image_create( m_width, m_height, 3, 1, NULL );

		
	int	size;
	if( FAILED( m_sampleGrabber->GetCurrentBuffer( &size, m_tim->data  ) ) ){
		*im = NULL;
//		sprintf( msg, " %d %d  %d", m_width, m_height, size );
//		doMsg2( "grab-failed", msg );


		return( -1 );
	}

	m_im = image3_rotate90( m_tim, m_im );



	
//	sprintf( msg, " %d %d  %d", m_width, m_height, size );
//	doMsg2( "grab", msg );



	*im = m_im;
	return( 1 );
}









HRESULT CCameraGrabberWM::ConnectPins(IBaseFilter* outputFilter,
							   WCHAR *outputPinName,
							   IBaseFilter* inputFilter,
							   WCHAR *inputPinName ) 
{
CComPtr<IPin> inputPin;
CComPtr<IPin> outputPin;


   if (FAILED(FindPin(outputFilter, PINDIR_OUTPUT, outputPinName, &outputPin)))
	   return S_FALSE;



	if (FAILED(FindPin(inputFilter, PINDIR_INPUT, inputPinName, &inputPin)))
		return S_FALSE;



	 if (FAILED( m_filterGraph->Connect(outputPin, inputPin)) )
		 return( S_FALSE );

	 return( S_OK );

}

HRESULT CCameraGrabberWM::FindPin(IBaseFilter* baseFilter,
								PIN_DIRECTION direction,
								WCHAR *pinName,
								IPin	**destPin ) 
{
	CComPtr<IEnumPins> enumPins;

	*destPin = NULL;



	if (FAILED(baseFilter->EnumPins(&enumPins))) 
		return( S_FALSE );


	ULONG numFound;
	IPin *tmpPin;
	PIN_INFO pinInfo;
	while (SUCCEEDED(enumPins->Next(1, &tmpPin, &numFound))) 
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


HRESULT
CCameraGrabberWM::CreateVideoCaptureFilter()
{
CComVariant   varCamName;
CPropertyBag  PropBag;
WCHAR		wzDeviceName[ MAX_PATH + 1 ];
CComPtr<IPersistPropertyBag>    pPropertyBag;
HRESULT	hr;

	//
	// Create and initialize the video capture filter
	//
	hr =  m_pVideoCaptureFilter.CoCreateInstance( CLSID_VideoCapture );
	if( FAILED(hr ))	return( hr );

	hr = m_pVideoCaptureFilter.QueryInterface( &pPropertyBag );
	if( FAILED(hr ))	return( hr );

	// We are loading the driver CAM1 in the video capture filter. 
	hr = GetFirstCameraDriver( wzDeviceName );
	if( FAILED(hr ))	return( hr );

	varCamName = wzDeviceName;
	if( varCamName.vt != VT_BSTR )
		return( E_OUTOFMEMORY );

	hr =  PropBag.Write( L"VCapName", &varCamName );   
	if( FAILED(hr ))	return( hr );

	hr = pPropertyBag->Load( &PropBag, NULL );
	if( FAILED(hr ))	return( hr );

	// Everything succeeded, the video capture filter is added to the filtergraph
	hr = m_filterGraph->AddFilter( m_pVideoCaptureFilter, L"Video Capture Filter Source" );
	if( FAILED(hr ))	return( hr );

	return( S_OK );
}



HRESULT 
CCameraGrabberWM::GetFirstCameraDriver( WCHAR *pwzName )
{
	HRESULT hr = S_OK;
	HANDLE	handle = NULL;
	DEVMGR_DEVICE_INFORMATION di;
	GUID guidCamera = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86 };
//	GUID guidCamera = DEVCLASS_CAMERA_GUID;
	char szName [] = "CAM0";
	// Note about the above: The driver material doesn't ship as part of the SDK. This GUID is hardcoded
	// here to be able to enumerate the camera drivers and pass the name of the driver to the video capture filter

	if( pwzName == NULL ){
		return E_POINTER;
	}

	di.dwSize = sizeof(di);

	handle = FindFirstDevice( DeviceSearchByGuid, &guidCamera, &di );
	//handle = FindFirstDevice(DeviceSearchByDeviceName, szName, &di );
	if(( handle == NULL ) || ( di.hDevice == NULL )){
		hr = HRESULT_FROM_WIN32( GetLastError() );
		FindClose( handle );
		return hr;
	}

	StringCchCopy( pwzName, MAX_PATH, di.szLegacyName );

	if(  FindNextDevice( handle, &di ) ) {
		StringCchCopy( pwzName, MAX_PATH, di.szLegacyName );
	}

	FindClose( handle );
	return S_OK;
}



HRESULT
CCameraGrabberWM::RegisterDLL( TCHAR* Filename)
{
	HINSTANCE hLib = LoadLibrary(Filename);
	if (hLib == NULL ){
		doMsg( "1-1-0" );
		return( S_FALSE );
	}


#ifdef WINCE
	FARPROC fn = GetProcAddress(hLib, TEXT("DllRegisterServer"));
#else
	FARPROC fn = GetProcAddress(hLib, "DllRegisterServer");
#endif

	if( fn == NULL ){
		FreeLibrary(hLib);
//		doMsg( "1-2-0" );
		return( S_FALSE );
	}


	fn();


	FreeLibrary(hLib);


	return( S_OK );
}



HRESULT CCameraGrabberWM::SetCameraConfiguration( CComPtr<IBaseFilter> sourceFilter, int preferredWidth, int preferredHeight )
{
HRESULT hr;
char	msg[256];


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
//		m_width = infoHeader->bmiHeader.biWidth;
//		m_height = infoHeader->bmiHeader.biHeight;
		m_height = infoHeader->bmiHeader.biWidth;
		m_width = infoHeader->bmiHeader.biHeight;


//		sprintf( msg, " %d %d  %d", m_width, m_height, infoHeader->bmiHeader.biBitCount );
//		doMsg2( "size - 1", msg );


	//	if ((m_width == preferredWidth) &&  (m_height == preferredHeight) ){
		if ((m_width == preferredHeight) &&  (m_height == preferredWidth) ){

			pConfig->SetFormat(pMediaTypeConfig);
			CoTaskMemFree(pMediaTypeConfig->pbFormat);

//			sprintf( msg, " %d %d %d", m_width, m_height, infoHeader->bmiHeader.biBitCount  );
//			doMsg2( "size - 2", msg );

			return( S_OK );
		}

		CoTaskMemFree(pMediaTypeConfig->pbFormat);
	}



	return( S_FALSE );
}


