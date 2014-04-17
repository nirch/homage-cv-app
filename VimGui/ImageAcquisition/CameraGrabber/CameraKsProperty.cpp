


#include "Uigp/igp.h"
#include "Ulog/Log.h"




#include "CameraGrabber.h"
#include <Ks.h>
#include <KsMedia.h>




int CCameraGrabber::IsExposureSupport()
{
IKsPropertySet *pKsPropertySet;
HRESULT hr;


	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);

	if( FAILED(hr) )	return( -1 );

	DWORD pTypeSupport;

	hr = pKsPropertySet->QuerySupported(
		PROPSETID_VIDCAP_CAMERACONTROL,
		KSPROPERTY_CAMERACONTROL_EXPOSURE,
		&pTypeSupport );

	
	if( FAILED(hr) )	return( -1 );

	return( pTypeSupport );

}




int CCameraGrabber::SetExposure( long value, long flag  )
{
	IKsPropertySet *pKsPropertySet;
	KSPROPERTY_CAMERACONTROL_S aep;
	HRESULT hr;

	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
	if( FAILED(hr) )	return( -1 );


	memset(&aep, 0, sizeof(aep));
	aep.Property.Set = PROPSETID_VIDCAP_CAMERACONTROL;
	aep.Property.Id = KSPROPERTY_CAMERACONTROL_EXPOSURE;
	aep.Property.Flags = KSPROPERTY_TYPE_SET;


	aep.Flags = ( flag == CG_AUTO )? KSPROPERTY_CAMERACONTROL_FLAGS_AUTO : KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
//	aep.Flags |=  KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE;

//	aep.Capabilities = aep.Flags;
	aep.Value = value;

	hr = pKsPropertySet->Set(
		PROPSETID_VIDCAP_CAMERACONTROL,
		KSPROPERTY_CAMERACONTROL_EXPOSURE,
		PKSPROPERTY(&aep) + 1,
		sizeof(aep) - sizeof(KSPROPERTY),
		&aep,
		sizeof(aep)
		);

	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int CCameraGrabber::GetExposure( long *value, long *flag  )
{
	IKsPropertySet *pKsPropertySet;
	KSPROPERTY_CAMERACONTROL_S aep;
	HRESULT hr;

	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
	if( FAILED(hr) )	return( -1 );

	memset(&aep, 0, sizeof(aep));
	aep.Property.Set = PROPSETID_VIDCAP_CAMERACONTROL;
	aep.Property.Id = KSPROPERTY_CAMERACONTROL_EXPOSURE;
	aep.Property.Flags = KSPROPERTY_TYPE_GET;

	//aep.Flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO
;//_ABSOLUTE;



	DWORD a;
	hr = pKsPropertySet->Get(
		PROPSETID_VIDCAP_CAMERACONTROL,
		KSPROPERTY_CAMERACONTROL_EXPOSURE,
		PKSPROPERTY(&aep) + 1,
		sizeof(aep) - sizeof(KSPROPERTY),
		&aep,
		sizeof(aep), &a
		);

	if( FAILED(hr) )	return( -1 );


	*value = aep.Value ;

	if( aep.Flags ==  KSPROPERTY_CAMERACONTROL_FLAGS_AUTO )
		*flag =  CG_AUTO;


	if( aep.Flags ==  KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL )
		*flag =  CG_MANUAL;

	return( 1 );
}


int CCameraGrabber::IsPowerLineSupport()
{
	IKsPropertySet *pKsPropertySet;
	HRESULT hr;


	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);

	if( FAILED(hr) )	return( -1 );

	DWORD pTypeSupport;

	hr = pKsPropertySet->QuerySupported(
		PROPSETID_VIDCAP_VIDEOPROCAMP,
		KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY,
		&pTypeSupport );


	if( FAILED(hr) )	return( -1 );

	return( pTypeSupport );

}




