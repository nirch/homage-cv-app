//
//  FrameLabem_tLabel.h
//  Bw
//
//  Created by Oren Shtark on 7/18/13.
//  Copyright (c) 2013 Yoram Elichai. All rights reserved.
//

#ifndef _TEXT_LABEL_IOS_
#define _TEXT_LABEL_IOS_



#import <UIKit/UIKit.h>
#include "TextLabel.h"


class CTextLabelIos : public CTextLabel
{
public:
    CTextLabelIos(void* w);
    ~CTextLabelIos();
    
 
    void Init(int x, int y, int width, int height, const char *text);
    
    void SetBackground( int color, float alpha );
    void SetBackground(int red, int green, int blue, float alpha);
    void SetBackground( UIColor *color);
    void SetBackgroundAlpha(float alpha);
    
    void SetText(char *text);
    void SetTextColor(int color, float alpha);
    void SetTextColor(int red, int green, int blue, float alpha);
    void SetTextColor(UIColor *color);
    void SetTextAlpha(float alpha);

	UIColor *Create_UIColor( int color, float alpha );
  
private:
	UIView *m_drawOn;

    UIImageView *m_view;
    
    UIView  *m_bView;
    UILabel *m_tLabel;
};

#endif /* defined(__Bw__FrameLabem_tLabel__) */
