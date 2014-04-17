/********************
 ***   GpwX11.h   ***
 ********************/

#ifndef	_GPW_MAC_
#define	_GPW_MAC_

#import <UIKit/UIKit.h>



#include "../Gpw.h"



class CGpwMac : public CGpw
{
public:
	CGpwMac( void *w );
	virtual	~CGpwMac();


//	void SetHdc( HDC dc )	{ m_dc = dc; }


	void PointW( int wx, int wy, int color );

	

	void LineW( int wx0, int wy0, int wx1, int wy1, int color, int width = 0 );


	void String( float x, float y, char *s, int len, int color );

	void RectangleW( int wx0, int wy0, int wx1, int wy1, int color );

    void Clean();

private:

    //UIImageView *drawing;
    UIView *drawOn;

private:

    CGContextRef context;
};





#endif
