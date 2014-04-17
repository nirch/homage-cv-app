// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TEXT_LABEL_IOS_
#define _TEXT_LABEL_IOS_

#import <UIKit/UIKit.h>

#include "UVec/Vec2d.h"
#include "Ubox/Box2d.h"

#include "TextLabel.h"



#define TEXT_LEN 512

class CTextLabelIos: public CTextLabel
{
public:
		CTextLabelIos();

		virtual ~CTextLabelIos();


		void SetFrameSize( int width, int height );
		void SetBackgroundColor( int color, float transparent );



		void SetShow( int flag );

		int HandleLButtonUp(u_int nFlags, int x, int y );



public:

	virtual int Init( char *file );

    virtual int Init( UIView *drawOn,  int width, int height );


	void	DrawTextBox ();



private:
	UIView		*m_textboxview;

	UITextView	*m_textview;

	UIButton	*m_hidetextbutton;
};




#endif