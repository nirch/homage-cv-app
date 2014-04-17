/************************
 *** ImageDecoder.cpp ***
 ***********************/
#include	<string.h>
	
#include	"ImageType/ImageType.h"

#include	"ImageDecoder.h"





CImageDecoder::CImageDecoder()
{
	m_image = NULL;

	m_vimatixData = NULL;
}


CImageDecoder::~CImageDecoder()
{
	if( m_image != NULL )
		image_destroy( m_image, 1 );


	if( m_vimatixData != NULL ){
		free( m_vimatixData );
		m_vimatixData = NULL;
	}
}


void CImageDecoder::SetVimatixData( char *data )
{
	if( data == NULL || data[0] == 0 )	return;

	if( m_vimatixData != NULL )
		free( m_vimatixData );

	m_vimatixData = strdup( data );
}

