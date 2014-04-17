


#include "Uigp/igp.h"
#include "Ulog/Log.h"


//#include <atlcomcli.h>

#include <Vidcap.h>
#include <KsMedia.h>
#include "CameraGrabber.h"



// fliker


HRESULT FindExtensionNode(IKsTopologyInfo* pIksTopologyInfo, GUID extensionGuid, DWORD* pdwNodeId);

///////////////////////////////////////////////////////////////

//IKsTopologyInfo* m_pIKsTopologyInfo = NULL;
//IVideoProcAmp* m_pIVideoProcAmp = NULL;



HRESULT CCameraGrabber::QueryIVideoProcAmpInterface()
{
HRESULT hr;

	//Query for IKsTopologyInfo after fixing the Video Size .
	hr = m_pVideoCaptureFilter->QueryInterface( __uuidof(IKsTopologyInfo) , (LPVOID*)&m_pIKsTopologyInfo );

	if( hr != S_OK || m_pIKsTopologyInfo == NULL )
		return( S_FALSE );




	//hr = m_pKSProp->QuerySupported(PROPSETID_VIDCAP_VIDEOPROCAMP, KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY, &dwSupported);

	//hr = m_pKSProp->Set(PROPSETID_VIDCAP_VIDEOPROCAMP,
	//	KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY,
	//	INSTANCEDATA_OF_PROPERTY_PTR(&Frequency),
	//	INSTANCEDATA_OF_PROPERTY_SIZE(Frequency),
	//	&Frequency,
	//	sizeof(Frequency));





		//Find extension node .
	DWORD dwNodeId = 0;
	hr = FindExtensionNode(m_pIKsTopologyInfo, KSNODETYPE_VIDEO_PROCESSING, &dwNodeId);

	if( hr != S_OK || dwNodeId < 0 )
		return( S_FALSE );

	hr = m_pIKsTopologyInfo->CreateNodeInstance(dwNodeId, __uuidof(IVideoProcAmp), reinterpret_cast<void**>(&m_pIVideoProcAmp));

//	hr = m_pIKsTopologyInfo->CreateNodeInstance(dwNodeId,  IID_IVideoProcAmp, reinterpret_cast<void**>(&m_pIVideoProcAmp));




	//hr = m_graphBuilder->FindInterface(
	//	NULL,//[in]   const GUID *pCategory,
	//	NULL,//[in]   const GUID *pType,
	//	m_pVideoCaptureFilter,//[in]   IBaseFilter *pf,
	//	__uuidof(IVideoProcAmp), reinterpret_cast<void**>(&m_pIVideoProcAmp) );
	////		[in]   REFIID riid,
	////		[out]  void **ppint
	////		);



	return( hr );
}




HRESULT FindExtensionNode(IKsTopologyInfo* pIksTopologyInfo, GUID extensionGuid, DWORD* pdwNodeId)
{
	DWORD dwNumberOfNodes;
	HRESULT hResult;

	hResult = pIksTopologyInfo->get_NumNodes(&dwNumberOfNodes);
	if (SUCCEEDED(hResult))
	{
		DWORD dwI;
		GUID nodeGuid;
		for (dwI = 0; dwI < dwNumberOfNodes; dwI++)
		{
			if (SUCCEEDED(pIksTopologyInfo->get_NodeType(dwI, &nodeGuid)))
			{
				if (IsEqualGUID(extensionGuid, nodeGuid))
				{ // Found the extension node
					*pdwNodeId = dwI;
					break;
				}
			}

		}

		if (dwI == dwNumberOfNodes)
		{ // Did not find the node
			hResult = S_FALSE;
		}
	}

	return hResult;
}




////////////////////////////////////////////////////////////////
/*
HRESULT AntiFlicker( long lVal, int iOpt )
{
	HRESULT hr = S_OK;

	if ( NULL == m_pIVideoProcAmp )
	{
		return S_FALSE;
	}

	if ( Get == iOpt )
	{
		long lValue;
		long lFlag;

		hr = m_pIVideoProcAmp->get_PowerlineFrequency(&lValue, &lFlag);
		if ( S_OK != hr )
		{
			return S_FALSE;
		}
	}
	else
	{
		switch ( lVal )
		{
		case 60:
			hr = m_pIVideoProcAmp->put_PowerlineFrequency(2, VideoProcAmp_Flags_Manual);
			break;
		case 50:
			hr = m_pIVideoProcAmp->put_PowerlineFrequency(1, VideoProcAmp_Flags_Manual);
			break;
		case 0:
			hr = m_pIVideoProcAmp->put_PowerlineFrequency(0, VideoProcAmp_Flags_Manual);
			break;
		}

		if ( S_OK != hr )
		{
			return S_FALSE;
		}
	}


	return S_OK;
}
*/
//m_pIKsTopologyInfo = NULL;
//m_pIVideoProcAmp = NULL;





#ifdef AA
HRESULT FindExtensionNode( IKsTopologyInfo* pIksTopologyInfo,
						  GUID extensionGuid,
						  DWORD* pNodeId )
{
	DWORD numberOfNodes;
	HRESULT hResult;

	hResult = pIksTopologyInfo->get_NumNodes(&numberOfNodes);
	if (SUCCEEDED(hResult))
	{
		DWORD i;
		GUID nodeGuid;
		for (i = 0; i < numberOfNodes; i++)
		{
			if (SUCCEEDED(pIksTopologyInfo->get_NodeType(i, &nodeGuid)))
			{
				if (IsEqualGUID(extensionGuid, nodeGuid))
				{ // Found the extension node
					*pNodeId = i;
					break;
				}
			}

		}

		if (i == numberOfNodes)
		{ // Did not find the node
			hResult = S_FALSE;
		}
	}

	return hResult;
}
#endif