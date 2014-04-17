/********************
 ***   GpwX11.h   ***
 ********************/

#ifndef	_GPW_X11_
#define	_GPW_X11_


#include        <X11/Xlib.h>
#include	<X11/Xutil.h>


//#include		"Uigp/igp.h"
//#include        "Uvec/Vec2d.h"
//#include        "Ubox/Box2d.h"


#include "../Gpw.h"



class CGpwX11 : public CGpw
{
public:
	CGpwX11();
	virtual	~CGpwX11();


	void SetHdc( HDC dc )	{ m_dc = dc; }


	void PointW( int wx, int wy, int color );

	

	void LineW( int wx0, int wy0, int wx1, int wy1, int color, int width = 0 );


	void String( float x, float y, char *s, int len, int color );

	void RectangleW( int wx0, int wy0, int wx1, int wy1, int color );


private:

	Widget	m_w;
	Display	*m_dis;
	GC	m_gc;



private:

};





#endif
