//
//  FrameLabem_tLabel.cpp
//  Bw
//
//  Created by Oren Shtark on 7/18/13.
//  Copyright (c) 2013 Yoram Elichai. All rights reserved.
//

#include "TextLabelIos.h"

CTextLabelIos::CTextLabelIos(void *w )
{

	m_drawOn = (UIView*)w;

    m_bcolor = 0x4e145c;
    m_color = 0xFF0000;


    m_bView = [[UIView alloc] init];
    [m_bView setAlpha:0.5];

    m_tLabel = [[UILabel alloc] init];
    [m_tLabel setBackgroundColor:[UIColor clearColor]];
    [m_tLabel setAdjustsFontSizeToFitWidth:YES];
    [m_tLabel setFont:[UIFont systemFontOfSize:25]];
    [m_tLabel setTextAlignment:NSTextAlignmentCenter];
    
}

CTextLabelIos::~CTextLabelIos(){
    [m_tLabel release];
    [m_bView release];
}



void CTextLabelIos::Init(int x, int y, int width, int height, const char *text)
{
    if (m_view){
        [m_view release];
    }
    
    CGRect frame = CGRectMake(x, y, width, height);
    m_view = [[UIImageView alloc] initWithFrame:frame];
    [m_view setBackgroundColor:[UIColor clearColor]];
    [m_drawOn addSubview:m_view];
    
    [m_bView setFrame:CGRectMake(0, 0, width, height)];
    [m_tLabel setFrame:CGRectMake(3.0, 3.0, width - 6.0, height - 6.0)];
    [m_view addSubview:m_bView];
    [m_view addSubview:m_tLabel];
    
    [m_tLabel setText:[NSString stringWithUTF8String:text]];
}


void CTextLabelIos::SetBackground( int color, float alpha )
{
	m_bcolor = color;
	UIColor *ucolor = Create_UIColor( m_bcolor, alpha );
//    [m_bView setBackgroundColor:[UIColor colorWithRed:red/255.0
//green:green/255.0 blue:blue/255.0 alpha:alpha]];
    [m_bView setBackgroundColor:ucolor];
    
}

void CTextLabelIos::SetBackground(UIColor *color)
{
    [m_bView setBackgroundColor:color];
}

void CTextLabelIos::SetBackgroundAlpha(float alpha)
{
    [m_bView setAlpha:alpha];
}



void CTextLabelIos::SetText(char *text)
{
    [m_tLabel setText:[NSString stringWithUTF8String:text]];
}

void CTextLabelIos::SetTextColor( int color, float alpha)
{
    [m_tLabel setTextColor:Create_UIColor( m_bcolor, alpha ) ];
  }

void CTextLabelIos::SetTextColor(int red, int green, int blue, float alpha)
{
    [m_tLabel setTextColor:[UIColor colorWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:alpha]];
}

void CTextLabelIos::SetTextColor(UIColor *color)
{
    [m_tLabel setTextColor:color];
}

void CTextLabelIos::SetTextAlpha(float alpha)
{
    [m_tLabel setAlpha:alpha];
}


UIColor * CTextLabelIos::Create_UIColor( int color, float alpha )
{
  	CGFloat red = (m_color >> 16) & 0xFF;
	CGFloat green = (m_color >> 8) & 0xFF;
	CGFloat blue = m_color & 0xFF;

	UIColor *ucolor = [UIColor colorWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:alpha];

	return( ucolor );
}
