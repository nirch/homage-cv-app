// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAME_LABEL_MAC_
#define _FRAME_LABEL_MAC_

#include "FrameLabel.h"

#import <UIKit/UIKit.h>


class CFrameLabelMac : public CFrameLabel
{
public:
    CFrameLabelMac( void *w );

    virtual ~CFrameLabelMac();

    virtual int Init( char *file );
    
    virtual int  Init( int x, int y, char *file );
   
    
	virtual int Draw();
    
    void SetPose( float x, float y );


  //void SetMove( int x, int y );
    
//	void GetPose( int *x, int *y );
    
    
	void SetShow( int flag );
	void SetDrag( int flag );
    
    UIImageView *img;
    
protected:
    UIView *drawOn;

};




#endif