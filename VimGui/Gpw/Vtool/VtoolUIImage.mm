/**********************************
 ***   Vtool.mm   ***
 **********************************/
#include	<string.h>

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"
#include	"Utime/GpTime.h"
#include	"ImageType/ImageType.h"

#import     "../FrameLabel/FrameLabelMac.h"
#import     "../FrameLabel/Ios/ImageLabelIos.h"
#include	"Vtool.h"







UIImage* CVtool::CreateUIImage(image_type *imageData)
{
    unsigned char *pixels        = imageData->data;
    int size = imageData->width * imageData->height * 4;
    
    size_t width                    = imageData->width;
    size_t height                   = imageData->height;
    size_t bitsPerComponent         = 8;
    size_t bitsPerPixel             = 32;
    size_t bytesPerRow              = imageData->width * 4;
    
    CGColorSpaceRef colorspace      = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo         = kCGBitmapByteOrderDefault;
    
    NSData* newPixelData = [NSData dataWithBytes:pixels length:size];
    CFDataRef imgData = (CFDataRef)newPixelData;
    CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData(imgData);
    
    CGImageRef newImageRef = CGImageCreate (
                                            width,
                                            height,
                                            bitsPerComponent,
                                            bitsPerPixel,
                                            bytesPerRow,
                                            colorspace,
                                            bitmapInfo,
                                            imgDataProvider,
                                            NULL,
                                            NO,
                                            kCGRenderingIntentDefault
                                            );
    
    UIImage *newImage   = [[UIImage alloc] initWithCGImage:newImageRef];//[UIImage imageWithCGImage:newImageRef];
    
    CGColorSpaceRelease(colorspace);
    CGDataProviderRelease(imgDataProvider);
    CGImageRelease(newImageRef);
    
    return [newImage autorelease];
}















UIImage* CVtool::CreateUIImage( CVPixelBufferRef pixelBuffer )
{
    
    size_t width = CVPixelBufferGetWidth(pixelBuffer);
    size_t height = CVPixelBufferGetHeight(pixelBuffer);
    

    
    
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0 );
    unsigned char *pixels = (u_char *)CVPixelBufferGetBaseAddress(pixelBuffer);
    
    
    
    int size = (int)(width * height * 4);
    
 //   size_t width                    = width;
 //   size_t height                   = height;
    size_t bitsPerComponent         = 8;
    size_t bitsPerPixel             = 32;
    size_t bytesPerRow              = width * 4;
    
    CGColorSpaceRef colorspace      = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo         = kCGBitmapByteOrderDefault;
    
    NSData* newPixelData = [NSData dataWithBytes:pixels length:size];
    CFDataRef imgData = (CFDataRef)newPixelData;
    CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData(imgData);
    
    CGImageRef newImageRef = CGImageCreate (
                                            width,
                                            height,
                                            bitsPerComponent,
                                            bitsPerPixel,
                                            bytesPerRow,
                                            colorspace,
                                            bitmapInfo,
                                            imgDataProvider,
                                            NULL,
                                            NO,
                                            kCGRenderingIntentDefault
                                            );
    
    UIImage *newImage   = [[UIImage alloc] initWithCGImage:newImageRef];//[UIImage imageWithCGImage:newImageRef];
    
    CGColorSpaceRelease(colorspace);
    CGDataProviderRelease(imgDataProvider);
    CGImageRelease(newImageRef);
    
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer,0);

    
    return [newImage autorelease];
}







image_type * CVtool::DecomposeUIimage( UIImage* image )
{
    
    image_type *im;
    
    
    int width = image.size.width;
    int height = image.size.height;
    im = image_alloc( width, height, 4, IMAGE_TYPE_U8, 1 );
    
    
    
    
    CGImageRef imageRef = image.CGImage;
    
    size_t bit =  CGImageGetBitsPerPixel( imageRef );
    CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(imageRef));
    NSData *data      = (NSData *)dataRef;
    
    unsigned char *pixels        = (unsigned char *)[data bytes];
    
    memcpy(im->data, pixels, [data length]);
    
    CFRelease(dataRef);
    
    return( im );
}




image_type * CVtool::UIimage_to_image( UIImage* image, image_type *im )
{
    

    
    
    int width = image.size.width;
    int height = image.size.height;
    
    im = image_realloc( im, width, height, 4, IMAGE_TYPE_U8, 1 );
    
    
    
    
    CGImageRef imageRef = image.CGImage;
    
    size_t bit =  CGImageGetBitsPerPixel( imageRef );
    CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(imageRef));
    NSData *data      = (NSData *)dataRef;
    
    unsigned char *pixels        = (unsigned char *)[data bytes];
    
    memcpy(im->data, pixels, [data length]);
    
    CFRelease(dataRef);
    
    return( im );
}



