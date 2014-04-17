/***************************
 ***   FrameLabel.cpp   ***
 ***************************/


#include "FrameLabelMac.h"



CFrameLabelMac::CFrameLabelMac( void *w )
{
    drawOn = (UIView*)w;
}



CFrameLabelMac::~CFrameLabelMac()
{
    [img release];
}



int  CFrameLabelMac::Init( int x, int y, char *file )
{
	//m_drag = drag;
    
    if( Init(  file ) < 0 )
        return( -1 );
    
    SetFocus( x, y );
    
	return( 1 );
    
}



int  CFrameLabelMac::Init( char *file )
{
	//m_drag = drag;
    
    NSString *filename = [NSString stringWithUTF8String:file];
    UIImage *image = [UIImage imageWithContentsOfFile:filename];
    img = [[UIImageView alloc] initWithImage:image];
    [drawOn addSubview:img];
    [img setHidden:YES];
    [img setUserInteractionEnabled:YES];

	return( 1 );

}

int CFrameLabelMac::Draw(){
    if (img.superview != drawOn){
        [drawOn addSubview:img];
    }
    [img setHidden:NO];
    return(0);
}


void CFrameLabelMac::SetPose( float x, float y )
{
	m_x = x;
	m_y = y;
    
    float ax = m_rs*m_scale*(m_x - m_rx);
    float ay = m_rs*m_scale*(m_y - m_ry);

//    int w = img.frame.size.width;
//    int h = img.frame.size.height;



	m_gpw->xy2win( m_x, m_y, &wx, &wy );
    
    
 //   [img setFrame:CGRectMake( m_x*m_scale - m_fx, m_y*m_scale - m_fy, img.frame.size.width, img.frame.size.height)];
	[img setFrame:CGRectMake( ax - m_fx, ay - m_fy, img.frame.size.width, img.frame.size.height)];
}



void CFrameLabelMac::SetShow( int flag ){
    m_show = flag;
    [img setHidden:(flag == 1) ? NO : YES];
}

void CFrameLabelMac::SetDrag( int flag ){
    m_drag = flag;
    
    [img setUserInteractionEnabled:(flag == 1) ? YES : NO];
}

