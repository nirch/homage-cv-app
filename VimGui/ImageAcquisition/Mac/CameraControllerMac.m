//
//  CameraControllerMac.m
//  ImageAcquisition
//
//  Created by Oren Shtark on 4/28/13.
//
//

#import "CameraControllerMac.h"

@implementation CameraControllerMac

@synthesize captureSession;

- (id)init{
    if (self = [super init]){
        
    }
    return self;
}



- (CGImageRef)lastFrame{
    @synchronized(self){
        CGImageRef temp = lastFrame;
        
        return CGImageCreateCopy(temp);
    }
    
    return nil;
}

// Create and configure a capture session and start it running
- (void)setupCaptureSession
{
    NSError *error = nil;
    
    // Create the session
    QTCaptureSession *session = [[QTCaptureSession alloc] init];
    [self setCaptureSession:session];
    
    // Configure the session to produce lower resolution video frames, if your
    // processing algorithm can cope. We'll specify medium quality for the
    // chosen device.
    
    
    
    
    // set resolution
    [self setPreset:[ self Resolution ] ];
    
    
    
    
    
    
    // Find a suitable AVCaptureDevice
    QTCaptureDevice *device = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
    //[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo];
    //[device isConnected]
    
    /*if( [self cameraDevice] == UIImagePickerControllerCameraDeviceRear )
        device = [ self cameraWithPosition:AVCaptureDevicePositionBack ];
    else
        device = [ self cameraWithPosition:AVCaptureDevicePositionFront ];
    */
    
    if (!device) { return; }
    
    bool success = [device open:&error];
    
    //[self setDevice:device];
    
    // Create a device input with the device and add it to the session.
    //QTCaptureDeviceInput *input = [QTCaptureDeviceInput deviceInputWithDevice:device];
    QTCaptureDeviceInput *input = [[[QTCaptureDeviceInput alloc] initWithDevice:device] autorelease];
    
    if (!input) {
        // Handling the error appropriately.
    }
    [session addInput:input error:&error];
    
    if (error){ }
    
    //[self setVideoInput:input];
    
    // Create a VideoDataOutput and add it to the session
    //QTCaptureVideoDataOutput *output = [[[QTCaptureVideoDataOutput alloc] init] autorelease];
    QTCaptureDecompressedVideoOutput *output = [[[QTCaptureDecompressedVideoOutput alloc] init] autorelease];
    [output setDelegate:self];
    [session addOutput:output error:&error];
    if (error){ }
    
    // Configure your output.
    /*dispatch_queue_t queue = dispatch_queue_create("myQueue", NULL);
    [output setSampleBufferDelegate:self queue:queue];
    dispatch_release(queue);
    */
    
    // Specify the pixel format
    /*output.videoSettings =
    [NSDictionary dictionaryWithObject:
     [NSNumber numberWithInt:kCVPixelFormatType_32BGRA]
                                forKey:(id)kCVPixelBufferPixelFormatTypeKey];
    */
    output.pixelBufferAttributes =
    //[NSDictionary dictionaryWithObject:
     //[NSNumber numberWithInt:kCVPixelFormatType_32BGRA]
       //                         forKey:(id)kCVPixelBufferPixelFormatTypeKey];
    [NSDictionary dictionaryWithObjectsAndKeys:
     [NSNumber numberWithDouble:640.0], (id)kCVPixelBufferWidthKey,
     [NSNumber numberWithDouble:480.0], (id)kCVPixelBufferHeightKey,
     [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA], (id)kCVPixelBufferPixelFormatTypeKey,
     nil];
    
/*    [output setPixelBufferAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
                                       [NSNumber numberWithDouble:640.0], (id)kCVPixelBufferWidthKey,
                                       [NSNumber numberWithDouble:480.0], (id)kCVPixelBufferHeightKey,
                                       [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32RGBA], (id)kCVPixelBufferPixelFormatTypeKey,
                                       nil]];
*/
    
    // If you wish to cap the frame rate to a known value, such as 15 fps, set
    // minFrameDuration.
    //output.minFrameDuration = CMTimeMake(1, 15);
    
    // Start the session running to start the flow of data
    [session startRunning];
    
    // Assign session to an ivar.
    // [self setCaptureSession:session];
}

- (void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection{

    CGImageRef image = [self imageFromVideoFrame:videoFrame];
    CGImageRef temp;
    
    @synchronized (self) {
        temp = lastFrame;
        lastFrame = image;
    }
    
    if (temp) {
        CGImageRelease(temp);
        temp = nil;
    }
    
    /*
    CVImageBufferRef imageBufferToRelease;
    
    CVBufferRetain(videoFrame);
    
    @synchronized (self) {
        imageBufferToRelease = mCurrentImageBuffer;
        mCurrentImageBuffer = videoFrame;
    }
    CVBufferRelease(imageBufferToRelease);
     */
}

// Create a UIImage from sample buffer data
- (CGImageRef) imageFromVideoFrame:(CVImageBufferRef) imageBuffer
{
    // Get a CMSampleBuffer's Core Video image buffer for the media data
    //CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
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

- (void)addVideoPreviewLayer: (CGRect) layerRect{
    
}

- (void)setPreset: (int) preset{
    [self setResolution:preset];
}

- (void) SetAutoFocusAtPoint:(CGPoint)point{
    
}

- (void) SetContinuousFocusAtPoint:(CGPoint)point{
    
}

- (int) isAdjustingFocus{
    return 1;
}

- (void)dealloc{
    if (captureSession){
        [captureSession stopRunning];
        [captureSession release];
    }
    if (lastFrame) { CGImageRelease(lastFrame); }
    
    [super dealloc];
}

@end
