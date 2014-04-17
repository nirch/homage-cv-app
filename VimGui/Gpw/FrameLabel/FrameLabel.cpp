/***************************
 ***   FrameLabel.cpp   ***
 ***************************/


#include "FrameLabel.h"



CFrameLabel::CFrameLabel()
{
    m_fx = 0;
    m_fy = 0;
    
    m_scale = 1.0;


	m_rx = 0;
	m_ry = 0;
	m_rs = 1.0;

	m_gpw = NULL;
}



CFrameLabel::~CFrameLabel()
{

}







int  CFrameLabel::Init( char *file )
{
	//m_drag = drag;

	return( 1 );

}

int  CFrameLabel::Init( int x, int y, char *file )
{
	//m_drag = drag;
    
	return( 1 );
    
}


void CFrameLabel::SetFocus( int x, int y )
{
    m_fx = x;
    m_fy = y;
}


void CFrameLabel::Set( int visable, int x, int y )
{
    SetBasePose( x, y );
    
    SetShow( visable );
}


void CFrameLabel::SetBasePose( float x, float y )
{
    m_x0 = x;
    m_y0 = y;
    
    SetPose( m_x0, m_y0 );
}

void CFrameLabel::SetPose( float x, float y )
{
	m_x = x;
	m_y = y;
}

void CFrameLabel::SetPose( vec2f_type *p )
{
    int x = p->x;
    int y = p->y;
    
    SetPose( x, y );
}


void CFrameLabel::StartMove( int x, int y )
{
    m_mx = x;
    m_my = y;
}


void CFrameLabel::MoveTo( int x, int y )
{
    
    SetMove( x - m_mx, y - m_my );
    m_mx = x;
    m_my = y;
}



void CFrameLabel::SetMove( int ix, int iy )
{
float   x,  y;

	x = ix / (m_scale*m_rs);
	y = iy / (m_scale*m_rs);
    
    
    m_x0 += x;
    m_y0 += y;
    
    SetPose( x + m_x, y + m_y );
}


void CFrameLabel::GetPose( int *x, int *y )
{
	*x = m_x0;
	*y = m_y0;
}

void CFrameLabel::GetPose( float *x, float *y )
{
	*x = m_x0;
	*y = m_y0;
}

void CFrameLabel::GetPose( vec2f_type *p )
{
	p->x = m_x0;
	p->y = m_y0;
}




void CFrameLabel::SetRelativeScale( float rx, float ry, float rs )
{
	m_rx = rx;
	m_ry = ry;
	m_rs = rs;
    
    SetPose( m_x, m_y );
}
