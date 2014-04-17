/*****************************
 ***   FrameBufferPC.cpp   ***
 *****************************/

#include "Utime/GpTime.h"
#include "FrameBufferWin.h"



CFrameBufferWin::CFrameBufferWin()
{
	m_pDib = NULL;

	m_hdc = NULL;
	m_hBitmap = NULL;

	m_bcolor = 0x4e145c;

	m_transparency = 0.5;

	m_boxDib = NULL;
	m_boxIm = NULL;

	m_textBoxActive = 0;

	int i;
	for( i = 0 ; i < 4 ; i++ )
		m_aText[i][0] = 0;

	m_nText = 0;

	m_time = 0;

	m_timeout = 0;

	m_motion = 0;

}



CFrameBufferWin::~CFrameBufferWin()
{
	if( m_pDib != NULL ){
		delete m_pDib;
		m_pDib = NULL;
	}
}


void
CFrameBufferWin::Draw ( HDC hdc, int x, int y )
{
int	row,	col;

	if( m_pDib == NULL )	return;

	if( m_hdc == NULL ){
		if( CreateHdc( hdc ) < 0 )
			return;
	}


	m_pDib->GetDimensions( &row, &col );
	m_pDib->Draw( m_hdc, 0, 0, row, col );

	DrawTextBox();


	::BitBlt( hdc, x, y, col, row, m_hdc, 0, 0, SRCCOPY );

}





image_type *	CFrameBufferWin::Create( int width, int height )
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


	if( m_hdc != NULL )
		CreateHdc( m_hdc );


	return( m_image );
}




void CFrameBufferWin::Fill()
{
//	if( m_hdc != NULL )
//		image4_transparent( m_image, &m_textBox, m_transparency, m_bcolor, m_image );
}


void CFrameBufferWin::SetBitPerPixel()
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



int CFrameBufferWin::CreateHdc( HDC hdc )
{
	
	if( m_image == NULL )
		return( -1 );

	if( m_hdc == NULL )
		m_hdc =  ::CreateCompatibleDC( hdc );

	if( m_hBitmap != NULL )
		::DeleteObject(m_hBitmap);

	m_hBitmap = ::CreateBitmap( m_image->width, m_image->height, 1, 32, NULL );

	HGDIOBJ hOld = ::SelectObject( m_hdc, m_hBitmap );

	//	::BitBlt( hdc, c0, r0, column, row, hdcSrc, 0, 0, SRCCOPY );
//	::SelectObject( hdcSrc, hOld);


	m_textBox.x0 = 0;
	m_textBox.x1 = m_image->width;

	m_textBox.y0 = m_image->height-50;
	m_textBox.y1 = m_image->height;


	return( 1 );
}


int CFrameBufferWin::OpenTextBox( char *text, int color, int bcolor, int transparency, int timeout, int motion )
{
	if( m_image == NULL )	return( -1 );

	m_time = gp_base_time();

	strncpy( m_text, text, TEXT_LEN );

	m_textLen = (int)strlen( m_text );

	m_color = color;

	m_bcolor = bcolor;

	m_transparency = transparency/100.0;

	m_timeout = timeout;

	m_motion = motion;

	m_iText = 0;

	CreateBoxDib( m_image->width, 50 );

	SplitText( 64 );

	return( 1 );

}

int CFrameBufferWin::OpenTextBox( char *str )
{
int	transparency;

	if( m_image == NULL )	return(-1);

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

	m_time = gp_base_time();

	m_textLen = (int)strlen( m_text );

	m_transparency = transparency/100.0;

	m_iText = 0;


	CreateBoxDib( m_image->width, 50 );

	SplitText( 64 );

	return( 1 );
}


void CFrameBufferWin::SplitText( int n )
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

	


void CFrameBufferWin::CloseTextBox( )
{
	m_time = -1;

}


void CFrameBufferWin::CloseTextBox( char *text )
{
	if( m_time < 0 )	return;

	if( strncmp( m_text, text, strlen(text)) == 0 )
		m_time = -1;

}

