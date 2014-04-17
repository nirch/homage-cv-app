// Brandwit.cpp: implementation of the CHomage class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <string.h>



#ifdef _DEBUG
#define _DUMP
#endif

#include "Uigp/igp.h"
#include "Ulog/Log.h"
#include "Uln/PlnType.h"

#include "Gpw/Gpw.h"


#include "Homage.h"

#include "MattingLib/ColorBackground/UniformBackground.h"


CHomage::CHomage( CFrameBuffer *fb )
{
	m_fb = fb;

	m_im = NULL;
    
    m_aim = NULL;

    m_cb = NULL;
    
    m_process = 0;

	gpTime_init( &m_gTime );
    gpTime_init( &m_tProcess );
    
   
}



CHomage::~CHomage()
{



	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}




	DeleteContents();
}






void CHomage::DeleteContents()
{


}











int CHomage::Init( char *ctrFile, int width, int height )
{

	gpTime_init( &m_tProcess );

	m_cb = new  CUniformBackground();

    m_cb->ReadMask( ctrFile, width, height );

	return( 1 );
}






int CHomage::Process( image_type *sim, int iFrame )
{
image_type *bim;
int	ret;
    
    
    if( m_process == 0 ){
        gpTime_start( &m_gTime );
        m_fb->Set( sim );
        gpTime_stop( &m_gTime );
        return( 1 );
    }
    

    gpTime_start( &m_tProcess );

	ret = m_cb->Process( sim, iFrame, &bim );

    gpTime_stop( &m_tProcess );
    
   
    
    
    gpTime_start( &m_gTime );
   m_aim = imageA_set_color( sim, bim, 255, 0x00FFFF, m_aim );
    
   // m_aim = imageA_set_color( sim, m_cb->m_mim, 255, 0x00FFFF, m_aim );


//	m_aim = image3_from( bim, m_aim );
 //   m_aim = image3_from( m_cb->m_mim, m_aim );

    
    m_fb->Set( m_aim );
    gpTime_stop( &m_gTime );
    
    
    
	return( 1 );
}




char * CHomage::GetProcessLog()
{

    
	sprintf( m_processLog, "%d  %d  G:%s", gpTime_mpf( &m_gTime), gpTime_mpf( &m_tProcess),
            m_cb->GetProcessLog() );
    
	return( m_processLog );
}

