/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _VTOOL_
#define _VTOOL_





#import <UIKit/UIKit.h>

#import     "../FrameLabel/FrameLabelMac.h"
#import     "../FrameLabel/Ios/ImageLabelIos.h"


#include	"Uigp/igp.h"



struct image_type;




class CVtool
{
public:
	CVtool();
	virtual ~CVtool();



    
    static int SavePng( image_type *sim );

    
    static int SavePng( UIImage* imgTaken );
    
    
    static image_type *  LoadPng( char *file );
    
    
    static UIImage* CreateUIImage( image_type *im );
    
    UIImage* CreateUIImage( CVPixelBufferRef pixelBuffer );
    
    static image_type * DecomposeUIimage( UIImage* image );
    
    static CFrameLabelMac* CreateFrameLabel( UIView *view, NSBundle *bundle,
                                          NSString *name, int fx, int fy );
    static CFrameLabelMac* CreateFrameLabelDouble( UIView *view, NSBundle *bundle,
                                           NSString *imageName1, NSString *imageName2, int fx, int fy );

    
    static CImageLabelIos * CreateImageLabel( UIView *view, NSBundle *bundle,
                                            NSString *name, int fx, int fy );
    
    static UIColor * CreateUIColor( int color, float alpha );
    
    const char* GetSessionID(  char *id);
    void clearSessionID();
    
    
    static CVPixelBufferRef CVPixelBufferRef_from_image( image_type *im );
    static image_type * CVPixelBufferRef_to_image4( CVPixelBufferRef pixelBuffer, image_type *im );
    
    static image_type *CVPixelBufferRef_to_image( CVPixelBufferRef pixelBuffer, image_type *im );
    
    static image_type * CVPixelBufferRef_to_image_sample2( CVPixelBufferRef pixelBuffer, image_type *im );
    
    static image_type * CVPixelBufferRef_to_image_crop( CVPixelBufferRef pixelBuffer,
                                                int x0, int y0, int width, int height, image_type *im );
    
    
    static void CVPixelBufferRef_rotate180( CVPixelBufferRef pixelBuffer );
    

protected:

private:
    NSString *sessionID;
};

#endif




