/******************************
 ***   TextFrameLabel.cpp   ***
 ******************************/

#include "Utime/GpTime.h"
#include "TextFrameLabel.h"

#include "Wutil/UWdib/Cdib.h"

#include "ImageType/ImageType.h"


#include "TextBoxX.h"



CTextFrameLabel::CTextFrameLabel()
{
	m_rgbBackground = RGB(223, 223, 223);
	m_hdc = NULL;
	m_hBitmap = NULL;


	Create_xIconDib();

}



CTextFrameLabel::~CTextFrameLabel()
{

}



int  CTextFrameLabel::Init( char *file )
{
	//m_drag = drag;

	return( 1 );

}


int CTextFrameLabel::CreateHdc( HDC hdc )
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







void CTextFrameLabel::SetFrameSize( int width, int height )
{
	m_width = width;
	m_height = height;

	if( m_hdc != NULL )
		CreateHdc( NULL );
}

void CTextFrameLabel::SetTransparentColor( float t, int color )
{
	m_bcolor = color;
	m_transparent = t;

	if( m_hdc != NULL ){
		HBRUSH bcolor =  CreateSolidBrush( m_color );

		FillRect( m_hdc, &m_rect,  bcolor );
	}
}

void CTextFrameLabel::Draw( HDC hdc )
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

	m_xIconDib->Draw( hdc,  m_x0+ m_xBox.x0, m_x0+ m_xBox.y0, 0, 0 );


	DrawTextBox( hdc );

}







int CTextFrameLabel::OpenTextBox( char *text, int color, int bcolor, int transparency, int timeout, int motion )
{

	m_time = gpTime_base();

	strncpy( m_text, text, TEXT_LEN );

	m_textLen = (int)strlen( m_text );

	m_color = color;

	m_bcolor = bcolor;

	m_transparency = transparency/100.0;

	m_timeout = timeout;

	m_motion = motion;

	m_iText = 0;

	SplitText( 64 );

	SetShow( 1 );

	return( 1 );

}

int CTextFrameLabel::OpenTextBox( char *str )
{
	int	transparency;

	if( gpStr_strtok_string( str, "|", m_text ) < 0 )
		return( -1 );

	if( gpStr_strtok_hexa( NULL, "|", &m_color ) < 0 )
		return( -1 );

	if( gpStr_strtok_hexa( NULL, "|", &m_bcolor ) < 0 )
		return( -1 );


	if( gpStr_strtok_int( NULL, "|", &transparency ) < 0 )
		return( -1 );

	if( gpStr_strtok_int( NULL, "|", &m_timeout ) < 0 )
		return( -1 );

	m_motion = 0;
	gpStr_strtok_int( NULL, "|", &m_motion );

	m_time = gpTime_base();

	m_textLen = (int)strlen( m_text );

	m_transparency = transparency/100.0;

	m_iText = 0;

	SplitText( 64 );

	return( 1 );
}


void CTextFrameLabel::SplitText( int n )
{
	int	i,	i0,	k,	size;

	for( i = 0 ; i < 4 ; i++ )
		m_aText[i][0] = 0;

	m_nText = 0;
	size = strlen( m_text );

	for( i = 0, i0 = 0 ; i < 2 ; i++ ){
		if( i0 + n > size ){
			strncpy( &m_aText[m_nText][0], &m_text[i0], size - i0 );
			m_aText[m_nText][size - i0] = 0;
			m_nText++;
			return;
		}

		for( k = i0+n ; k > i0 ; k-- )
			if( m_text[k] == ' ' )	break;
		if( k == i0 )	return;

		strncpy( &m_aText[m_nText][0], &m_text[i0], k - i0 );
		m_aText[m_nText][k - i0] = 0;
		m_nText++;

		i0 = k + 1;
	}
}




void CTextFrameLabel::CloseTextBox( )
{
	m_time = -1;

}


void CTextFrameLabel::CloseTextBox( char *text )
{
	if( m_time < 0 )	return;

	if( strncmp( m_text, text, strlen(text)) == 0 )
		m_time = -1;

}

void
CTextFrameLabel::DrawTextBox ( HDC hdc )
{
	m_textBoxActive = 0;
	if( IsTextBoxActive( ) < 0 )
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
	m_textBoxActive = 1;
}



int CTextFrameLabel::IsTextBoxActive( )
{

	if( m_time <= 0  )	return( -1 );

	if( m_timeout > 0 && gpTime( m_time ) > m_timeout*1000 )	return( -1 );


	return( 1 );
}








int CTextFrameLabel::Create_xIconDib()
{
	image_type *im;

	im = image_create( XICON_SIZE, XICON_SIZE, 4, 1, (u_char *)xIcon );


	m_xIconDib = new CDib( im->height, im->width, 32 );
	m_xIconDib->CreateSection();

	m_xIconDib->data_image( im, 0 );

	image_destroy( im, 0 );


	return( 1 );
}



int CTextFrameLabel::HandleLButtonUp(u_int nFlags, int x, int y )
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




