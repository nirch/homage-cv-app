// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAMEBUFFER_IOS_
#define _FRAMEBUFFER_IOS_




#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "../FrameBuffer.h"

#include "Ubox/Box2d.h"
#include "ImageType/ImageType.h"



#define TEXT_LEN 512



class CFrameBufferIos  : public CFrameBuffer
{
public:
    CFrameBufferIos();
    virtual ~CFrameBufferIos();
    
    
public:
    
    UIImageView *m_view;
    
    void AddSubview(  UIView *view );

    
    void	Draw ( UIView *view, int x, int y );
    
    
    // frame buffer
    struct image_type *	Create( int width, int height );
    
    void	Fill();
    
    void SetBox( box2i_type *b );
    
    

    
    int HandleLButtonUp(u_int nFlags, int x, int y );
    

private:
    void CreateImageFromData();

public:
 //   static UIImage* createImageFromImageType(image_type *imageData);
    
public:
    
    box2i_type m_b;

    
};




#endif