//
//  CameraControllerMac.h
//  MyChic
//
//  Created by Oren Shtark on 10/1/12.
//  Copyright (c) 2012 Oren Shtark. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface CameraControllerMac : UIImagePickerController <AVCaptureVideoDataOutputSampleBufferDelegate>{
    AVCaptureSession *captureSession;
    UIImage *lastFrame;
    NSData *lastFrameData;
    BOOL resolutionChanged;
    
    CGSize imageSize;
}



@property (nonatomic, retain) AVCaptureSession *captureSession;
@property (retain) AVCaptureVideoPreviewLayer *previewLayer;
@property (readonly) UIImage *lastFrame;
@property (readonly) NSData *lastFrameData;
@property (nonatomic, assign) int Resolution;
@property (readonly) CGSize imageSize;
@property (nonatomic,retain) AVCaptureDeviceInput *videoInput;
@property (nonatomic,retain) AVCaptureDevice *device;

- (void)setupCaptureSession;
- (void)addVideoPreviewLayer: (CGRect) layerRect;
- (void)setPreset: (int) preset;
- (void) SetAutoFocusAtPoint:(CGPoint)point;
- (void) SetContinuousFocusAtPoint:(CGPoint)point;
- (int) isAdjustingFocus;

@end
