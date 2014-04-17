#pragma once

#include <string>
#include <math.h>

#include <windows.h>
#include <atlcomcli.h>
#include <dshow.h>



#include "ImageType/ImageType.h"



#define NNN

#include "SampleRendererFilter/SampleRendererGuid.h"



class CCameraGrabberWM
{
public:
	CCameraGrabberWM();


	~CCameraGrabberWM();

	void DeleteContent();


	int Open(	WCHAR filename[120], 
				int cameraIndex,
				int preferredWidth, 
				int preferredHeight );

	int GrabImage( image_type **im );

	int	GetWidth()	{ return( m_width ); }
	int	GetHeight()	{ return( m_height ); }


private:

	CComPtr<ICaptureGraphBuilder2>	m_graphBuilder;		// main graph builder
	CComPtr<IFilterGraph2>			m_filterGraph;		// filter graph

	CComPtr<IBaseFilter>			m_pVideoCaptureFilter;



	CComPtr<ISampleRendererFilter>	m_sampleGrabber;	// sample grabber videoCam


	CComPtr<IMediaControl>	m_pMediaControl;


	int				m_width;
	int				m_height;
	image_type	*m_im;
	image_type	*m_tim;



	HRESULT	CreateVideoCaptureFilter();

	HRESULT GetFirstCameraDriver( WCHAR *pwzName );


	HRESULT ConnectPins(IBaseFilter* outputFilter,
							WCHAR *outputPin,
							IBaseFilter* inputFilter,
							WCHAR *inputPin );


	HRESULT FindPin(IBaseFilter* baseFilter,
					PIN_DIRECTION direction,
					WCHAR *name,
					IPin** destPin);


	HRESULT	RegisterDLL( TCHAR* Filename);


	HRESULT	SetCameraConfiguration( CComPtr<IBaseFilter> sourceFilter,
						int preferredWidth, int preferredHeight );


};