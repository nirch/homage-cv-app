/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#pragma once

#include <string>
#include <math.h>

#include <windows.h>
#include <atlcomcli.h>
#include <dshow.h>
#include <qedit.h>

#ifdef _VIDEOCAP
#include "VidCap.h"
#endif

#include "ImageType/ImageType.h"

#define		CG_AUTO			0x1
#define		CG_MANUAL		0x2 


class CCameraGrabber
{
public:
	CCameraGrabber();


	~CCameraGrabber();

	void DeleteContent();

	HRESULT ReleaseGraph();


	int Open(	WCHAR filename[120], 
				int cameraIndex,
				int preferredWidth, 
				int preferredHeight );

	int GrabImage( image_type **im );

	int	GetWidth()	{ return( m_width ); }
	int	GetHeight()	{ return( m_height ); }


	int  SetWhiteBalance( long  value, long  flag );
	int  GetWhiteBalance( long *value, long *flag );
	int  GetRangeWhiteBalance( long *pMin, long *pMax, long *pSteppingDelta, long *pDefault, long *pCapsFlags );

	int  SetGain( long  value, long  flag );
	int  GetGain( long *value, long *flag );
	int  GetRangeGain( long *pMin, long *pMax, long *pSteppingDelta, long *pDefault, long *pCapsFlags );


	int  GetSharpness( long *value, long *flag );
	int  SetSharpness( long value, long flag );


	int  SetAntiFliker( long value, long flag );
	int  GetAntiFliker( long *value, long *flag );


	int  GetFocus( long *value, long *flag );
	int  SetFocus( long value, long flag );

	int SetExposureN( long value, long flag  );
	int GetExposureN( long *value, long *flag  );
	int GetRangeExposureN( long *pMin, long *pMax, long *pSteppingDelta, long *pDefault, long *pCapsFlags );



	static int CameraDevice(  wchar_t *aCamera[], int NC, int *nC );


	int IsExposureSupport();
	int SetExposure( long value, long flag  );
	int GetExposure( long *value, long *flag  );

	int IsPowerLineSupport();
	int SetPowerLine( long value, long flag  );
	int GetPowerLine( long *value, long *flag  );

	int IsRightLightSupport();
	int SetRightLight( long value, long flag  );
	int GetRightLight( long *value, long *flag  );

	int IsLp1ExposureSupport();
	int SetLp1Exposure( long value, long flag  );
	int GetLp1Exposure( long *value, long *flag  );

private:

	CComPtr<ICaptureGraphBuilder2>	m_graphBuilder;		// main graph builder
	CComPtr<IFilterGraph2>			m_filterGraph;		// filter graph
//	CComPtr<IGraphBuilder>			m_filterGraph;		// filter graph

	CComPtr<IBaseFilter>			m_pVideoCaptureFilter;

	CComPtr<ISampleGrabber>	m_sampleGrabber;	// sample grabber videoCam


	CComPtr<IMediaControl>	m_pMediaControl;

	IAMVideoProcAmp			*m_pVideoProcAmp;
	IAMCameraControl		*m_cameraControl;

#ifdef _VIDEOCAP
	IKsTopologyInfo	*m_pIKsTopologyInfo;
	IVideoProcAmp	*m_pIVideoProcAmp;
#endif
	int		m_width;
	int		m_height;
	int		m_bit;
	image_type	*m_im;



	HRESULT bindChosenCameraCapDevice(IBaseFilter **ppCap, int iCamera );


	HRESULT ConnectPins(IBaseFilter* outputFilter,
							WCHAR *outputPin,
							IBaseFilter* inputFilter,
							WCHAR *inputPin );


	HRESULT ConnectPinsCaputre(IBaseFilter* outputFilter, WCHAR *outputPinName,
								IBaseFilter* inputFilter, WCHAR *inputPinName ) ;


	HRESULT FindPin(IBaseFilter* baseFilter,
					PIN_DIRECTION direction,
					WCHAR *name,
					IPin** destPin);


	HRESULT PrintPin(IBaseFilter* baseFilter, PIN_DIRECTION direction );


	HRESULT FindPinCaputre(IBaseFilter* outputFilter, IPin	**pPin );



	HRESULT	SetCameraConfiguration( CComPtr<IBaseFilter> sourceFilter,
						int preferredWidth, int preferredHeight );

	HRESULT	CameraConfigurationLog( CComPtr<IBaseFilter> sourceFilter );


#ifdef _VIDEOCAP
	HRESULT CCameraGrabber::QueryIVideoProcAmpInterface();
#endif
};