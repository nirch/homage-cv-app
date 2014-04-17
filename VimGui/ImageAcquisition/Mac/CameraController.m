//
//  CameraController.m
//  ImageAcquisition
//
//  Created by Oren Shtark on 4/28/13.
//
//

#import "CameraController.h"

@implementation CameraController

//@synthesize captureSession;

- (id)init{
    if (self = [super init]){
        
    }
    return self;
}

- (CGImageRef)lastFrame{    
    return nil;
}

- (void)setupCaptureSession{
    
}

- (void)addVideoPreviewLayer: (CGRect) layerRect{
    
}

- (void)setPreset: (int) preset{
    
}

- (void) SetAutoFocusAtPoint:(CGPoint)point{
    
}

- (void) SetContinuousFocusAtPoint:(CGPoint)point{
    
}

- (int) isAdjustingFocus{
    return 1;
}

@end
