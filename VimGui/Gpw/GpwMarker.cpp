#include        <math.h>

#include	"Uigp/igp.h"
//#include "ImageMark/ImageMark.h"


#include	"Gpw.h"




void CGpw::Marker_p0( float x, float y, int color )
{
int	wx,	wy;

	xy2win(  x, y, &wx, &wy );

	PointW( wx, wy, color );
	PointW( wx-1, wy-1, color );
	PointW( wx-1, wy+1, color );
	PointW( wx+1, wy-1, color );
	PointW( wx+1, wy+1, color );

}




void CGpw::Marker_0( float x, float y, int d, int color )
{
int	wx,	wy;


	xy2win( x, y, &wx, &wy );

	//if( gpw->im != NULL ){
	//	image4_draw_line( gpw->im, wx-d, wy ,wx+d+1, wy, color );
	//	image4_draw_line( gpw->im, wx, wy-d ,wx, wy+d+1, color );

	//	return;
	//}

	LineW( wx-d, wy ,wx+d+1, wy, color );
	LineW( wx, wy-d ,wx, wy+d+1, color );



	//pen = CreatePen(PS_SOLID, 1,(COLORREF)color );

	//open = (HPEN)SelectObject( gpw->dc, pen );


	//MoveToEx( gpw->dc, wx-d, wy, NULL );
	//LineTo( gpw->dc, wx+d+1, wy );

	//MoveToEx( gpw->dc, wx, wy-d, NULL );
	//LineTo( gpw->dc, wx, wy+d+1 );

	//SelectObject( gpw->dc, open );

	//DeleteObject( pen );
}



#ifdef _AA_
void
gpw_marker_1( gpw_type *gpw, float x, float y, vec2d *v,
					int t0, int t1, int color )
{
HPEN	pen,	open;
int	wx0,	wy0,	wx1,	wy1;
vec2d	p;

	p.x = x + gpw->Unit *t0 * v->x;
	p.y = y + gpw->Unit *t0 * v->y;
	gpw_xy2win( gpw, p.x, p.y, &wx0, &wy0 );

	p.x = x + gpw->Unit *t1 * v->x;
	p.y = y + gpw->Unit *t1 * v->y;
	gpw_xy2win( gpw, p.x, p.y, &wx1, &wy1 );

	pen = CreatePen(PS_SOLID, 1,(COLORREF)color );

	open = (HPEN)SelectObject( gpw->dc, pen );


	MoveToEx( gpw->dc, wx0, wy0, NULL );
	LineTo( gpw->dc, wx1, wy1 );


	SelectObject( gpw->dc, open );

	DeleteObject( pen );
}



void
gpw_marker_box( gpw_type *gpw, float x, float y, int n, int color )
{
HPEN	pen,	open;
int	wx,	wy;

	gpw_xy2win( gpw, x, y, &wx, &wy );

	pen = CreatePen(PS_SOLID, 1,(COLORREF)color );

	open = (HPEN)SelectObject( gpw->dc, pen );


	MoveToEx( gpw->dc, wx-n, wy-n, NULL );
	LineTo( gpw->dc, wx-n, wy+n );
	LineTo( gpw->dc, wx+n, wy+n );
	LineTo( gpw->dc, wx+n, wy-n );
	LineTo( gpw->dc, wx-n, wy-n );


	SelectObject( gpw->dc, open );

	DeleteObject( pen );
}



void
gpw_marker_triangle( gpw_type *gpw, float x, float y,
					vec2d *p, int n, int color )
{
HPEN	pen,	open;
float	nx,	ny;
int	wx,	wy;

	pen = CreatePen(PS_SOLID, 1,(COLORREF)color );
	open = (HPEN)SelectObject( gpw->dc, pen );

	nx = n* gpw->Unit * p->x;
	ny = n* gpw->Unit * p->y;

	gpw_xy2win( gpw, x+nx, y + ny, &wx, &wy );
	MoveToEx( gpw->dc, wx, wy, NULL );

	gpw_xy2win( gpw, x-nx - 0.5*ny, y - ny + 0.5*nx, &wx, &wy );
	LineTo( gpw->dc, wx, wy );

	gpw_xy2win( gpw, x-nx + 0.5*ny, y - ny - 0.5*nx, &wx, &wy );
	LineTo( gpw->dc, wx, wy );

	gpw_xy2win( gpw, x+nx, y + ny, &wx, &wy );
	LineTo( gpw->dc, wx, wy );
/*
	MoveToEx( gpw->dc, x+nx, y + ny, NULL );

	LineTo( gpw->dc, x-nx - 0.5*ny, y - ny + 0.5*nx );
	LineTo( gpw->dc, x-nx + 0.5*ny, y - ny - 0.5*nx );
	LineTo( gpw->dc, x+nx, y + ny );
*/


	SelectObject( gpw->dc, open );
	DeleteObject( pen );
}


void
gpw_marker_arrow( gpw_type *gpw, float x, float y, int size, int color )
{
HPEN	pen,	open;
int	wx,	wy;

	pen = CreatePen(PS_SOLID, 1,(COLORREF)color );
	open = (HPEN)SelectObject( gpw->dc, pen );

	gpw_xy2win( gpw, x, y, &wx, &wy );


	MoveToEx( gpw->dc, wx, wy, NULL );
	LineTo( gpw->dc, wx, wy );

	MoveToEx( gpw->dc, wx, wy, NULL );
	LineTo( gpw->dc, wx -size, wy -size );

	MoveToEx( gpw->dc, wx, wy, NULL );
	LineTo( gpw->dc, wx +size, wy -size );

	MoveToEx( gpw->dc, wx -size, wy - size, NULL );
	LineTo( gpw->dc, wx + size, wy - size );


	SelectObject( gpw->dc, open );
	DeleteObject( pen );
}


