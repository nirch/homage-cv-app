//
//  CameraControllerIOS.m
//  MyChic
//
//  Created by Oren Shtark on 10/1/12.
//  Copyright (c) 2012 Oren Shtark. All rights reserved.
//

#import "CameraControllerIOS.h"
#import <objc/objc-sync.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>

@interface CameraControllerIOS ()

@end

@implementation CameraControllerIOS

@synthesize captureSession;
@synthesize imageSize;

- (id)init{
    if (self = [super init]){
        [self setSourceType:UIImagePickerControllerSourceTypeCamera];
        [self setCameraDevice:UIImagePickerControllerCameraDeviceFront];
        [self setAllowsEditing:NO];
        [self setShowsCameraControls:NO];
        [self setNavigationBarHidden:YES];
        [self setToolbarHidden:YES];
        [self setWantsFullScreenLayout:YES];
        [self setCameraViewTransform:CGAffineTransformScale([self cameraViewTransform], 1.26, 1.26)];
        
        [self setResolution:2];
        
        //[self setupCaptureSession];
        captureSession = nil;
        lastFrame = nil;
        lastFrameData = nil;
        resolutionChanged = NO;
            }
    
    return self;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (CGImageRef)lastFrame{
    @synchronized(self){
        //UIImage *retVal = [[lastFrame retain] autorelease];
        //UIImage *retVal = [lastFrame.copy autorelease];
        /*UIImage *retVal = lastFrame;
        return retVal;*/
        
        CGImageRef temp = lastFrame;
        
        return CGImageCreateCopy(temp);
    }
    
    return nil;
}

- (NSData*)lastFrameData{
    @synchronized(self){
        NSData *retVal = [lastFrameData.copy autorelease];
        return retVal;
    }
    
    return nil;
}


// Find a front facing camera, returning nil if one is not found
- (AVCaptureDevice *) frontFacingCamera
{
    return [self cameraWithPosition:AVCaptureDevicePositionFront];
}

// Find a back facing camera, returning nil if one is not found
- (AVCaptureDevice *) backFacingCamera
{
    return [self cameraWithPosition:AVCaptureDevicePositionBack];
}



- (AVCaptureDevice *) cameraWithPosition:(AVCaptureDevicePosition) position
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices) {
        if ([device position] == position) {
            return device;
        }
    }
    return nil;
}

- (void)setupCaptureSession{
    [self setupCaptureSessionWithPreviewLayer:CGRectMake(0.0, 0.0, 0.0, 0.0)];
}

// Create and configure a capture session and start it running
- (void)setupCaptureSessionWithPreviewLayer:(CGRect)layerRect
{
    NSError *error = nil;
    
    // Create the session
    AVCaptureSession *session = [[AVCaptureSession alloc] init];
    [self setCaptureSession:session];
    
    // Configure the session to produce lower resolution video frames, if your
    // processing algorithm can cope. We'll specify medium quality for the
    // chosen device.
    
    
    
    
    // set resolution
    [self setPreset:[ self Resolution ] ];
    

    
    
    
    
    // Find a suitable AVCaptureDevice
     AVCaptureDevice *device;
    
    if( [self cameraDevice] == UIImagePickerControllerCameraDeviceRear )
        device = [ self cameraWithPosition:AVCaptureDevicePositionBack ];
    else
        device = [ self cameraWithPosition:AVCaptureDevicePositionFront ];
    
    if (!device) { return; }
    
     [self setDevice:device];
    

    
    
    //AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    
    
    
    // Create a device input with the device and add it to the session.
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:device
                                                                        error:&error];
    
    if (!input) {
        // Handling the error appropriately.
    }
    [session addInput:input];
   
    [self setVideoInput:input];
    
 //   [self setvideoInput:[ self input ] ];
    
    // Create a VideoDataOutput and add it to the session
    AVCaptureVideoDataOutput *output = [[[AVCaptureVideoDataOutput alloc] init] autorelease];
    [session addOutput:output];
    
    // Configure your output.
    dispatch_queue_t queue = dispatch_queue_create("myQueue", NULL);
    [output setSampleBufferDelegate:self queue:queue];
    dispatch_release(queue);
    
    // Specify the pixel format
    output.videoSettings =
    [NSDictionary dictionaryWithObject:
     [NSNumber numberWithInt:kCVPixelFormatType_32BGRA]
                                forKey:(id)kCVPixelBufferPixelFormatTypeKey];
    
    
    if (layerRect.size.width != 0.0 && layerRect.size.height != 0.0){
        [self addVideoPreviewLayer:layerRect];
    }
    
    // If you wish to cap the frame rate to a known value, such as 15 fps, set
    // minFrameDuration.
    //output.minFrameDuration = CMTimeMake(1, 15);
    
    // Start the session running to start the flow of data
    [session startRunning];
    
    // Assign session to an ivar.
   // [self setCaptureSession:session];
}

