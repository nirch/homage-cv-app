/**********************
 ***   GpwImage.h   ***
 **********************/

#ifndef	_GPW_IMAGE_
#define	_GPW_IMAGE_




#include		"Uigp/igp.h"
#include        "Uvec/Vec2d.h"
#include        "Ubox/Box2d.h"

#include "ImageType/ImageType.h"

#include "Gpw.h"



class CGpwImage : public CGpw
{
public:
	CGpwImage();

	CGpwImage( image_type *im );

	CGpwImage( int width, int height );


	virtual	~CGpwImage();



	void PointW( int wx, int wy, int color );


	void LineW( int wx0, int wy0, int wx1, int wy1, int color, int width = 0 );


	void String( float x, float y, char *s, int len, int color );


	void RectangleW( int wx0, int wy0, int wx1, int wy1, int color );



private:

	image_type *m_im;



private:

};



#endif
