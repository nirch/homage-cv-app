/***********************
 ***   GpwTool.cpp   ***
 ***********************/
#include    <malloc.h>
#include	<math.h>


#include	"Uigp/igp.h"



#include "GpwX11.h"



CGpwX11::CGpwX11( Widget w )
{

	m_w = w;
	m_dis = XtDisplay(w);

	m_gc = XCreateGC( m_dis,
			RootWindow(m_dis, DefaultScreen(m_dis)), 0, NULL );
}


CGpwX11::~CGpwX11()
{

}




void CGpwX11::PointW( int wx, int wy, int color )
{

	Window	win = Xgp_draw_window(m_w);
	Pixmap	pix = Xgp_draw_rpixmap(m_w);


	XSetForeground( m_dis, m_gc, color );

	if( win != NULL )
		XDrawPoint( m_dis, win, m_gc, wx, wy );
	if( pix != 0 )
		XDrawPoint( m_dis, pix, m_gc, wx, wy );
}



void CGpwX11::LineW( int wx0, int wy0, int wx1, int wy1, int color, int width )
{

	Window	win = Xgp_draw_window(m_w);
	Pixmap	pix = Xgp_draw_rpixmap(m_w);


	XSetForeground( m_dis, m_gc, color );

	if( win != NULL )
		XDrawLine( m_dis, win, m_gc, wx0, wy0, wx1, wy1 );
	if( pix != 0 )
		XDrawLine( m_dis, pix, m_gc, wx0, wy0, wx1, wy1 );

}





void CGpwX11::String( float x, float y,
					 char *s, int len, int color )
{
	//int	wx,	wy;

	//SetTextColor( m_dc, color );
	//SetBkMode( m_dc, TRANSPARENT );


	//xy2win(  x, y, &wx, &wy );

	//TextOut( m_dc, wx, wy, (LPCWSTR)s, len );
}





void CGpwX11::RectangleW( int wx0, int wy0, int wx1, int wy1, int color )
{
	Window  win = Xgp_draw_window(m_w);
	Pixmap  pix = Xgp_draw_rpixmap(m_w);



	XSetForeground( m_dis, m_gc, color );

	if( win != NULL )
		XFillRectangle( m_dis, win, m_gc,
		wx0, wy0,
		wx1 - wx0, wy1 - wy0 );

	if( pix != NULL )
		XFillRectangle( m_dis, pix, m_gc,
		wx0, wy0,
		wx1 - wx0, wy1 - wy0 );
}