// Delegate routine that is called when a sample buffer was written
- (void)captureOutput:(AVCaptureOutput *)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection
{
    //@synchronized(self){
    //objc_sync_enter(self);
        // Create a UIImage from the sample buffer data
        
        CGImageRef image = [self imageFromSampleBuffer:sampleBuffer];
    
        CGImageRef temp = lastFrame;
        lastFrame = image;
        if (temp) {
            CGImageRelease(temp);
            temp = nil;
        }
    
    
        /*
         //UIImage *image = [self imageFromSampleBuffer:sampleBuffer];
         
         if (lastFrame) {
            //[lastFrame release];
            //lastFrame = nil;
        }
        lastFrame = [image retain];*/
    
        /*NSData *imageData = [self dataFromSampleBuffer:sampleBuffer];
    
        if (lastFrameData){
            [lastFrameData release];
            lastFrameData = nil;
        }
        lastFrameData = [imageData retain];
    */
        //< Add your code here that uses the image >
    //objc_sync_exit(self);
    //}
    
}

// Create a UIImage from sample buffer data
- (CGImageRef) imageFromSampleBuffer:(CMSampleBufferRef) sampleBuffer
{
    // Get a CMSampleBuffer's Core Video image buffer for the media data
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    // Lock the base address of the pixel buffer
    CVPixelBufferLockBaseAddress(imageBuffer, 0);
    
    // Get the number of bytes per row for the pixel buffer
    void *baseAddress = CVPixelBufferGetBaseAddress(imageBuffer);
    
    // Get the number of bytes per row for the pixel buffer
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    // Get the pixel buffer width and height
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);
    
    // Create a device-dependent RGB color space
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    // Create a bitmap graphics context with the sample buffer data
    CGContextRef context = CGBitmapContextCreate(baseAddress, width, height, 8,
                                                 bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
    // Create a Quartz image from the pixel data in the bitmap graphics context
    CGImageRef quartzImage = CGBitmapContextCreateImage(context);
    // Unlock the pixel buffer
    CVPixelBufferUnlockBaseAddress(imageBuffer,0);
    
    // Free up the context and color space
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    
    // Create an image object from the Quartz image
    //UIImage *image = [UIImage imageWithCGImage:quartzImage];
    
    // Release the Quartz image
    //CGImageRelease(quartzImage);
    
    return (quartzImage);
}

-(NSData*) dataFromSampleBuffer:(CMSampleBufferRef)source {
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(source);
    CVPixelBufferLockBaseAddress(imageBuffer,0);
    
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);
    void *src_buff = CVPixelBufferGetBaseAddress(imageBuffer);
    
    imageSize = CGSizeMake(width, height);
    
    //NSData *data = [NSData dataWithBytes:src_buff length:bytesPerRow * height];
    //NSData *data = [[NSData alloc] initWithBytesNoCopy:src_buff length:bytesPerRow * height freeWhenDone:YES];
    NSData *data = [[[NSData alloc] initWithBytes:src_buff length:bytesPerRow * height] autorelease];
    
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
    return data;
}


- (void)addVideoPreviewLayer: (CGRect) layerRect {
    [self setPreviewLayer:[[[AVCaptureVideoPreviewLayer alloc] initWithSession:[self captureSession]] autorelease]];
    
    [[self previewLayer] setVideoGravity:AVLayerVideoGravityResizeAspectFill];
    
    
    [ [self previewLayer] setBounds:layerRect];
    [ [self previewLayer] setPosition:CGPointMake(CGRectGetMidX(layerRect),CGRectGetMidY(layerRect))];
}


- (void)setPreset: (int) preset {
    [self setResolution:preset];
    
    switch( preset ){
        case 0:
            captureSession.sessionPreset = AVCaptureSessionPresetLow;
            break;
        case 1:
            captureSession.sessionPreset = AVCaptureSessionPresetMedium;
            break;
        case 2:
            captureSession.sessionPreset = AVCaptureSessionPreset640x480;
            break;
        case 3:
            captureSession.sessionPreset = AVCaptureSessionPresetHigh;
            break;
        case 4:
            captureSession.sessionPreset = AVCaptureSessionPresetPhoto;
            break;
        case 5:
            captureSession.sessionPreset = AVCaptureSessionPreset1280x720;
            break;
        default:
            captureSession.sessionPreset = AVCaptureSessionPresetMedium;
            break;
    }
}

- (void)dealloc{
    if (captureSession){
        [captureSession stopRunning];
        [captureSession release];
    }
    //if (lastFrame) { [lastFrame release]; }
    if (lastFrame) { CGImageRelease(lastFrame); }
    if (lastFrameData) { [lastFrameData release]; }
    
    [super dealloc];
}



