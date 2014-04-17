/***********************
 ***   GpwTool.cpp   ***
 ***********************/
#include	<math.h>


#include	"Uigp/igp.h"

#include "ImageMark/ImageMark.h"





#include "GpwImage.h"



CGpwImage::CGpwImage()
{
	m_im = NULL;
}


CGpwImage::CGpwImage( image_type *im )
{
	m_im = im;

	SetWin( 0, 0, m_im->width, m_im->height );

	SetXy( 0, 0, m_im->width, m_im->height, AXIS_WH_XY );
}


CGpwImage::CGpwImage( int width, int height )
{
	m_im = image_alloc( width, height, 4, IMAGE_TYPE_U8, 1 );

	SetWin( 0, 0, m_im->width, m_im->height );

	SetXy( 0, 0, m_im->width, m_im->height, AXIS_WH_XY );
}


CGpwImage::~CGpwImage()
{

}




void CGpwImage::PointW( int wx, int wy, int color )
{
	color = COLOR_SWAP_RB( color );
	image4_draw_point( m_im, wx, wy, color );
}





void CGpwImage::LineW( int wx0, int wy0, int wx1, int wy1, int color, int width )
{
	

//	color = (color & 0xff00) | ( (color>>16) &0xff ) | ( (color<<16) &0xff0000 );
	color = COLOR_SWAP_RB( color );

	image4_draw_line( m_im, wx0, wy0 ,wx1, wy1, color );

}





void CGpwImage::String( float x, float y,
					 char *s, int len, int color )
{
	//int	wx,	wy;

	//SetTextColor( m_dc, color );
	//SetBkMode( m_dc, TRANSPARENT );


	//xy2win(  x, y, &wx, &wy );

	//TextOut( m_dc, wx, wy, (LPCWSTR)s, len );
}


void CGpwImage::RectangleW( int wx0, int wy0, int wx1, int wy1, int color )
{
	color = COLOR_SWAP_RB( color );

	image_draw_rectangle( m_im, wx0, wy0, wx1 - wx0+1, wy1 - wy0+1, color );
}




