/***************************
 ***   FrameLabel.cpp   ***
 ***************************/


#include "ImageLabelIos.h"



CImageLabelIos::CImageLabelIos( void *w )
{
    drawOn = (UIView*)w;
}



CImageLabelIos::~CImageLabelIos()
{
    [m_view release];
}



int  CImageLabelIos::Init( char *file, int x, int y )
{
	  
    if( Init(  file ) < 0 )
        return( -1 );


    
 
    m_fx = x;
    m_fy = y;
    
	return( 1 );
    
}



int  CImageLabelIos::Init( char *file )
{    
    NSString *filename = [NSString stringWithUTF8String:file];
    UIImage *image = [UIImage imageWithContentsOfFile:filename];
    m_view = [[UIImageView alloc] initWithImage:image];
    [drawOn addSubview:m_view];
    [m_view setHidden:YES];
    [m_view setUserInteractionEnabled:YES];

	return( 1 );

}  

int CImageLabelIos::Draw( void *dc, int wx, int wy )
{

//    if (m_view.superview != drawOn){
//        [drawOn addSubview:m_view];
//    }

//    [m_view setHidden:NO];
    
    SetPose( wx, wy );

    return(1);
}


void CImageLabelIos::SetPose( int x, int y )
{
	[m_view setFrame:CGRectMake( x - m_fx, y - m_fy, m_view.frame.size.width, m_view.frame.size.height)];
}



void CImageLabelIos::SetVisible( int flag )
{
    m_visible = flag;
    [m_view setHidden:(flag == 1) ? NO : YES];
}



void CImageLabelIos::SetDrag( int flag )
{
    m_drag = flag;
    
    [m_view setUserInteractionEnabled:(flag == 1) ? YES : NO];
}



int CImageLabelIos::IsTouchBegin( void *view, int x, int y )
{

	if( view != m_view )
	return( -1 );

	m_hint = 1;

	return( 1 );
}

int CImageLabelIos::IsTouchMove( void *view, int x, int y )
{
	if( view == m_view )
		return( 1 );

	return( -1 );
}

int CImageLabelIos::IsTouchEnd( void *view, int x, int y )
{
	if( view == m_view ){
		m_hint = -1;
		return( 1 );
	}

	return( -1 );
}