int CCameraGrabber::SetPowerLine( long value, long flag  )
{
	IKsPropertySet *pKsPropertySet;
	KSPROPERTY_VIDEOPROCAMP_S aep;
	HRESULT hr;

	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
	if( FAILED(hr) )	return( -1 );


	memset(&aep, 0, sizeof(aep));
	aep.Property.Set = PROPSETID_VIDCAP_CAMERACONTROL;
	aep.Property.Id = KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY;

	aep.Flags = ( flag == CG_AUTO )? KSPROPERTY_CAMERACONTROL_FLAGS_AUTO : KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
	aep.Value = value;

	hr = pKsPropertySet->Set(
		PROPSETID_VIDCAP_VIDEOPROCAMP,
		KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY,
		PKSPROPERTY(&aep) + 1,
		sizeof(aep) - sizeof(KSPROPERTY),
		&aep,
		sizeof(aep)
		);

	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int CCameraGrabber::GetPowerLine( long *value, long *flag  )
{
	IKsPropertySet *pKsPropertySet;
	 KSPROPERTY_VIDEOPROCAMP_S aep;
	HRESULT hr;

	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
	if( FAILED(hr) )	return( -1 );

	memset(&aep, 0, sizeof(aep));
	aep.Property.Set = PROPSETID_VIDCAP_CAMERACONTROL;
	aep.Property.Id = KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY;



	DWORD a;
	hr = pKsPropertySet->Get(
		PROPSETID_VIDCAP_VIDEOPROCAMP,
		KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY,
		PKSPROPERTY(&aep) + 1,
		sizeof(aep) - sizeof(KSPROPERTY),
		&aep,
		sizeof(aep), &a
		);

	if( FAILED(hr) )	return( -1 );


	*value = aep.Value ;

	if( aep.Flags ==  KSPROPERTY_CAMERACONTROL_FLAGS_AUTO )
		*flag =  CG_AUTO;


	if( aep.Flags ==  KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL )
		*flag =  CG_MANUAL;

	return( 1 );
}




int CCameraGrabber::IsRightLightSupport()
{
	IKsPropertySet *pKsPropertySet;
	HRESULT hr;


	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);

	if( FAILED(hr) )	return( -1 );

	DWORD pTypeSupport;

	hr = pKsPropertySet->QuerySupported(
		PROPSETID_VIDCAP_CAMERACONTROL,
		KSPROPERTY_CAMERACONTROL_AUTO_EXPOSURE_PRIORITY,
		&pTypeSupport );


	if( FAILED(hr) )	return( -1 );

	return( pTypeSupport );

}



int CCameraGrabber::SetRightLight( long value, long flag  )
{
IKsPropertySet *pKsPropertySet = NULL;
KSPROPERTY_CAMERACONTROL_S aep;
HRESULT hr;

	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
	if( FAILED(hr) )	return( -1 );


	memset(&aep, 0, sizeof(aep));
	aep.Property.Set = PROPSETID_VIDCAP_CAMERACONTROL;
	aep.Property.Id = KSPROPERTY_CAMERACONTROL_AUTO_EXPOSURE_PRIORITY;
	aep.Value = ( flag == CG_AUTO )? 1 : 0;

	aep.Flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;//( flag == CG_AUTO )? KSPROPERTY_CAMERACONTROL_FLAGS_AUTO : KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
	aep.Value = ( flag == CG_AUTO )? 1: 0;

	hr = pKsPropertySet->Set(
							 PROPSETID_VIDCAP_CAMERACONTROL,
							 KSPROPERTY_CAMERACONTROL_AUTO_EXPOSURE_PRIORITY,
							 PKSPROPERTY(&aep) + 1,
							 sizeof(aep) - sizeof(KSPROPERTY),
							 &aep,
							 sizeof(aep)
						 );

	if( FAILED(hr) )	return( -1 );

	return( 1 );

}


int CCameraGrabber::GetRightLight( long *value, long *flag  )
{
IKsPropertySet *pKsPropertySet = NULL;
KSPROPERTY_CAMERACONTROL_S aep;
HRESULT hr;
DWORD	a;

	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
	if( FAILED(hr) )	return( -1 );

	memset(&aep, 0, sizeof(aep));
	aep.Property.Set = PROPSETID_VIDCAP_CAMERACONTROL;
	aep.Property.Id = KSPROPERTY_CAMERACONTROL_AUTO_EXPOSURE_PRIORITY;
	


	DWORD pTypeSupport;

	hr = pKsPropertySet->QuerySupported(
									PROPSETID_VIDCAP_CAMERACONTROL,
									KSPROPERTY_CAMERACONTROL_AUTO_EXPOSURE_PRIORITY,
									&pTypeSupport );

	hr = pKsPropertySet->Get(
							 PROPSETID_VIDCAP_CAMERACONTROL,
							 KSPROPERTY_CAMERACONTROL_AUTO_EXPOSURE_PRIORITY,
							 PKSPROPERTY(&aep) + 1,
							 sizeof(aep) - sizeof(KSPROPERTY),
							 &aep,
							 sizeof(aep), &a
						 );

	if( FAILED(hr) )	return( -1 );

	*value = aep.Value;

	return( 1 );
}