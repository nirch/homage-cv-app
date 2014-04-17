/***************************
 ***   FrameLabel.cpp   ***
 ***************************/


#include "ImageLabel.h"



CImageLabel::CImageLabel()
{

}


CImageLabel::CImageLabel( void *w )
{

}


CImageLabel::~CImageLabel()
{
}



int  CImageLabel::Init( char *file, int fx, int fy )
{
	if( Init(  file ) < 0 )
		return( -1 );

	m_fx = fx;
	m_fx = fx;

	return( 1 );
}


int  CImageLabel::Init( char *file )
{

		return( 1 );
}


int CImageLabel::Draw( void *dc, int wx, int wy )
{
   
	return(0);
}


void CImageLabel::SetPose( int x, int y )
{
	m_x = x;
	m_y = y;
    
}



void CImageLabel::SetVisible( int flag )
{
    m_visible = flag;
 
}

void CImageLabel::SetDrag( int flag )
{
    m_drag = flag;
    
}

