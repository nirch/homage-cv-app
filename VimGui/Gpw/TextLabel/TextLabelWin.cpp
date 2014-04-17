/******************************
 ***   TextFrameLabel.cpp   ***
 ******************************/

#include "Utime/GpTime.h"
#include "TextLabelWin.h"

#include "Wutil/UWdib/Cdib.h"

#include "ImageType/ImageType.h"


#include "TextBoxX.h"



CTextLabelWin::CTextLabelWin()
{

	m_hdc = NULL;
	m_hBitmap = NULL;


	Create_xIconDib();

}



CTextLabelWin::~CTextLabelWin()
{

}



int  CTextLabelWin::Init( char *file )
{
	return( 1 );

}

void CTextLabelWin::SetFrameSize( int width, int height )
{
	m_width = width;
	m_height = height;

	if( m_hdc != NULL )
		CreateHdc( NULL );
}

void CTextLabelWin::SetBackgroundColor( int color, float transparent )
{
	m_bcolor = color;
	m_transparent = transparent;

	if( m_hdc != NULL ){
		HBRUSH bcolor =  CreateSolidBrush( m_color );

		FillRect( m_hdc, &m_rect,  bcolor );
	}
}


int CTextLabelWin::CreateHdc( HDC hdc )
{

	if( m_hdc == NULL )
		m_hdc =  ::CreateCompatibleDC( hdc );

	if( m_hBitmap != NULL )
		::DeleteObject(m_hBitmap);

	m_hBitmap = ::CreateBitmap( m_width, m_height, 1, 32, NULL );

	HGDIOBJ hOld = ::SelectObject( m_hdc, m_hBitmap );



	m_rect.top = 0;
	m_rect.bottom = m_height;
	m_rect.left = 0;
	m_rect.right = m_width;


	HBRUSH bcolor =  CreateSolidBrush( m_bcolor );

	FillRect( m_hdc, &m_rect,  bcolor );


	m_xBox.x0 = m_width - XICON_SIZE-5;
	m_xBox.y0 = m_height -XICON_SIZE -5;

	m_xBox.x1 = m_xBox.x0 + XICON_SIZE;
	m_xBox.y1 = m_xBox.y0 + XICON_SIZE;

	return( 1 );
}









void CTextLabelWin::Draw( HDC hdc )
{
	if( m_show == 0 )
		return;

	if( m_hdc == NULL )
		CreateHdc( hdc );

	BLENDFUNCTION ftn;
	ftn.BlendOp = AC_SRC_OVER;
	ftn.BlendFlags = 0;
	ftn.SourceConstantAlpha = (BYTE)(m_transparent *255);
	ftn.AlphaFormat = 0;

	AlphaBlend( hdc, m_x0, m_y0, m_width, m_height, 
				m_hdc, 0, 0, m_width, m_height,
				ftn );

//	m_xIconDib->Draw( hdc,  m_x0+ m_xBox.x0, m_y0+ m_xBox.y0, 0, 0 );
	m_xIconDib->Draw( hdc, m_y0+ m_xBox.y0,  m_x0+ m_xBox.x0, 0, 0 );


	DrawTextBox( hdc );

}






void
CTextLabelWin::DrawTextBox ( HDC hdc )
{

	if( IsActive( ) < 0 )
		return;



	SetTextColor( hdc, m_color );
	SetBkMode( hdc, TRANSPARENT );


	//WCHAR    wtext[TEXT_LEN]; 
	//MultiByteToWideChar( 0,0, m_text, m_textLen, wtext, m_textLen);


	if( m_iText * 10 > m_width )	m_iText = 0;
	int	x;
	x = m_x0 + 10 + m_iText*m_motion*10;
	m_iText++;


	int	i;
	for( i = 0 ; i < m_nText ; i++ )
		TextOut( hdc, x, m_y0 + 5+i*20, (LPCSTR)&m_aText[i][0], strlen(m_aText[i]) );

}












int CTextLabelWin::Create_xIconDib()
{
	image_type *im;

	im = image_create( XICON_SIZE, XICON_SIZE, 4, 1, (u_char *)xIcon );


	m_xIconDib = new CDib( im->height, im->width, 32 );
	m_xIconDib->CreateSection();

	m_xIconDib->data_image( im, 0 );

	image_destroy( im, 0 );


	return( 1 );
}



int CTextLabelWin::HandleLButtonUp(u_int nFlags, int x, int y )
{

	x -= m_x0;
	y -= m_y0;


	if( m_show == 1 ){
		if( BOX2D_IS_IN( m_xBox, x, y ) ){
			SetShow( 0 );
			return( 1 );
		}
	}

	return( -1 );
}




