// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TEXT_FRAME_LABEL_WIN_
#define _TEXT_FRAME_LABEL_WIN_

#include <windows.h>
#include "UVec/Vec2d.h"
#include "Ubox/Box2d.h"

#include "TextLabel.h"


#define TEXT_LEN 512

class CTextLabelWin: public CTextLabel
{
public:
		CTextLabelWin();

		virtual ~CTextLabelWin();


		void SetFrameSize( int width, int height );
		void SetBackgroundColor( int color, float transparent );

		void Draw( HDC hdc );

		int HandleLButtonUp(u_int nFlags, int x, int y );



public:

	virtual int Init( char *file );



private:
	int CreateHdc( HDC hdc );


	void	DrawTextBox ( HDC hdc );



	HDC m_hdc;
	RECT	m_rect;

	HBITMAP m_hBitmap;


	int	Create_xIconDib();

	class CDib *m_xIconDib;
	box2i_type	m_xBox;
 
};




#endif