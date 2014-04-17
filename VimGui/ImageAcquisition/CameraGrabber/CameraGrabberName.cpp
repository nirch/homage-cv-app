


#include "Uigp/igp.h"
#include "Ulog/Log.h"
#pragma comment( lib, "msacm32.lib" )
//#pragma comment( lib, "strmiids-v6.lib" )
#pragma comment( lib, "strmiids.lib" )



#include <atlcomcli.h>

//#include <Vidcap.h>
#include <comdef.h>


#include "CameraGrabber.h"

using namespace std;

//
int CCameraGrabber::CameraDevice( wchar_t *aCamera[], int NC, int *nC )
{


	CoInitialize(NULL);

	
	// Create an enumerator
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	
	if(!pCreateDevEnum)
		return( -1 );

	// Enumerate video capture devices
	CComPtr<IEnumMoniker> pEm;
	pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);

	if( pEm == NULL  )
		return( -1 );


	pEm->Reset();

	// Go through and find  capture device
	//


	ULONG ulFetched;
	CComPtr<IMoniker> pM;


	// Bind  moniker to an object.
	IPropertyBag *pPropBag;
	VARIANT varName;

	*nC = 0;
	while( pEm->Next(1, &pM, &ulFetched) == S_OK ){

		if( pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag ) != S_OK ){
			pM.Release();
			continue;
		}


		VariantInit(&varName);


		if( pPropBag->Read(L"FriendlyName", &varName, 0 ) == S_OK ){


			aCamera[(*nC)++] = wcsdup( varName.bstrVal );
			if( *nC >= NC )	break;

			
//			wprintf(L"Selected Video Input Device is : %s\n", varName.bstrVal );
//
//			wchar_t *a = wcsdup( varName.bstrVal );
//			wprintf(L"Selected Video Input Device is : %s\n", a );
//
//
//			string b = bstr_t( varName.bstrVal );
////			wsprintf(str, "Selected Video Input Device is : %s\n", varName.bstrVal);
//			aCamera[(*nC)++] = strdup( (char *)varName.bstrVal );
////			wprintf(L"Selected Video Input Device is : %s\n", varName.bstrVal);
		}
		VariantClear(&varName);



		int	i;
		for( i = 0 ; i < *nC ; i++ )
			wprintf(L"%s\n", aCamera[i] );

		

		pM.Release();
	}



	return( *nC );
}
