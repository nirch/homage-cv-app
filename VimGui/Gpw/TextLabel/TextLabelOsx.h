// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TEXT_LABEL_OSX_
#define _TEXT_LABEL_OSX_

#import <Foundation/Foundation.h>
#include <AppKit/AppKit.h>

#include "UVec/Vec2d.h"
#include "Ubox/Box2d.h"

#include "TextLabel.h"



#define TEXT_LEN 512

class CTextLabelOsx: public CTextLabel
{
public:
		CTextLabelOsx();

		virtual ~CTextLabelOsx();


		void SetFrameSize( int width, int height );
		void SetBackgroundColor( int color, float transparent );



		void SetVisible( int flag );

		int HandleLButtonUp(u_int nFlags, int x, int y );



public:

	virtual int Init( char *file );

    virtual int Init( NSView *drawOn,  int width, int height );


	void	DrawTextBox ();



private:
	NSView		*m_textboxview;

	NSTextView	*m_textview;

	NSButton	*m_hidetextbutton;
};




#endif