void
CFrameBufferWin::DrawTextBox ()
{
	m_textBoxActive = 0;
	if( IsTextBoxActive( ) < 0 )
		return;


	SeteBoxDib();

	int	row,	col;
	m_boxDib->GetDimensions( &row, &col );
	m_boxDib->Draw( m_hdc, 0, 0, row, col );


	SetTextColor( m_hdc, m_color );
	SetBkMode( m_hdc, TRANSPARENT );



	WCHAR    wtext[TEXT_LEN]; 
	MultiByteToWideChar( 0,0, m_text, m_textLen, wtext, m_textLen);

//	TextOut( m_hdc, 10, 20, wtext, m_textLen );
//	TextOut( m_hdc, 10, 5, m_text, m_textLen );

	if( m_iText * 10 > m_textBox.x1 )	m_iText = 0;
	int	x;
	x = 10 + m_iText*m_motion*10;
	m_iText++;


	int	i;
	for( i = 0 ; i < m_nText ; i++ )
		TextOut( m_hdc, x, 5+i*20, &m_aText[i][0], strlen(m_aText[i]) );
	m_textBoxActive = 1;
}



int CFrameBufferWin::IsTextBoxActive( )
{

	if( m_time <= 0  )	return( -1 );

	if( m_timeout > 0 && gpTime( m_time ) > m_timeout*1000 )	return( -1 );


	return( 1 );
}


void
CFrameBufferWin::CreateBoxDib( int width, int height )
{
	int depth = 4;
	int bpp = 32;

	if( m_boxDib == NULL ){
		m_boxDib = new CDib( height, width, bpp);
		m_boxDib->CreateSection();

		m_boxIm = image_create( height, width, depth, 1, m_boxDib->m_lpImage );

		return;
	}


	if( m_boxDib->ReSize( height, width, bpp, 1 ) == FALSE )	return;



	m_boxIm->data = m_boxDib->m_lpImage;
	m_boxIm->row = height;
	m_boxIm->column = width;

	m_boxIm->column_byte = m_image->column*m_image->depth;
}


#include "TextBoxX.h"


static void	image4_transparent_box( image_type *sim, box2i *b, float trans, int backcolor, image_type *im );

void
CFrameBufferWin::SeteBoxDib()
{

	image4_transparent_box( m_image, &m_textBox, m_transparency, m_bcolor, m_boxIm );


	int	i,	j;
	int	x0;
	int	y0;
	x0 = m_boxIm->width - XICON_SIZE-5;
	y0 = 5;//m_boxIm->height -XICON_SIZE-10;

	u_int *sp = xIcon;
	for( i = y0 ; i < y0+XICON_SIZE ; i++ ){
		u_int	*tp = (u_int *)IMAGE_PIXEL( m_boxIm, i, x0 );
		for( j = x0 ; j < x0+XICON_SIZE ; j++ )
			*tp++ = *sp++;
	}

}


int CFrameBufferWin::HandleLButtonUp(u_int nFlags, int x, int y )
{
int	x0,	y0;



	if( m_textBoxActive == 1 ){

		x0 = m_boxIm->width - XICON_SIZE-5;
		y0 = m_boxIm->height -XICON_SIZE-5;

		if( x > x0+1 && x < x0 + XICON_SIZE -2   && y > y0+1 && y < y0 + XICON_SIZE -2 ){
			CloseTextBox();
			return( 1 );
		}
	}


	return( -1 );
}




static void
image4_transparent_box( image_type *sim, box2i *b, float trans, int backcolor, image_type *im )
{
	u_int	*sp,	*tp;
	int	R,	G,	B,	Rb,	Gb,	Bb;
	int	i,	j;



	Rb = IMAGE4_RED(backcolor); 
	Gb = IMAGE4_GREEN(backcolor);
	Bb = IMAGE4_BLUE(backcolor);


	for( i = b->y0 ; i < b->y1 ; i++ ){

		sp = (u_int *)IMAGE_PIXEL( sim, i, b->x0 );
		tp = (u_int *)IMAGE_PIXEL( im, i-b->y0, b->x0 );

		for( j = b->x0 ; j < b->x1 ; j++ ){			

			R = IMAGE4_RED(*sp); 
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);
			sp++;



			R = R + (Rb - R)*trans;
			G = G + (Gb - G)*trans;
			B = B + (Bb - B)*trans;

			*tp = IMAGE4_RGB(R,G,B);
			tp++;

		}
	}
}