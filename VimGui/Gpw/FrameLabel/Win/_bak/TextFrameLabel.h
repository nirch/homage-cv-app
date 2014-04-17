// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TEXT_FRAME_LABEL_
#define _TEXT_FRAME_LABEL_

#include <windows.h>
#include "UVec/Vec2d.h"
#include "Ubox/Box2d.h"

#include "../FrameLabel.h"


#define TEXT_LEN 512

class CTextFrameLabel : public CFrameLabel
{
public:
		CTextFrameLabel();

		virtual ~CTextFrameLabel();


		void SetFrameSize( int width, int height );
		void SetTransparentColor( float t, int color );

		void Draw( HDC hdc );

		int HandleLButtonUp(u_int nFlags, int x, int y );



public:

	virtual int Init( char *file );

	virtual int Draw()	{ return(0); }

	int	OpenTextBox( char *text, int color, int bcolor, int transparency, int timeout, int motion = 0 );

	void	 CloseTextBox();

private:
	int CreateHdc( HDC hdc );


	// text box

	int	OpenTextBox( char *str );


	void	CloseTextBox( char *text );

	void	SplitText( int n );

	int		IsTextBoxActive();

	void	DrawTextBox ( HDC hdc );



	HDC m_hdc;
	RECT	m_rect;

	HBITMAP m_hBitmap;

	COLORREF m_rgbBackground;

	int	m_width;
	int	m_height;

	int m_bcolor;
	float m_transparent;



// text
	char	m_text[TEXT_LEN];
	int		m_color;

	int		m_timeout;
	int		m_motion;

	char	m_aText[4][256];
	int		m_nText;

	float		m_transparency;

	int		m_textLen;

	int		m_time;

	int m_textBoxActive;


	int	m_iText;


	int	Create_xIconDib();

	class CDib *m_xIconDib;
	box2i_type	m_xBox;
 
};




#endif