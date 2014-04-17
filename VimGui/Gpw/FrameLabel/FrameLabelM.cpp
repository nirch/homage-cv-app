/***************************
 ***   FrameLabel.cpp   ***
 ***************************/


#include "FrameLabelM.h"
#include "../Gpw.h"


CFrameLabelM::CFrameLabelM( class CGpw *gpw, int dragable )
{
	m_nL = 0;
	m_iL = 0;

	m_gpw = gpw;

	m_drag = dragable;
	m_visible = 1;


}



CFrameLabelM::~CFrameLabelM()
{

}





int  CFrameLabelM::Add( CImageLabel *label )
{
	m_aL[m_nL++] = label;

	if( m_iL < 0 )
		m_iL = 0;

	return( 1 );

}


void CFrameLabelM::Draw( void *dc )
{

	if( m_visible <= 0 )
		return;

	int	wx,	wy;
	m_gpw->xy2win( m_x, m_y, &wx, &wy );
	m_aL[m_iL]->Draw( dc, wx, wy );
}


void CFrameLabelM::SetVisible( int flag )
{
//	if( m_visible == flag )
//		return;

	if( m_visible == 1 )
		m_aL[m_iL]->SetVisible( 0 );

	m_visible = flag;

	if( m_visible == 1 )
		m_aL[m_iL]->SetVisible( 1 );
}


void CFrameLabelM::SetLabel( int iL )
{
    if( m_visible == 1 )
        m_aL[m_iL]->SetVisible( 0 );
        
	m_iL = iL;

	m_aL[m_iL]->SetDrag( m_drag );

    m_aL[m_iL]->SetVisible( m_visible );


	SetPose( m_x, m_y );
}

void CFrameLabelM::SetDrag( int flag )
{
	m_drag = flag;

	SetDrag( m_drag );

	SetPose( m_x, m_y );
}


void CFrameLabelM::Set( int visable, int x, int y )
{
    SetBasePose( x, y );
    
    SetVisible( visable );
}


void CFrameLabelM::SetBasePose( float x, float y )
{
    m_bp.x = x;
    m_bp.y = y;
    
    SetPose(  m_bp.x,  m_bp.y );
}


void CFrameLabelM::SetPose( vec2f_type *p )
{   

	SetPose( p->x, p->y );

}


void CFrameLabelM::SetPose( float x, float y )
{
	m_x = x;
	m_y = y;

	int	wx,	wy;
	m_gpw->xy2win( m_x, m_y, &wx, &wy );

	if( m_visible == 1 )
		m_aL[m_iL]->SetPose( wx, wy );
}




void CFrameLabelM::StartMove( int wx, int wy )
{
    m_gpw->win2xy( wx, wy, &m_mp.x, &m_mp.y );
}



void CFrameLabelM::MoveTo( int wx, int wy )
{
vec2f_type	p;

	if( m_drag == 0 )	return ;

     m_gpw->win2xy( wx, wy, &p.x, &p.y );

    float dx = p.x - m_mp.x;
	float dy = p.y - m_mp.y;


	m_bp.x += dx;
	m_bp.y += dy;

	SetPose( m_x + dx, m_y + dy );

   
	m_mp = p;
}



void CFrameLabelM::GetPose( int *x, int *y )
{
	*x = m_bp.x;
	*y = m_bp.y;
}

void CFrameLabelM::GetPose( float *x, float *y )
{
	*x = m_bp.x;
	*y = m_bp.y;
}

void CFrameLabelM::GetPose( vec2f_type *p )
{
	*p = m_bp;
}




int CFrameLabelM::TouchBegin( void *view, int x, int y )
{
	if( m_drag == 0 || m_visible == 0)
		return( -1 );

	
	int ret = m_aL[m_iL]->IsTouchBegin( view, x, y );

	if( ret == 1 )
		StartMove( x, y );

	return( ret );

}

int CFrameLabelM::TouchMove( void *view, int x, int y )
{
	if( m_drag == 0 || m_visible == 0)
		return( -1 );


	int ret = m_aL[m_iL]->IsTouchMove( view, x, y );

	if( ret == 1 )
		MoveTo( x, y );


	return( ret );
}


int CFrameLabelM::TouchEnd( void *view, int x, int y )
{
	if( m_drag == 0 || m_visible == 0)
		return( -1 );

	int ret = m_aL[m_iL]->IsTouchEnd( view, x, y );

	return( ret );
}


