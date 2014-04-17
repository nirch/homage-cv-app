/***************************
 ***   FrameBuffer.cpp   ***
 ***************************/

#include "ImageType/ImageType.h"
#include "FrameBuffer.h"



CFrameBuffer::CFrameBuffer()
{
	m_image = NULL;
}



CFrameBuffer::~CFrameBuffer()
{
	if( m_image != NULL ){
		image_destroy( m_image, 1 );
		m_image = NULL;
	}
}







image_type *  CFrameBuffer::Create( int width, int height )
{

	m_image = image_realloc( m_image, width, height, 4, IMAGE_TYPE_U8, 1 );

	return( m_image );

}




void CFrameBuffer::Fill()
{
}



void CFrameBuffer::Set( image_type *sim )
{
    Create( sim->width, sim->height );
                   
	if( sim->depth != 4 ){
		m_image = image4_from( sim, m_image );
        Fill();
		return;
	}

	m_image = image_make_copy( sim, m_image );
    
    Fill();
}
