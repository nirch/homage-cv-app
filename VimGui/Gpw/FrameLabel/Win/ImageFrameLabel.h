// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _IMAGE_FRAME_LABEL_
#define _IMAGE_FRAME_LABEL_

#include <windows.h>
#include "UVec/Vec2d.h"
#include "Ubox/Box2d.h"

#include "../FrameLabel.h"


#define TEXT_LEN 512

class CImageFrameLabel : public CFrameLabel
{
public:
		CImageFrameLabel();

		virtual ~CImageFrameLabel();


		int InitFile( char *file, int fx, int fy );

		int  Init( HBITMAP hBitmap, int fx, int fy );

		int  InitPng( HINSTANCE hInstance,  int ID, int fx, int fy );

		int  Init( HINSTANCE hInstance, int ID, int fx, int fy );


		void Draw( HDC hdc );
		int HandleLButtonDown( int x, int y );
		int HandleLButtonUp(u_int nFlags, int x, int y );

public:


private:
	struct image_type	*m_im;
	
	
	HDC m_hdc;
	HBITMAP m_hBitmap;


	int	m_width;
	int	m_height;

	int	m_wx;
	int	m_wy;
};




#endif