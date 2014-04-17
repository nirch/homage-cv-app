//
//  CameraControllerIOS.h
//  MyChic
//
//  Created by Oren Shtark on 10/1/12.
//  Copyright (c) 2012 Oren Shtark. All rights reserved.
//

#ifndef _CAMERA_CONTROLLER_IOS__
#define _CAMERA_CONTROLLER_IOS__

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface CameraControllerIOS : UIImagePickerController <AVCaptureVideoDataOutputSampleBufferDelegate>{
    AVCaptureSession *captureSession;
    //UIImage *lastFrame;
    CGImageRef lastFrame;
    NSData *lastFrameData;
    BOOL resolutionChanged;
    
    CGSize imageSize;
}



@property (nonatomic, retain) AVCaptureSession *captureSession;
@property (retain) AVCaptureVideoPreviewLayer *previewLayer;
//@property (readonly) UIImage *lastFrame;
@property (readonly) CGImageRef lastFrame;
@property (readonly) NSData *lastFrameData;
@property (nonatomic, assign) int Resolution;
@property (readonly) CGSize imageSize;
@property (nonatomic,retain) AVCaptureDeviceInput *videoInput;
@property (nonatomic,retain) AVCaptureDevice *device;

- (void)setupCaptureSession;
- (void)setupCaptureSessionWithPreviewLayer:(CGRect)layerRect;
- (void)addVideoPreviewLayer: (CGRect) layerRect;
- (void)setPreset: (int) preset;
- (void) SetAutoFocusAtPoint:(CGPoint)point;
- (void) SetContinuousFocusAtPoint:(CGPoint)point;
- (int) isAdjustingFocus;

- (void) SetAutoExposureAtPoint:(CGPoint)point;
- (void) SetContinuousExposureAtPoint:(CGPoint)point;
- (int) isAdjustingExposure;

- (void) SetContinuousWhiteBalance;
- (void) SetWhiteBalanceLock;

@end

#endif
