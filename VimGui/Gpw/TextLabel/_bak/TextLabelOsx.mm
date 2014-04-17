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

    
	m_transparency = 0.5;
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

	// create text box
//	NSImage *headerDecImg = [NSImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"header_decoration@2x" ofType:@"png"]];
//	NSImage *logoImg = [NSImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"logo@2x" ofType:@"png"]];



	// text box
	m_textboxview = [[NSView alloc] initWithFrame:CGRectMake(0.0, 0.0, width, 55.0 + headerDecImg.size.height)];
	[m_textboxview setHidden:YES];

	NSImageView *headerDecImgView = [[[NSImageView alloc] initWithImage:headerDecImg] autorelease];
	[headerDecImgView setFrame:CGRectMake(0.0, m_textboxview.frame.size.height - headerDecImg.size.height, headerDecImg.size.width, headerDecImg.size.height)];
	[m_textboxview addSubview:headerDecImgView];


	NSImageView *logoImgView = [[[NSImageView alloc] initWithImage:logoImg] autorelease];
	[logoImgView setFrame:CGRectMake(5.0, 12.0, logoImg.size.width, logoImg.size.height)];
	[m_textboxview addSubview:logoImgView];


	// text view
	m_textview = [[NSTextView alloc] initWithFrame:CGRectMake(10.0 + logoImg.size.width, 10.0, width - logoImgView.frame.size.width - 50.0, 45.0)];
    [m_textview setFont:[NSFont fontWithName:@"ArialMT"  size:16]];
    
	[m_textview setBackgroundColor:[NSColor clearColor]];
	[m_textview setHidden:YES];
    
    
    
    



//	[m_textboxview addSubview:m_textview];
	[drawOn addSubview:m_textview];


	[drawOn addSubview:m_textboxview];

	return( 1 );
}



void CTextLabelOsx::SetShow( int flag )
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



	[m_textboxview setHidden:NO];
	[m_textview setHidden:NO];
	[m_textview setText:@""];


	CGFloat red = (m_color >> 16) & 0xFF;
	CGFloat green = (m_color >> 8) & 0xFF;
	CGFloat blue = m_color & 0xFF;
	[m_textview setTextColor:[NSColor colorWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:1.0]];

	CGFloat redbg = (m_bcolor >> 16) & 0xFF;
	CGFloat greenbg = (m_bcolor >> 8) & 0xFF;
	CGFloat bluebg = m_bcolor & 0xFF;
	[m_textboxview setBackgroundColor:[NSColor colorWithRed:redbg/255.0 green:greenbg/255.0 blue:bluebg/255.0 alpha:m_transparency]];



	if( m_iText * 10 > m_width )	m_iText = 0;
	int	x;
	x = 10 + m_iText*m_motion*10;
	m_iText++;

	[m_textview setFrame:CGRectMake(m_textview.frame.origin.x, m_textview.frame.origin.y, m_textview.frame.size.width, m_textview.frame.size.height)];
	[m_textview setText:@""];

	NSString *str = @"";

	const char *line;
	int	i;
	for( i = 0 ; i < m_nText ; i++ ){
		line = m_aText[i];
		str = [str stringByAppendingString:[NSString stringWithFormat:@"%@\n", [NSString stringWithCString:line encoding:NSUTF8StringEncoding]]];
	}
	[m_textview setText:str];

}




int CTextLabelOsx::HandleLButtonUp(u_int nFlags, int x, int y )
{

	x -= m_x0;
	y -= m_y0;




	return( -1 );
}




