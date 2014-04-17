/*****************************
 ***   VimRendererPC.cpp   ***
 *****************************/

#include <windows.h>


#include	"Wutil/UWdib/Cdib.h"




#include "FrameBufferPC.h"



CFrameBufferPC::CFrameBufferPC()
{
	m_pDib = NULL;
}



CFrameBufferPC::~CFrameBufferPC()
{
	if( m_pDib != NULL ){
		delete m_pDib;
		m_pDib = NULL;
	}


	if( m_image != NULL ){
		image_destroy( m_image, 0 );
		m_image = NULL;
	}
}


void
CFrameBufferPC::Draw ( HDC hdc, int x, int y )
{
int	row,	col;

	if( m_pDib != NULL ){

		m_pDib->GetDimensions( &row, &col );
		m_pDib->Draw( hdc, x, y, row, col );
	}

}





image_type * CFrameBufferPC::Create( int width, int height )
{
int depth = 4;
int bpp = 32;

	if( m_pDib == NULL ){
		m_pDib = new CDib( height, width, bpp);
		m_pDib->CreateSection();

		m_image = image_create( height, width, depth, 1, m_pDib->m_lpImage );

		return( m_image );
	}


	if( m_pDib->ReSize( height, width, bpp, 1 ) == FALSE )
		return( m_image );



	m_image->data = m_pDib->m_lpImage;
	m_image->row = height;
	m_image->column = width;

	m_image->column_byte = m_image->column*m_image->depth;

	return( m_image );
}




void CFrameBufferPC::SetBitPerPixel()
{
	HDC hdc = GetDC(NULL);
	int iBitDepth = GetDeviceCaps(hdc, BITSPIXEL);
	ReleaseDC(NULL, hdc);

	// Set the media type.
	//mt.majortype = MEDIATYPE_Video;
	//switch (iBitDepth)
	//{
	//case 8:
	//	mt.subtype = MEDIASUBTYPE_RGB8;
	//	break;
	//case 16:
	//	mt.subtype = MEDIASUBTYPE_RGB555;
	//	break;
	//case 24:
	//	mt.subtype = MEDIASUBTYPE_RGB24;
	//	break;
	//case 32:
	//	mt.subtype = MEDIASUBTYPE_RGB32;
	//	break;
	//default:
	//	return E_FAIL;
	//}
	//hr = pGrabber->SetMediaType(&mt);

}
