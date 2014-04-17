/********************************
 ***   ImageAcquisition.cpp   ***
 ********************************/
#include	<string.h>


#include	"ImageAcquisition.h"


#include "ImageType/ImageType.h"






CImageAcquisition::CImageAcquisition()
{
	m_active = 0;

	m_width = -1;
	m_height = -1;

	m_iFrame = -1;
    
    m_fx = m_fy = 0;

	m_image = NULL;
}


CImageAcquisition::~CImageAcquisition()
{
//	m_width = 320;
//	m_height = 240;
}

int CImageAcquisition::Open( char *name )
{
	m_width = 320;
	m_height = 240;

	m_image = image_create( m_height, m_width, 3, 1, NULL );

	image3_const( m_image, 255, 255, 255 );

	return( 1 );
}

void CImageAcquisition::Close()
{

}

int CImageAcquisition::Get( image_type **im, int forceActive )
{
	*im = m_image;

	return( 1 );
}


void CImageAcquisition::SetFrameSize( int width, int height )
{
	m_rWidth = width;
	m_rHeight = height;
    
    m_width = width;
    m_height = height;
}

void CImageAcquisition::SetFoucusPixel( int x, int y )
{
	m_fx = x;
	m_fy = y;
    
}