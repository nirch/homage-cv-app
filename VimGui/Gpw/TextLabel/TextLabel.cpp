/*************************
 ***   TextLabel.cpp   ***
 *************************/
#include <string.h>
#include "Utime/GpTime.h"
#include "TextLabel.h"



CTextLabel::CTextLabel( void *view )
{
	m_show = 0;
}

CTextLabel::CTextLabel()
{
	m_show = 0;
}

CTextLabel::~CTextLabel()
{

}



int  CTextLabel::Init( char *file )
{
	//m_drag = drag;

	return( 1 );

}







void CTextLabel::SetFrameSize( int width, int height )
{
	m_width = width;
	m_height = height;

}

void CTextLabel::SetBackgroundColor( int color, float transparent )
{
	m_bcolor = color;
	m_transparent = transparent;
}


void CTextLabel::SetColor( int color )
{
	m_color = color;
}


int CTextLabel::Set( char *text )
{
	m_time = vTime();

	strncpy( m_text, text, TEXT_LEN );

	SplitText( 64 );

	SetVisible( 1 );

	return( 1 );
}


void CTextLabel::SetVisible( int flag )
{
    m_show = flag;
}


int CTextLabel::Open( char *text, int color, int bcolor, int transparency, int timeout, int motion )
{

	m_time = vTime();

	strncpy( m_text, text, TEXT_LEN );

	m_textLen = (int)strlen( m_text );

	m_color = color;

	m_bcolor = bcolor;

//	m_transparency = transparency/100.0;
    m_transparent = transparency/100.0;

	m_timeout = timeout;

	m_motion = motion;

	m_iText = 0;

	SplitText( 64 );

	SetVisible( 1 );

	return( 1 );
}



int CTextLabel::Open( char *str )
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

	m_time = vTime();

	m_textLen = (int)strlen( m_text );

	m_transparent = transparency/100.0;

	m_iText = 0;

	SplitText( 64 );

	return( 1 );
}


void CTextLabel::SplitText( int n )
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




void CTextLabel::Close( )
{
	SetVisible( 0 );
	m_time = -1;

}


void CTextLabel::Close( char *text )
{
	if( m_time < 0 )	return;

	if( strncmp( m_text, text, strlen(text)) == 0 )
		m_time = -1;

}


int CTextLabel::IsActive( )
{

	if( m_time <= 0  )	return( -1 );

	if( m_timeout > 0 && vTime() > m_timeout*1000 )	return( -1 );


	return( 1 );
}





int CTextLabel::HandleLButtonUp(u_int nFlags, int x, int y )
{


	return( -1 );
}


