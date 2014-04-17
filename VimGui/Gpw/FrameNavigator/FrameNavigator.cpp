/******************************
 ***   FrameNavigator.cpp   ***
 ******************************/
#include	<math.h>
#include	"Uigp/igp.h"

#include "FrameNavigator.h"




CFrameNavigator::CFrameNavigator( int wWidth, int wHeight, int width, int height, CGpw *gpw )
{
	m_sWidth = wWidth;
	m_sHeight = wHeight;

//	m_gpw = new CGpw();
//	m_gpw->SetWin( 0, 0, m_sWidth, m_sHeight );
    
    m_gpw = gpw;


	SetSize( width, height );
}



CFrameNavigator::~CFrameNavigator()
{
}





void CFrameNavigator::SetWinSize( int width, int height )
{
	m_sWidth = width;
	m_sHeight = height;
}

void CFrameNavigator::SetSize( int width, int height )
{
	m_width = width;
	m_height = height;

	m_b.x0 = 0;
	m_b.x1 = m_width;

	m_b.y0 = 0;
	m_b.y1 = m_height;


	m_gpw->SetXy( m_b.x0, m_b.y0, m_b.x1, m_b.y1, AXIS_WH_XY );

	m_scale = m_sWidth / (float)( m_b.x1 - m_b.x0);
}

int CFrameNavigator::Move( vec2i_type p0[], vec2i_type p1[], int n )
{
	if( n == 2 ){
		Move2( &p0[0], &p1[0],  &p0[1], &p1[1] );
		return( 1 );
	}

	if( n == 1 ){
//		Move( &p0[0], &p1[0] );
//		return( 1 );
	}

	return( -1 );
}


int CFrameNavigator::Move( vec2i_type *p0, vec2i_type *p1 )
{
    vec2f_type  P0, P1;

    m_gpw->win2xy( p0->x, p0->y, &P0.x, &P0.y );
    m_gpw->win2xy( p1->x, p1->y, &P1.x, &P1.y );

	int	dx = P1.x - P0.x;
	int dy = P1.y - P0.y;

	
	m_b.x0 -= dx;
	m_b.x1 -= dx;
	m_b.y0 -= dy;
	m_b.y1 -= dy;


	PushTo();

	return( 1 );
}


int CFrameNavigator::Move2( vec2i_type *p0, vec2i_type *p1, vec2i_type *q0, vec2i_type *q1 )
{
vec2f_type	P,	Q;


	vec2f_type	dp;
	dp.x = q0->x - p0->x;
	dp.y = q0->y - p0->y;
	float pl = hypot( dp.x, dp.y );

	dp.x = q1->x - p1->x;
	dp.y = q1->y - p1->y;
	float ql = hypot( dp.x, dp.y );

	float s = ql/pl;

	vec2f_type	p;
	p.x = 0.5*(q0->x + p0->x );
	p.y = 0.5*(q0->y + p0->y );
	m_gpw->win2xy( p.x, p.y, &P.x, &P.y );

	vec2f_type	q;
	q.x = 0.5*(q1->x + p1->x );
	q.y = 0.5*(q1->y + p1->y );
	m_gpw->win2xy( q.x, q.y, &Q.x, &Q.y );


	float scale = 1.0 / m_gpw->Unit();
	scale *= s;
    
    float t0 = m_sWidth / (float)m_width;

	scale = PUSH_TO_RANGE( scale, t0, t0*4.0 );

	int width = m_sWidth / scale;
	int height = m_sHeight / scale;

	vec2f_type	T;
	m_gpw->SetXy( 0, 0, width, height, AXIS_WH_XY );
	m_gpw->win2xy( q.x, q.y, &T.x, &T.y );

	T.x = P.x - T.x;
	T.y = P.y - T.y;

	m_b.x0 = T.x;
	m_b.x1 = m_b.x0 + width;

	m_b.y0 = T.y;
	m_b.y1 = m_b.y0 + height;


    m_gpw->SetXy( m_b.x0, m_b.y0, m_b.x1, m_b.y1, AXIS_WH_XY );
	m_gpw->win2xy( p.x, p.y, &T.x, &T.y );


	PushTo();

	return( 1 );

}


/*
int CFrameNavigator::Move2( vec2i_type *p0, vec2i_type *p1, vec2i_type *q0, vec2i_type *q1 )
{
	vec2f_type	P0,	P1,	Q0	,Q1;


	vec2f_type	dp;
	dp.x = q0->x - p0->x;
	dp.y = q0->y - p0->y;
	float pl = hypot( dp.x, dp.y );

	dp.x = q1->x - p1->x;
	dp.y = q1->y - p1->y;
	float ql = hypot( dp.x, dp.y );

	float s = pl/ql;


	p.x = 0.5*(q0->x + p0->x );
	p.y = 0.5*(q0->y + p0->y );
	m_gpw->win2xy( p0->x, p0->y, &P0.x, &P0.y );
	m_gpw->win2xy( p1->x, p1->y, &P1.x, &P1.y );
	m_gpw->win2xy( q0->x, q0->y, &Q0.x, &Q0.y );
	m_gpw->win2xy( q1->x, q1->y, &Q1.x, &Q1.y );

	vec2f_type	P,	Q;
	P.x = 0.5*( P0.x + Q0.x );
	P.y = 0.5*( P0.y + Q0.y );

	Q.x = 0.5*( P1.x + Q1.x );
	Q.y = 0.5*( P1.y + Q1.y );


	vec2f_type	dp;
	dp.x = Q0.x - P0.x;
	dp.y = Q0.y - P0.y;
	float pl = hypot( dp.x, dp.y );

	dp.x = Q1.x - P1.x;
	dp.y = Q1.y - P1.y;
	float ql = hypot( dp.x, dp.y );

	float s = pl/ql;



	m_b.x0 = s*( m_b.x0 - P.x ) + Q.x;
	m_b.x1 = s*( m_b.x1 - P.x ) + Q.x;

	m_b.y0 = s*( m_b.y0 - P.y ) + Q.y;
	m_b.y1 = s*( m_b.y1 - P.y ) + Q.y;



	PushTo();

	return( 1 );

}
*/

void CFrameNavigator::PushTo()
{
int	t;

	if( m_b.x1 - m_b.x0 > m_width ){
		m_b.x0 = 0;
		m_b.x1 = m_width;
	}



	if( m_b.x0 < 0 ){
		m_b.x1 += -m_b.x0;
		m_b.x0 = 0;
	}
	if( m_b.x1 > m_width ){
		t = m_b.x1 - m_width;
		m_b.x0 -= t;
		m_b.x1 -= t;
	}





	if( m_b.y1 - m_b.y0 > m_height ){
		m_b.y0 = 0;
		m_b.y1 = m_height;
	}


	if( m_b.y0 < 0 ){
		m_b.y1 += -m_b.y0;
		m_b.y0 = 0;
	}
	if( m_b.y1 > m_height ){
		t = m_b.y1 - m_height;
		m_b.y0 -= t;
		m_b.y1 -= t;
	}


	m_scale = m_sWidth / (float)( m_b.x1 - m_b.x0);
	m_gpw->SetXy( m_b.x0, m_b.y0, m_b.x1, m_b.y1, AXIS_WH_XY );
    
    float t1 = (m_b.x1 - m_b.x0 )/(float)(m_b.y1 - m_b.y0 );
    float t2 = m_sWidth / (float)m_sHeight;
}



