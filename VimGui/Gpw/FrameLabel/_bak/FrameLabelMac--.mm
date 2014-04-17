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







int  CFrameLabelMac::Init( char *file )
{
	//m_drag = drag;
    
    // missing!!! char* file length
    NSString *filename = [NSString stringWithUTF8String:<#(const char *)#>:(const char*)file];
    img = [[UIImageView alloc] initWithImage:[UIImage imageNamed:filename]];
    [drawOn addSubview:img];
    [img setHidden:YES];
    [img setUserInteractionEnabled:NO];
    
 //   m_x = 50;
 //   m_y = 50;

	return( 1 );

}

int CFrameLabelMac::Draw(){
    [img setHidden:YES];
    return(0);
}


void CFrameLabelMac::SetPose( int x, int y )
{
	m_x = x;
	m_y = y;
    
    [img setFrame:CGRectMake(x, y, img.frame.size.width, img.frame.size.height)];
}


void CFrameLabelMac::GetPose( int *x, int *y )
{
	*x = m_x;
	*y = m_y;
}

void CFrameLabelMac::SetShow( int flag ){
    m_show = flag;
    [img setHidden:(flag == 1) ? NO : YES];
}

void CFrameLabelMac::SetDrag( int flag ){
    m_drag = flag;
    
    [img setUserInteractionEnabled:(flag == 1) ? YES : NO];
}

