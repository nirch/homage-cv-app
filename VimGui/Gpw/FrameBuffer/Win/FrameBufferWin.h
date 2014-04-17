// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAMEBUFFER_WIN_
#define _FRAMEBUFFER_WIN_


//#if defined _DEBUG || defined DEBUG
//#pragma comment( lib, "VimRendererD.lib" )
//#else
//#pragma comment( lib, "VimRenderer.lib" )
//#endif


#include <windows.h>
#include <wininet.h>

#include	"Wutil/UWdib/Cdib.h"



#include	"../FrameBuffer.h"


#define TEXT_LEN 512



class CFrameBufferWin  : public CFrameBuffer
{
public:
		CFrameBufferWin();
		virtual ~CFrameBufferWin();


public:

		HDC m_hdc;
		HBITMAP m_hBitmap;

		CDib		*m_pDib;

		CDib		*m_boxDib;
		image_type	*m_boxIm;

		void	Draw ( HDC hdc, int x, int y );

		virtual void Draw( struct gpw_type *gpw ) {};


		// frmae buffer 
		struct image_type *	Create( int width, int height );

		void	Fill();

		int CreateHdc( HDC hdc );


		void SetBitPerPixel();



		// text box
		int	OpenTextBox( char *text, int color, int bcolor, int transparency, int timeout, int motion = 0 );

		int	OpenTextBox( char *str );

		void	 CloseTextBox();

		void	CloseTextBox( char *text );

		void	SplitText( int n );

		int	 IsTextBoxActive();

		void	CreateBoxDib( int width, int height );

		void	SeteBoxDib();

		void	DrawTextBox ();

		int HandleLButtonUp(u_int nFlags, int x, int y );


public:
	char	m_text[TEXT_LEN];
	int		m_color;
	int		m_bcolor;
	int		m_timeout;
	int		m_motion;

	char	m_aText[4][256];
	int		m_nText;

	float		m_transparency;

	int		m_textLen;

	int		m_time;
	box2i	m_textBox;

	int m_textBoxActive;


	int	m_iText;

};




#endif