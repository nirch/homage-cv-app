/******************************
 ***   TextFrameLabel.cpp   ***
 ******************************/

#include "Utime/GpTime.h"
#include "TextLabelOsx.h"




CTextLabelOsx::CTextLabelOsx()
{
    //m_bcolor = 0x00FF00;
    
    m_bcolor = 0x4e145c;
    m_color = 0xFF0000;

    
	m_transparent = 0.5;
    
    m_view = nil;
}



CTextLabelOsx::~CTextLabelOsx()
{
	

	if (m_textboxview != nil ){
		[m_textboxview release];
	}
	

	if (m_textview != nil ){
		[m_textview release];
	}
}



int  CTextLabelOsx::Init( char *file )
{
	return( 1 );

}

void CTextLabelOsx::SetFrameSize( int width, int height )
{
	m_width = width;
	m_height = height;


}

void CTextLabelOsx::SetBackgroundColor( int color, float transparent )
{
	m_bcolor = color;
	m_transparent = transparent;


}




int CTextLabelOsx::Init( NSView *drawOn,  int width, int height )
{





	// text box
	m_textboxview = [[NSView alloc] initWithFrame:CGRectMake(0.0, 0.0, width, height)];
	[m_textboxview setHidden:YES];
    
//    m_textboxview.backgroundColor = [ NSColor colorWithCalibratedRed:0.2f Green:0.25f blue:0.337f ];
    m_textboxview.AlphaValue = 0.5;
    
    [m_textboxview setFlipped:NO];
    
#ifdef _AA_
    
    CGFloat redbg = (m_bcolor >> 16) & 0xFF;
	CGFloat greenbg = (m_bcolor >> 8) & 0xFF;
	CGFloat bluebg = m_bcolor & 0xFF;
    
    m_textboxview.layer = [CALayer layer];
    [m_textboxview setWantsLayer:YES];
    
    CALayer *viewLayer = [CALayer layer];
    [viewLayer setBackgroundColor:[NSColor colorWithCalibratedRed:redbg/255.0 green:greenbg/255.0 blue:bluebg/255.0 alpha:m_transparent].CGColor];
       [m_textboxview.layer addSublayer:viewLayer];
   // [m_textboxview setWantsLayer:YES]; // view's backing store is using a Core Animation Layer
#endif



	// text view
	m_textview = [[NSTextView alloc] initWithFrame:CGRectMake(0.0, 0.0, width, height)];
    [m_textview setFont:[NSFont fontWithName:@"ArialMT"  size:16]];
    
	[m_textview setBackgroundColor:[NSColor clearColor]];
	[m_textview setHidden:YES];
    
   // [m_textview setFlipped:YES];
    
  //  [m_textview  setFrameRotation:180];

 //   [m_textview setFrameCenterRotation:180];
    
    

	[drawOn addSubview:m_textboxview];

	[m_textboxview addSubview:m_textview];
//	[drawOn addSubview:m_textview];




	return( 1 );
}



void CTextLabelOsx::SetVisible( int flag )
{
	m_show = flag;

	if( flag == 0 ){
		[m_textboxview setHidden:YES];
		return;
	}

	DrawTextBox();

	[m_textboxview setHidden:NO];
}


void
CTextLabelOsx::DrawTextBox ()
{
//	if( IsTextBoxActive( ) < 0 )
//		return;



//	[m_textboxview setHidden:NO];
	[m_textview setHidden:NO];
	[m_textview setString:@""];


	CGFloat red = (m_color >> 16) & 0xFF;
	CGFloat green = (m_color >> 8) & 0xFF;
	CGFloat blue = m_color & 0xFF;
	[m_textview setTextColor:[NSColor colorWithCalibratedRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:1.0]];
    
    
 //	CGFloat redbg = (m_bcolor >> 16) & 0xFF;
//	CGFloat greenbg = (m_bcolor >> 8) & 0xFF;
//	CGFloat bluebg = m_bcolor & 0xFF;
    
  
//    CALayer *viewLayer = [CALayer layer];
//    [viewLayer setBackgroundColor:[NSColor colorWithCalibratedRed:redbg/255.0 green:greenbg/255.0 blue:bluebg/255.0 alpha:m_transparent].CGColor];
//    [m_textboxview setWantsLayer:YES]; // view's backing store is using a Core Animation Layer
 //   [m_textboxview setLayer:viewLayer];



	if( m_iText * 10 > m_width )	m_iText = 0;
	int	x;
	x = 10 + m_iText*m_motion*10;
	m_iText++;

	[m_textview setFrame:CGRectMake(m_textview.frame.origin.x, m_textview.frame.origin.y, m_textview.frame.size.width, m_textview.frame.size.height)];
	[m_textview setString:@""];

	NSString *str = @"";

	const char *line;
	int	i;
	for( i = 0 ; i < m_nText ; i++ ){
		line = m_aText[i];
		str = [str stringByAppendingString:[NSString stringWithFormat:@"%@\n", [NSString stringWithCString:line encoding:NSUTF8StringEncoding]]];
	}
	[m_textview setString:str];

}




int CTextLabelOsx::HandleLButtonUp(u_int nFlags, int x, int y )
{

//	x -= m_x0;
//	y -= m_y0;




	return( -1 );
}




