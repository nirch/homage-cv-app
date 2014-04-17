


#include "Uigp/igp.h"
#include "Ulog/Log.h"




#include "CameraGrabber.h"
#include <Ks.h>
#include <KsMedia.h>

#include "LVUVCPublic.h"




int CCameraGrabber::IsLp1ExposureSupport()
{
IKsPropertySet *pKsPropertySet;
HRESULT hr;


	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);

	if( FAILED(hr) )	return( -1 );

	DWORD pTypeSupport;

	hr = pKsPropertySet->QuerySupported(
		PROPSETID_LOGITECH_PUBLIC1,
		KSPROPERTY_LP1_EXPOSURE_TIME,
		&pTypeSupport );

	
	if( FAILED(hr) )	return( -1 );

	return( pTypeSupport );

}




int CCameraGrabber::SetLp1Exposure( long value, long flag  )
{
	IKsPropertySet *pKsPropertySet;
	KSPROPERTY_LP1_EXPOSURE_TIME_S aep;
	HRESULT hr;

	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
	if( FAILED(hr) )	return( -1 );


	memset(&aep, 0, sizeof(aep));
	//aep.Property.Set = PROPSETID_LOGITECH_PUBLIC1;
	//aep.Property.Id = KSPROPERTY_LP1_EXPOSURE_TIME;
	//aep.Property.Flags = KSPROPERTY_TYPE_SET;


	aep.Header.ulFlags = ( flag == CG_AUTO )? KSPROPERTY_CAMERACONTROL_FLAGS_AUTO : KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
//	aep.Flags |=  KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE;

//	aep.Capabilities = aep.Flags;
	aep.ulExposureTime = value;

	hr = pKsPropertySet->Set(
		PROPSETID_LOGITECH_PUBLIC1,
		KSPROPERTY_LP1_EXPOSURE_TIME,
		NULL,//PKSPROPERTY(&aep) + 1,
		0,//sizeof(aep) - sizeof(KSPROPERTY),
		&aep,
		sizeof(aep)
		);

	if( FAILED(hr) )	return( -1 );

	return( 1 );

}

int CCameraGrabber::GetLp1Exposure( long *value, long *flag  )
{
	IKsPropertySet *pKsPropertySet;
	KSPROPERTY_LP1_EXPOSURE_TIME_S aep;
	HRESULT hr;

	hr = m_pVideoProcAmp->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
	if( FAILED(hr) )	return( -1 );

	memset(&aep, 0, sizeof(aep));
	//aep.Property.Set = PROPSETID_LOGITECH_PUBLIC1;
	//aep.Property.Id = KSPROPERTY_LP1_EXPOSURE_TIME;
	//aep.Property.Flags = KSPROPERTY_TYPE_GET;

	//aep.Flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO
;//_ABSOLUTE;



	DWORD a;
	hr = pKsPropertySet->Get(
		PROPSETID_LOGITECH_PUBLIC1,
		KSPROPERTY_LP1_EXPOSURE_TIME,
		NULL,//PKSPROPERTY(&aep) + 1,
		0,//sizeof(aep) - sizeof(KSPROPERTY),
		&aep,
		sizeof(aep), &a
		);

	if( FAILED(hr) )	return( -1 );


	*value = aep.ulExposureTime ;

	if(aep.Header.ulFlags ==  KSPROPERTY_CAMERACONTROL_FLAGS_AUTO )
		*flag =  CG_AUTO;


	if(aep.Header.ulFlags ==  KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL )
		*flag =  CG_MANUAL;

	return( 1 );
}