// Perform an auto focus at the specified point. The focus mode will automatically change to locked once the auto focus is complete.
- (void) SetAutoFocusAtPoint:(CGPoint)point
{
 //   AVCaptureDevice *device = [[self videoInput] device];
    AVCaptureDevice *device = [self device];

    
    if ([device isFocusPointOfInterestSupported] && [device isFocusModeSupported:AVCaptureFocusModeAutoFocus]) {
        NSError *error;
        if ([device lockForConfiguration:&error]) {
            [device setFocusPointOfInterest:point];
            [device setFocusMode:AVCaptureFocusModeAutoFocus];
            [device unlockForConfiguration];
        } else {
 //           if ([[self delegate] respondsToSelector:@selector(captureManager:didFailWithError:)]) {
 //               [[self delegate] captureManager:self didFailWithError:error];
  //          }
        }
    }
}

// Switch to continuous auto focus mode at the specified point
- (void) SetContinuousFocusAtPoint:(CGPoint)point
{
 //   AVCaptureDevice *device = [[self videoInput] device];
    AVCaptureDevice *device = [self device];

	
    if ([device isFocusPointOfInterestSupported] && [device isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]) {
		NSError *error;
		if ([device lockForConfiguration:&error]) {
			[device setFocusPointOfInterest:point];
			[device setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
			[device unlockForConfiguration];
		} else {
//			if ([[self delegate] respondsToSelector:@selector(captureManager:didFailWithError:)]) {
//               [[self delegate] captureManager:self didFailWithError:error];
//			}
		}
	}
}


- (int) isAdjustingFocus
{
    AVCaptureDevice *device = [self device];

    BOOL ret = [device isAdjustingFocus ];
    
    return( ret );
}



// Perform an auto focus at the specified point. The focus mode will automatically change to locked once the auto focus is complete.
- (void) SetAutoExposureAtPoint:(CGPoint)point
{
    //   AVCaptureDevice *device = [[self videoInput] device];
    AVCaptureDevice *device = [self device];
    
    
    if ([device isExposurePointOfInterestSupported] && [device isExposureModeSupported:AVCaptureExposureModeAutoExpose]) {
        NSError *error;
        if ([device lockForConfiguration:&error]) {
            [device setExposurePointOfInterest:point];
            [device setExposureMode:AVCaptureExposureModeAutoExpose];
            [device unlockForConfiguration];
        } else {
            //           if ([[self delegate] respondsToSelector:@selector(captureManager:didFailWithError:)]) {
            //               [[self delegate] captureManager:self didFailWithError:error];
            //          }
        }
    }
}

// Switch to continuous auto focus mode at the specified point
- (void) SetContinuousExposureAtPoint:(CGPoint)point
{
    //   AVCaptureDevice *device = [[self videoInput] device];
    AVCaptureDevice *device = [self device];
    
	
    if ([device isExposurePointOfInterestSupported] && [device isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure]) {
		NSError *error;
		if ([device lockForConfiguration:&error]) {
			[device setExposurePointOfInterest:point];
			[device setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
			[device unlockForConfiguration];
		} else {
            //			if ([[self delegate] respondsToSelector:@selector(captureManager:didFailWithError:)]) {
            //               [[self delegate] captureManager:self didFailWithError:error];
            //			}
		}
	}
}


- (int) isAdjustingExposure
{
    AVCaptureDevice *device = [self device];
    
    BOOL ret = [device isAdjustingFocus ];
    
    return( ret );
}



// Switch to continuous auto focus mode at the specified point
- (void) SetContinuousWhiteBalance
{
    //   AVCaptureDevice *device = [[self videoInput] device];
    AVCaptureDevice *device = [self device];
    
	
    if ([device isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance]) {
		NSError *error;
		if ([device lockForConfiguration:&error]) {
			[device setWhiteBalanceMode:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance];
			[device unlockForConfiguration];
		} else {
            //			if ([[self delegate] respondsToSelector:@selector(captureManager:didFailWithError:)]) {
            //               [[self delegate] captureManager:self didFailWithError:error];
            //			}
		}
	}
}

// Switch to continuous auto focus mode at the specified point
- (void) SetWhiteBalanceLock
{
    //   AVCaptureDevice *device = [[self videoInput] device];
    AVCaptureDevice *device = [self device];
    
	
    if ([device isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeLocked]) {
		NSError *error;
		if ([device lockForConfiguration:&error]) {
			[device setWhiteBalanceMode:AVCaptureWhiteBalanceModeLocked];
			[device unlockForConfiguration];
		} else {
            //			if ([[self delegate] respondsToSelector:@selector(captureManager:didFailWithError:)]) {
            //               [[self delegate] captureManager:self didFailWithError:error];
            //			}
		}
	}
}



@end
