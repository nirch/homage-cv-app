//
//  CameraController.h
//  ImageAcquisition
//
//  Created by Oren Shtark on 4/28/13.
//
//

#import <QuartzCore/QuartzCore.h>
#import <Foundation/Foundation.h>

@interface CameraController : NSObject{
    CGImageRef lastFrame;
    BOOL resolutionChanged;
}

//@property (retain) AVCaptureVideoPreviewLayer *previewLayer;
@property (readonly) CGImageRef lastFrame;
@property (nonatomic, assign) int Resolution;
//@property (nonatomic,retain) AVCaptureDeviceInput *videoInput;
//@property (nonatomic,retain) AVCaptureDevice *device;

- (void)setupCaptureSession;
- (void)addVideoPreviewLayer: (CGRect) layerRect;
- (void)setPreset: (int) preset;
- (void) SetAutoFocusAtPoint:(CGPoint)point;
- (void) SetContinuousFocusAtPoint:(CGPoint)point;
- (int) isAdjustingFocus;

@end
