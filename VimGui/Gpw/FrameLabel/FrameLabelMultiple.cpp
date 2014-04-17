/***************************
 ***   FrameLabel.cpp   ***
 ***************************/


#include "FrameLabelMultiple.h"



CFrameLabelMultiple::CFrameLabelMultiple()
{
    m_fx = 0;
    m_fy = 0;
    
    m_scale = 1.0;

	m_nF = 0;
	m_show = 0;
}



CFrameLabelMultiple::~CFrameLabelMultiple()
{

}







int  CFrameLabelMultiple::Add( CFrameLabel *f )
{
	m_af[m_nF++] = f;

	f->SetPose( m_x, m_y );

	return( 1 );

}


void CFrameLabelMultiple::SetShow( int iF )
{
	int	i;

	m_show = iF;

	for( i = 0 ; i < m_nF ; i++ )
		m_af[i]->SetShow( (i == m_show) );
}


void CFrameLabelMultiple::Draw( void *dc )
{

	if( m_show >= 0 )
		m_af[m_show]->Draw( dc );
}



void CFrameLabelMultiple::Set( int visable, int x, int y )
{
    SetBasePose( x, y );
    
    SetShow( visable );
}


void CFrameLabelMultiple::SetBasePose( int x, int y )
{
    m_x0 = x;
    m_y0 = y;
    
    SetPose( m_x0, m_y0 );
}

void CFrameLabelMultiple::SetPose( float x, float y )
{
int	i;
	m_x = x;
	m_y = y;

	for( i = 0 ; i < m_nF ; i++ )
		m_af[i]->SetPose( m_x, m_y );
}


void CFrameLabelMultiple::SetPose( vec2f_type *p )
{
    int x = p->x;
    int y = p->y;
    
    SetPose( x, y );
}


void CFrameLabelMultiple::StartMove( int x, int y )
{
    m_mx = x;
    m_my = y;
}


void CFrameLabelMultiple::MoveTo( int x, int y )
{
    
    SetMove( x - m_mx, y - m_my );
    m_mx = x;
    m_my = y;
}



void CFrameLabelMultiple::SetMove( int ix, int iy )
{
float   x,  y;
    
    x = ix / m_scale;
    y = iy / m_scale;
        
    
    m_x0 += x;
    m_y0 += y;
    
    SetPose( x + m_x, y + m_y );
}


void CFrameLabelMultiple::GetPose( int *x, int *y )
{
	*x = m_x0;
	*y = m_y0;
}

void CFrameLabelMultiple::GetPose( float *x, float *y )
{
	*x = m_x0;
	*y = m_y0;
}

void CFrameLabelMultiple::GetPose( vec2f_type *p )
{
	p->x = m_x0;
	p->y = m_y0;
}

