/********************
 ***   GpwWin.h   ***
 ********************/

#ifndef	_GPW_WIN_
#define	_GPW_WIN_


#include <windows.h>

#include		"Uigp/igp.h"
#include        "Uvec/Vec2d.h"
#include        "Ubox/Box2d.h"


#include "Gpw.h"



class CGpwWin : public CGpw
{
public:
	CGpwWin();
	virtual	~CGpwWin();


	void SetHdc( HDC dc )	{ m_dc = dc; }


	void PointW( int wx, int wy, int color );

	void Point( float x, float y, int color );


	void LineW( int wx0, int wy0, int wx1, int wy1, int color, int width = 0 );


	void String( float x, float y, char *s, int len, int color );

	void RectangleW( int wx0, int wy0, int wx1, int wy1, int color );


	void Rectangle_e( float x0, float y0, float x1, float y1, int color );


//	void Move( float x, float y, int color, int flag );

private:

	HDC	m_dc;



private:

};








#endif
