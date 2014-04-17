/***********************
 ***   GpwTool.cpp   ***
 ***********************/
#include	<math.h>


#include	"Uigp/igp.h"



#include "GpwWin.h"



CGpwWin::CGpwWin()
{
}


CGpwWin::~CGpwWin()
{

}



void CGpwWin::Point( float x, float y, int color )
{
	int	wx,	wy;

	xy2win(  x, y, &wx, &wy );

	SetPixel( m_dc, wx, wy, (COLORREF)color );
}


void CGpwWin::PointW( int wx, int wy, int color )
{
	SetPixel( m_dc, wx, wy, (COLORREF)color );
}



void CGpwWin::LineW( int wx0, int wy0, int wx1, int wy1, int color, int width )
{
	HPEN	pen,	open;


	//if( im != NULL ){
	//	image4_draw_line( im, wx0, wy0 ,wx1, wy1, color );
	//	return;
	//}



	pen = CreatePen(PS_SOLID, width,(COLORREF)color );

	open = (HPEN)SelectObject( m_dc, pen );


	MoveToEx( m_dc, wx0, wy0, NULL );
	LineTo( m_dc, wx1, wy1 );

	SelectObject( m_dc, open );

	DeleteObject( pen );
}





void CGpwWin::String( float x, float y,
					 char *s, int len, int color )
{
	int	wx,	wy;

	SetTextColor( m_dc, color );
	SetBkMode( m_dc, TRANSPARENT );


	xy2win(  x, y, &wx, &wy );

	TextOut( m_dc, wx, wy, (LPCSTR)s, len );
}




//void CGpwWin::Rectangle(
//		float x0, float y0, float x1, float y1, int color )
//{
//HBRUSH	brush;
//RECT rect;
//int	wx0,	wy0,	wx1,	wy1;
//
//	brush = CreateSolidBrush(color);
//
//	xy2win(  x0, y0, &wx0, &wy0 );
//	xy2win(  x1, y1, &wx1, &wy1 );
//	rect.left = wx0;
//	rect.right = wx1;
//	rect.top = wy0;
//	rect.bottom = wy1;
//
//	FillRect( m_dc, &rect, brush );
//
//	DeleteObject( brush );
//}



void CGpwWin::RectangleW( int wx0, int wy0, int wx1, int wy1, int color )
{
	RECT rect;
	HBRUSH	brush = CreateSolidBrush(color);

	rect.left = wx0;
	rect.right = wx1;
	rect.top = wy0;
	rect.bottom = wy1;

	FillRect( m_dc, &rect, brush );

	DeleteObject( brush );
}






void CGpwWin::Rectangle_e( float x0, float y0, float x1, float y1, int color )
{
HPEN	pen,	open;
int	wx0,	wy0,	wx1,	wy1;

	xy2win(  x0, y0, &wx0, &wy0 );
	xy2win(  x1, y1, &wx1, &wy1 );

	pen = CreatePen(PS_SOLID, 1,(COLORREF)color );

	open = (HPEN)SelectObject( m_dc, pen );


	MoveToEx( m_dc, wx0, wy0, NULL );
	LineTo( m_dc, wx0, wy1 );
	LineTo( m_dc, wx1, wy1 );
	LineTo( m_dc, wx1, wy0 );
	LineTo( m_dc, wx0, wy0 );


	SelectObject( m_dc, open );

	DeleteObject( pen );
}











#ifdef _AA_
void CGpwWin::Move( float x, float y, int color, int flag )
{
HPEN	pen,	open;
int	wx,	wy;


	xy2win(  x, y, &wx, &wy );

	if(  wx == wx && wy == wy )	return;


	if( flag == GPW_DRAW ){
		pen = CreatePen(PS_SOLID, 1,(COLORREF)color );
		open = (HPEN)SelectObject( dc, pen );

		MoveToEx( dc, wx, wy, NULL );
		LineTo( dc, wx, wy );

		SelectObject( dc, open );
		DeleteObject( pen );

	}

	wx = wx;
	wy = wy;
}
#endif