void
gpw_marker_rectangle( gpw_type *gpw,
				float x, float y, int size, int color )
{
HBRUSH	brush = CreateSolidBrush(color);
RECT rect;
int	wx,	wy;

	gpw_xy2win( gpw, x, y, &wx, &wy );
	rect.left	= wx - size;
	rect.right	= wx + size+1;
	rect.top	= wy - size;
	rect.bottom = wy + size+1;

	FillRect( gpw->dc, &rect, brush );

	DeleteObject( brush );
}

/*
void
gpw_marker_triangle( gpw_type *gpw, float x, float y,
					vec2d *p, int n, int color )
{
float	nx,	ny;

	nx = n* gpw->Unit * p->x;
	ny = n* gpw->Unit * p->y;


	gpw_move( gpw, x+nx, y + ny, color, GPW_MOVE );

	gpw_move( gpw, x-nx - 0.5*ny, y - ny + 0.5*nx, color, GPW_DRAW );
	gpw_move( gpw, x-nx + 0.5*ny, y - ny - 0.5*nx, color, GPW_DRAW );

	gpw_move( gpw, x+nx, y + ny, color, GPW_DRAW );
}
*/


void
gpw_marker_ellipse( gpw_type *gpw, float x, float y,
			float r1, float r2, float angle, int color)
{
float	cosa,	sina,	t,	scale,	pi_2;
float	rxt,	ryt,	xt,	yt;
int	wx,	wy;

	pi_2 = (float)(2.0 * M_PI);
	scale = pi_2 / (6 * r2 );

	r2 *= gpw->Unit;
	r1 *= gpw->Unit;

	cosa = (float)cos(angle);
	sina = (float)sin(angle);

	for( t = 0 ; t <= pi_2; t += scale ) {
		xt = (float)(r2 * cos(t));
		yt = (float)(r1 * sin(t));


		rxt = cosa * xt + sina * yt;
		ryt = -sina * xt + cosa * yt;


		gpw_xy2win( gpw, x+rxt, y+ryt, &wx, &wy );

		SetPixel( gpw->dc, wx, wy, (COLORREF)color );
	}
}

/*
void
gpw_marker_arrow( gpw_type *gpw, float x, float y, int size, int color )
{
Window  win = Xgp_draw_window(gpw->w);
Pixmap  pix = Xgp_draw_rpixmap(gpw->w);
int	wx,	wy;

	gpw_xy2win( gpw, x, y, &wx, &wy );

	if( win != NULL ){
		XSetForeground( gpw->dis, gpw->gc, color );
		XDrawLine( gpw->dis, win, gpw->gc, wx, wy, wx, wy - 4*size );
		XDrawLine( gpw->dis, win, gpw->gc, wx, wy, wx -size, wy -size );
		XDrawLine( gpw->dis, win, gpw->gc, wx, wy, wx +size, wy -size );
		XDrawLine( gpw->dis, win, gpw->gc, wx -size, wy - size,
			wx + size, wy - size );
	}

	if( pix != NULL ){
		XSetForeground( gpw->dis, gpw->gc, color );
		XDrawLine( gpw->dis, pix, gpw->gc, wx, wy, wx, wy - 4*size );
		XDrawLine( gpw->dis, pix, gpw->gc, wx, wy, wx -size, wy -size );
		XDrawLine( gpw->dis, pix, gpw->gc, wx, wy, wx +size, wy -size );
		XDrawLine( gpw->dis, pix, gpw->gc, wx -size, wy - size,
			wx + size, wy - size );
	}
}



void
gpw_marker_box2i( gpw_type *gpw, box2i *rm, int draw_op, int color )
{
Window  win = XtWindow(gpw->w);
Pixmap  pix = Xgp_draw_rpixmap(gpw->w);

	gpw_set_draw_funtion( gpw, draw_op );

	XSetForeground( gpw->dis, gpw->gc, color );

	
	if( XtIsRealized( gpw->w ) ){
		XDrawLine( gpw->dis, win, gpw->gc,
					rm->x0, rm->y0, rm->x0, rm->y1 );
		XDrawLine( gpw->dis, win, gpw->gc,
					rm->x1, rm->y0, rm->x1, rm->y1 );
		XDrawLine( gpw->dis, win, gpw->gc,
					rm->x0+1, rm->y0, rm->x1-1, rm->y0 );
		XDrawLine( gpw->dis, win, gpw->gc,
					rm->x0+1, rm->y1, rm->x1-1, rm->y1 );
	}

	if( pix != 0 ){
		XDrawLine( gpw->dis, pix, gpw->gc,
					rm->x0, rm->y0, rm->x0, rm->y1 );
		XDrawLine( gpw->dis, pix, gpw->gc,
					rm->x1, rm->y0, rm->x1, rm->y1 );
		XDrawLine( gpw->dis, pix, gpw->gc,
					rm->x0+1, rm->y0, rm->x1-1, rm->y0 );
		XDrawLine( gpw->dis, pix, gpw->gc,
					rm->x0+1, rm->y1, rm->x1-1, rm->y1 );
	}

	gpw_set_draw_funtion( gpw, GXcopy );
}
*/
#endif