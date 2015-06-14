
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"


#include "UniformBackground.h"

#include "UnBackground/UnBackground.h"

#include "..\DynamicMask\DynamicMask.h"




int	CUniformBackground::ProcessDynamicMaskInit()
{
	if( m_unBackground == NULL )
		return( -1 );

	m_dm = new CDynamicMask();


	

	m_dm->Init( m_yim, m_unBackground->m_imMask, m_unBackground->m_apt, 16 );

	return( 1 );
}

int	CUniformBackground::ProcessDynamicMask()
{
	if( m_dm == NULL )
		return( -1 );


	gpTime_start( &m_tEdge );

	m_dm->Process( m_yim, m_iFrame );

	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "m", m_iFrame, "1" );
	}

	m_dm->ProcessMaskN( m_yim, m_cim, m_iFrame );


	m_dm->MoveIm( m_yim, m_iFrame );


	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "m", m_iFrame, "2" );
	}



	gpTime_stop( &m_tEdge );


	return( 1 );
}

