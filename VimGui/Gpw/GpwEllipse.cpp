
#include        <math.h>
#include	"Uigp/igp.h"

#include	"Gpw.h"




void CGpw::Ellipse(float x, float y,
			float r1, float r2, float angle, int color)
{
	float	cosa,	sina,	t,	scale,	pi_2;
	float	rxt,	ryt,	xt,	yt;
//	int	wx,	wy;

	//if( gpw->im != NULL ){
	//	image_draw_ellipse( gpw->im, x, y, r1, r2, angle, color );
	//	return;
	//}

	pi_2 = (float)(2.0 * M_PI);
	scale = m_unit*pi_2 / (6 * r1 );



	cosa = (float)cos(angle);
	sina = (float)sin(angle);


	for( t = 0 ; t <= pi_2; t += scale ) {
		xt = (float)(r1 * cos(t));
		yt = (float)(r2 * sin(t));

		/* rotate  */
		rxt = cosa * xt + sina * yt;
		ryt = -sina * xt + cosa * yt;



		///* transform to sunview coordinates */
		//gpw_xy2win( gpw, x+rxt, y+ryt, &wx, &wy );

		//SetPixel( gpw->dc, wx, wy, (COLORREF)color );

		Point( x+rxt, y+ryt, color );
	}
}



void CGpw::EellipseR( float x, float y,
			 float r1, float r2, float angle, float angle0, float angle1, int color )
{
	float	cosa,	sina,	t,	scale,	pi_2;
	float	rxt,	ryt,	xt,	yt;
//	int	wx,	wy;
	float	t0,	t1;

	//if( gpw->im != NULL ){
	//	image_draw_ellipse( gpw->im, x, y, r1, r2, angle, color );
	//	return;
	//}

	pi_2 = (float)(2.0 * M_PI);
	scale = m_unit*pi_2 / (6 * MAX(r1,r2) );



	cosa = (float)cos(angle);
	sina = (float)sin(angle);

	t0 = ANGLE_D2R( angle0 );
	t1 = ANGLE_D2R( angle1 );


	//	for( t = 0 ; t <= pi_2; t += scale ) {
	for( t = t0 ; t <= t1; t += scale ) {

		xt = (float)(r1 * cos(t));
		yt = (float)(r2 * sin(t));

		/* rotate  */
		rxt = cosa * xt + sina * yt;
		ryt = -sina * xt + cosa * yt;

		///* transform to sunview coordinates */
		//gpw_xy2win( gpw, x+rxt, y+ryt, &wx, &wy );

		//SetPixel( gpw->dc, wx, wy, (COLORREF)color );

		Point( x+rxt, y+ryt, color );
	}
}


#ifdef _AA_
void 
gpw_Ycurve( gpw_type *gpw, vec2d *ctr, vec2d *Ydirection,
		float x0, float x1, float Curve[4], int dgree, int color )
{
vec2d   Xdirection,	p;
float	X,	Y,	dx;
int	drow_mod;


	Xdirection.x = Ydirection->y;
	Xdirection.y = -Ydirection->x;

	dx = 0.5F*gpw->Unit;

	x0 = ((int)(x0/dx)) *dx;


	drow_mod = GPW_MOVE;

	for( X = x0 ; X <= x1 ; X += dx ){
		if( dgree == 3 )
			Y = Curve[0] + X * (Curve[1] + X *
					(Curve[2] + X * Curve[3]));
		else	Y = Curve[0] + X * (Curve[1] + X * Curve[2]);

		p.x = ctr->x + X * Xdirection.x + Y * Ydirection->x;
		p.y = ctr->y + X * Xdirection.y + Y * Ydirection->y;

		gpw_move( gpw, p.x, p.y, color, drow_mod );
		drow_mod = GPW_DRAW;
	}
}



void 
gpw_Tcurve( gpw_type *gpw, float t0, float t1,
		float Cx[4], float Cy[4], int dgree, int color )
{
vec2d   p;
float	t,	dt;
int	drow_mod;



	dt = 0.5F*gpw->Unit;

	t0 = ((int)(t0/dt)) *dt;


	drow_mod = GPW_MOVE;

	for( t = t0 ; t <= t1 ; t += dt ){
		if( dgree == 3 ){
			p.x = Cx[0] + t * (Cx[1] + t * (Cx[2] + t * Cx[3]));
			p.y = Cy[0] + t * (Cy[1] + t * (Cy[2] + t * Cy[3]));
		}
		else	{
			p.x = Cx[0] + t * (Cx[1] + t * Cx[2]);
			p.y = Cy[0] + t * (Cy[1] + t * Cy[2]);
		}

		gpw_move( gpw, p.x, p.y, color, drow_mod );
		drow_mod = GPW_DRAW;
	}
}
#endif


