/*********************************
 ***   CVideoAcquisition.cpp   ***
 ********************************/

#include	<string.h>



#include	"VideoAcquisition.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "VideoCodec\VideoDecoder.h"
#include "Avi\Avi\AviDecoder\AviGetDecoder.h"



CImageAcquisition *CreateVideoAcquisition()
{
CImageAcquisition	*vc;

	vc = new CVideoAcquisition();

	return( vc );
}


CVideoAcquisition::CVideoAcquisition()
{
	m_decoder = NULL;

	
//	strcpy(m_acqName,"Movie");

	m_acq = 0;

	m_image = NULL;

	m_active = 0;

}


CVideoAcquisition::~CVideoAcquisition()
{
	Close();

	if( m_decoder != NULL ){
		delete m_decoder;
		m_decoder = NULL;
	}
}



int CVideoAcquisition::Open( char *name )
{

	m_decoder = GetAviDecoder();

	if( m_decoder->Open( name ) < 0 ){
		delete m_decoder;
		m_decoder = NULL;
		return( -1 );
	}

	 m_decoder->SetImageFormat( IMAGE_FORMAT_RGB );

	 m_width = m_decoder->GetWidth();
	 m_height = m_decoder->GetHeight();


	 SetActiveOff();

	return 1;
}

void CVideoAcquisition::Close()
{
	if (m_decoder != NULL){
		m_decoder->Close();
	}

	if( m_image != NULL ){
		image_destroy( m_image, 1 );
		m_image = NULL;
	}
}

 
int CVideoAcquisition::Get( image_type **im, int iFrame )
{

	if( m_decoder == NULL )	return( -1 );

	if( iFrame < 0 ){
		if( iFrame == -2 || m_active == 1 )
			m_iFrame++;
	}
	else	m_iFrame = iFrame;

	if( m_iFrame < 0 )	m_iFrame = 0;

	image_type *tim;

	if( m_decoder->ReadFrame( m_iFrame, &tim ) < 0 )
		return( -1 );


	//m_image = image3_to_BGR( tim, m_image );
	m_image = image_make_copy( tim, m_image );

	//if( m_iFrame == 195 )
	//	image_dump( m_image, "im", m_iFrame, NULL );

	
	*im = m_image;


	return( 1 );
}



