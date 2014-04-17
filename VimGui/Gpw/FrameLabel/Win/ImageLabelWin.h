// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _IMAGE_LABEL_WIN_
#define _IMAGE_LABEL_WIN_

#include <windows.h>
#include "UVec/Vec2d.h"
#include "Ubox/Box2d.h"

#include "../ImageLabel.h"



class CImageLabelWin : public CImageLabel
{
public:
		CImageLabelWin();

		virtual ~CImageLabelWin();


		int InitFile( char *file, int fx, int fy );

		int  Init( HBITMAP hBitmap, int fx, int fy );

		int  InitPng( HINSTANCE hInstance,  int ID, int fx, int fy );

		int  Init( HINSTANCE hInstance, int ID, int fx, int fy );


		int Draw( void *hdc, int wx, int wy );

		int IsTouchBegin( void *view, int x, int y );
		int IsTouchMove( void *view, int x, int y );
		int IsTouchEnd( void *view, int x, int y );

public:


private:
	struct image_type	*m_im;
	
	
	HDC m_hdc;
	HBITMAP m_hBitmap;


	int	m_width;
	int	m_height;

	int m_hint;
};




#endif