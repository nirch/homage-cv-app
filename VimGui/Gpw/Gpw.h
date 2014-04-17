/*****************
 ***   Gpw.h   ***
 *****************/

#ifndef	_GPW_
#define	_GPW_


#include		"Uigp/igp.h"
#include        "Uvec/Vec2d.h"
#include        "Ubox/Box2d.h"



#include		"GpColor.h"



/*	for Axis_type */
#define		AXIS_WH_XY			0		// W -> X	H -> Y
#define		AXIS_WH_XFY			1		// W -> X	H -> FY

#define		AXIS_WH_YX			2		// W -> Y	H -> X
#define		AXIS_WH_YFX			3		// W -> Y	H -> FX

#define		IGP_AXIS			AXIS_WH_YX
#define		NORMAL_AXIS			AXIS_WH_XFY





class CGpw
{
public:
	CGpw();
	virtual	~CGpw();

	float	Unit()				{ return( m_unit ); }
	float	W2xy( int pixel )	{ return( pixel * m_unit ); }

	void	SetWin( int wx_min, int wy_min, int wx_max, int wy_max );

	void	ResetWin( int wx_min, int wy_min, int wx_max, int wy_max );

	void	SetXy( float x_min, float y_min, float x_max, float y_max, int axis = -1 );

	void	xy2win( float x, float y, int *wx, int *wy );

	void	win2xy( int wx, int wy, float *x, float *y );

	void	GetBox( box2d *box );


	virtual void PointW( int wx, int wy, int color ) {};//= 0;

	virtual void LineW( int wx0, int wy0, int wx1, int wy1, int color, int width = 0 ) {};//= 0 ;

	virtual void RectangleW( int x0, int y0, int x1, int y1, int color ) {};//= 0;

	virtual void String( float x, float y, char *s, int len, int color ) {};//= 0;



	virtual void Point( float x, float y, int color );

	virtual void Line( float x0, float y0, float x1, float y1, int color, int width = 0 );

	virtual void Rectangle( float x0, float y0, float x1, float y1, int color );


	virtual int IsImage()	{ return(0); }


    virtual void  Start()	{}
    
    virtual void  End()	{}


//	virtual void Rectangle_e( float x0, float y0, float x1, float y1, int color ) = 0;



	void Ellipse(float x, float y, float r1, float r2, float angle, int color );

	void EellipseR( float x, float y, float r1, float r2, float angle, float angle0, float angle1, int color );


	void Marker_p0( float x, float y, int color );

	void Marker_0( float x, float y, int d, int color );



	void Pln( struct plnA_type *apl, int color0, int color1 );

	void Pln( struct pln_type *pl, int color0, int color1, int flag );

	void LnL( vec2d *ctr, struct ln_type *link, int color0, int color1, int flag );

	void Ln( vec2d *ctr, struct ln_type *link, int color, int flag );



private:
	void SetXy_igp();

	void SetXy_normal();

	void SetXy_WXY_XY();

	void SetXy_WXY_YFX();



private:
	int	WXmin,	WXmax;
	int	WYmin,	WYmax;

	float	Xmin,	Xmax;
	float	Ymin,	Ymax;

	int	Axis_type;

	float	W2XY[2][3];
	float	XY2W[2][3];


	float	m_unit;	// number of pixel for each XY unit


	int		Fcs;
	int		cs_x,	cs_y;
	int		cs[5];


	int	wx,	wy;

};



#ifdef _AA_

/*	for gpw_move option */
#define		GPW_MOVE	0
#define		GPW_DRAW	1

#define		GPW_COPY	R2_COPYPEN
#define		GPW_XOR		R2_XORPEN



#define		GPW_IS_IN( gpw, d, x, y )       \
		( x >= gpw->Xmin -d  && x < gpw->Xmax +d &&\
			y >= gpw->Ymin -d && y < gpw->Ymax +d )



void	gpw_background( gpw_type *gpw, int color );




	/* GpwMarker.c */
void	gpw_marker_2( gpw_type *gpw, float x, float y, int size, int color );

void	gpw_marker_3( gpw_type *gpw, float x, float y, int size, int color );

void	gpw_marker_box( gpw_type *gpw, float x, float y, int n, int color );

void	gpw_marker_triangle( gpw_type *gpw, float x, float y,
                                        vec2d *p, int n, int color );

void	gpw_marker_ellipse( gpw_type *gpw, float x, float y,
                        float r1, float r2, float angle, int color);

void 	gpw_marker_arrow( gpw_type *gpw, float x, float y, int size, int color);

void	gpw_marker_box2i( gpw_type *gpw, box2i *rm, int draw_op, int color );

void	gpw_marker_rectangle( gpw_type *gpw,
				float x, float y, int size, int color );


	/* GpwCurve.c */
void 	gpw_Ycurve( gpw_type *gpw, vec2d *ctr, vec2d *Ydirection,
                float x0, float x1, float Curve[4], int dgree, int color );

void 	gpw_Tcurve( gpw_type *gpw, float t0, float t1,
                float Cx[4], float Cy[4], int dgree, int color );


	/* GpwCs.c */

void	gpw_cs( gpw_type *gpw, float x, float y );

void	gpw_cs_unshow( gpw_type *gpw );


	/* GpwBezier */
void	gpw_bezier( gpw_type *gpw, vec2d* p0,vec2d* p1,vec2d* p2,vec2d* p3,int color );

#endif

#endif
