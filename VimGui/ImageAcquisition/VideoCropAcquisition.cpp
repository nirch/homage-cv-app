/*********************************
 ***   CVideoCropAcquisition.cpp   ***
 ********************************/

#include	<string.h>



#include	"VideoCropAcquisition.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "VideoCodec\VideoDecoder.h"
#include "Avi\Avi\AviDecoder\AviGetDecoder.h"



CImageAcquisition *CreateCropVideoAcquisition()
{
CImageAcquisition	*vc;

	vc = new CVideoCropAcquisition();

	return( vc );
}


CVideoCropAcquisition::CVideoCropAcquisition()
{
	m_decoder = NULL;

	
//	strcpy(m_acqName,"Movie");

	m_acq = 0;

	m_image = NULL;

	m_active = 0;

}


CVideoCropAcquisition::~CVideoCropAcquisition()
{
	Close();

	if( m_decoder != NULL ){
		delete m_decoder;
		m_decoder = NULL;
	}
}



int CVideoCropAcquisition::Open( char *name )
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

void CVideoCropAcquisition::Close()
{
	if (m_decoder != NULL){
		m_decoder->Close();
	}

	if( m_image != NULL ){
		image_destroy( m_image, 1 );
		m_image = NULL;
	}
}

 
int CVideoCropAcquisition::Get( image_type **im, int iFrame )
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

	int	x0 = 0.5*( tim->width - m_rWidth);
	int	y0 = 0.5*( tim->height - m_rHeight);

	m_image = image_crop( tim, x0, y0, m_rWidth, m_rHeight, m_image );
//	m_image = image_make_copy( tim, m_image );



	
	*im = m_image;


	return( 1 );
}



