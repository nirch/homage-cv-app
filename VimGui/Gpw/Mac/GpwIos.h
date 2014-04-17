/********************
 ***   GpwX11.h   ***
 ********************/

#ifndef	_GPW_MAC_
#define	_GPW_MAC_

#import <UIKit/UIKit.h>



#include "../Gpw.h"



class CGpwIos : public CGpw
{
public:
	CGpwIos();
	virtual	~CGpwIos();

    void  AddSubview(  UIView *view );

    void  Start();
    
    void  End();


	void PointW( int wx, int wy, int color );

	

	void LineW( int wx0, int wy0, int wx1, int wy1, int color, int width = 0 );


	void String( float x, float y, char *s, int len, int color );

	void RectangleW( int wx0, int wy0, int wx1, int wy1, int color );


    
    UIColor *Color( int color );

public:

    UIImageView *m_view;
    

private:

    CGContextRef m_context;
};





#endif
