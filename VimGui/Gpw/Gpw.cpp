/***********************
 ***   GpwTool.cpp   ***
 ***********************/
#include	<math.h>


#include	"Uigp/igp.h"



#include "Gpw.h"



CGpw::CGpw()
{
	Axis_type = IGP_AXIS;
}


CGpw::~CGpw()
{

}

void CGpw::SetWin( int wx_min, int wy_min, int wx_max, int wy_max )
{
	WXmin = wx_min;
	WYmin = wy_min;

	WXmax = wx_max;
	WYmax = wy_max;



	wx = WXmin;
	wy = WYmin;
	Fcs = 0;
}


void CGpw::ResetWin( int wx_min, int wy_min, int wx_max, int wy_max )
{
 
	SetWin( wx_min, wy_min, wx_max, wy_max );

	SetXy( Xmin, Ymin, Xmax, Ymax, Axis_type );
}







void CGpw::SetXy( float x_min, float y_min, float x_max, float y_max, int axis )
{
float	x0,	y0,	x1,	y1;


	Xmin = x_min;
	Ymin = y_min;

	Xmax = x_max;
	Ymax = y_max;

	if( axis >= 0 )
		Axis_type = axis;

	switch( Axis_type ){
	case AXIS_WH_XY:
			SetXy_WXY_XY(  );
		break;
	case IGP_AXIS:
			SetXy_igp(  );
	    break;
	case NORMAL_AXIS:
			SetXy_normal(  );
	    break;
	case AXIS_WH_YFX:
			SetXy_WXY_YFX(  );
		break;
	}


	win2xy( 1, 1, &x0, &y0 );
	win2xy( 0, 0, &x1, &y1 );
	m_unit = x1-x0;
	m_unit = ABS( m_unit );
	
}



void CGpw::SetXy_igp()
{
	XY2W[0][2] = (WXmax - WXmin )/ ( Ymax - Ymin );
	XY2W[0][1] = 0;
	XY2W[0][0] = WXmin  - Ymin * XY2W[0][2];

	XY2W[1][2] = 0;
	XY2W[1][1] = (WYmax - WYmin )/ ( Xmax - Xmin );
	XY2W[1][0] = WYmin  - Xmin * XY2W[1][1];



	W2XY[0][1] = 0;
	W2XY[0][2] = (Xmax - Xmin )/ ( WYmax - WYmin );
	W2XY[0][0] = Xmin  - WYmin * W2XY[0][2];

	W2XY[1][2] = 0;
	W2XY[1][1] = (Ymax - Ymin )/ ( WXmax - WXmin );
	W2XY[1][0] = Ymin  - WXmin * W2XY[1][1];
}

void CGpw::SetXy_normal()
{
	// x -> wx
	XY2W[0][2] = 0;
	XY2W[0][1] = (WXmax - WXmin )/ ( Xmax - Xmin );
	XY2W[0][0] = WXmin  - Xmin * XY2W[0][1];

	// y -> wy
	XY2W[1][2] = (WYmax - WYmin )/ ( Ymin - Ymax );
	XY2W[1][1] = 0;
	XY2W[1][0] = WYmax  - Ymin * XY2W[1][2];


	// wx -> x
	W2XY[0][2] = 0;
	W2XY[0][1] = (Xmax - Xmin )/ ( WXmax - WXmin );
	W2XY[0][0] = Xmin  - WXmin * W2XY[0][1];

	// wy -> y
	W2XY[1][2] = (Ymax - Ymin )/ ( WYmin - WYmax );
	W2XY[1][1] = 0;
	W2XY[1][0] = Ymax  - WYmin * W2XY[1][2];
}
 




void CGpw::SetXy_WXY_XY()
{
	// x -> wx
	XY2W[0][2] = 0;
	XY2W[0][1] = (WXmax - WXmin )/ ( Xmax - Xmin );
	XY2W[0][0] = WXmin  - Xmin * XY2W[0][1];

	// y -> wy
	XY2W[1][2] = (WYmax - WYmin )/ ( Ymax - Ymin );
	XY2W[1][1] = 0;
	XY2W[1][0] = WYmin  - Ymin * XY2W[1][2];


	// wx -> x
	W2XY[0][2] = 0;
	W2XY[0][1] = (Xmax - Xmin )/ ( WXmax - WXmin );
	W2XY[0][0] = Xmin  - WXmin * W2XY[0][1];

	// wy -> y
	W2XY[1][2] = (Ymax - Ymin )/ ( WYmax - WYmin );
	W2XY[1][1] = 0;
	W2XY[1][0] = Ymin  - WYmin * W2XY[1][2];
}






void CGpw::SetXy_WXY_YFX()
{
	// y -> wx
	XY2W[0][2] = (WXmax - WXmin )/ ( Ymax - Ymin );
	XY2W[0][1] = 0;
	XY2W[0][0] = WXmin  - Ymin * XY2W[0][2];

	// x -> wy
	XY2W[1][2] = 0;
	XY2W[1][1] = -(WYmax - WYmin )/ ( Xmax - Xmin );
	XY2W[1][0] = WYmax  - Xmin * XY2W[1][1];


	// wy -> x
	W2XY[0][1] = 0;
	W2XY[0][2] = -(Xmax - Xmin )/ ( WYmax - WYmin );
	W2XY[0][0] = Xmax  + WYmin * W2XY[0][2];

	// wx -> y
	W2XY[1][2] = 0;
	W2XY[1][1] = (Ymax - Ymin )/ ( WXmax - WXmin );
	W2XY[1][0] = Ymin  - WXmin * W2XY[1][1];
}



void CGpw::xy2win( float x, float y, int *wx, int *wy )
{
	*wx = (int)(XY2W[0][0] + XY2W[0][1] *x + XY2W[0][2] *y + 0.5F);
	*wy = (int)(XY2W[1][0] + XY2W[1][1] *x + XY2W[1][2] *y + 0.5F);
}


void CGpw::win2xy( int wx, int wy, float *x, float *y )
{
	*x = W2XY[0][0] + W2XY[0][1] *wx + W2XY[0][2] *wy;
	*y = W2XY[1][0] + W2XY[1][1] *wx + W2XY[1][2] *wy;
}



void CGpw::GetBox( box2d *box )
{
	box->x0 = Xmin;
	box->x1 = Xmax;

	box->y0 = Ymin;
	box->y1 = Ymax;
}



void CGpw::Point( float x, float y, int color )
{
	int	wx,	wy;

	xy2win(  x, y, &wx, &wy );

	PointW(wx, wy, color );
}



void CGpw::Line( float x0, float y0, float x1, float y1, int color, int width )
{
	int	wx0,	wy0;
	int	wx1,	wy1;


	xy2win(  x0, y0, &wx0, &wy0 );
	xy2win(  x1, y1, &wx1, &wy1 );

	LineW( wx0, wy0, wx1, wy1, color, width );
}


void CGpw::Rectangle( float x0, float y0, float x1, float y1, int color )
{
	int	wx0,	wy0,	wx1,	wy1;


	xy2win(  x0, y0, &wx0, &wy0 );
	xy2win(  x1, y1, &wx1, &wy1 );


	RectangleW( wx0, wy0, wx1, wy1, color );
}


