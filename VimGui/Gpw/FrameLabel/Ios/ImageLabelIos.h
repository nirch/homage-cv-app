// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FIMAGE_LABEL_IOS_
#define _FIMAGE_LABEL_IOS_

#include "../ImageLabel.h"

#import <UIKit/UIKit.h>


class CImageLabelIos : public CImageLabel
{
public:
    CImageLabelIos( void *w );

    virtual ~CImageLabelIos();

    virtual int Init( char *file );
    
    virtual int  Init( char *file, int x, int y );
   
    
	virtual int Draw(  void *dc, int wx, int wy  );
    
    void SetPose( int x, int y );


    
	void SetVisible( int flag );

	void SetDrag( int flag );


	int IsTouchBegin( void *view, int x, int y );

	int IsTouchMove( void *view, int x, int y );

	int IsTouchEnd( void *view, int x, int y );
    
     
protected:

	UIImageView *m_view;

    UIView *drawOn;

	int m_hint;

};




#endif