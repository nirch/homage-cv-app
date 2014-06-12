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


CVtool::CVtool()
{
    sessionID = nil;


}


CVtool::~CVtool()
{
    if (sessionID){
        [sessionID release];
    }
}




int CVtool::SavePng( image_type *sim )
{
    image_type *im = image4_from( sim, NULL );
    
    UIImage* imgTaken = CreateUIImage( im );
    
    if( im != sim  )
    image_destroy( im, 1 );
    
    SavePng( imgTaken );
    
    return( 1 );
}



int CVtool::SavePng( UIImage* imgTaken )
{
    //UIImageWriteToSavedPhotosAlbum(imgTaken, nil, nil, nil);
    
    NSData *pngData = UIImagePNGRepresentation(imgTaken);
    UIImage  *img = [[UIImage alloc] initWithData: pngData];
    UIImageWriteToSavedPhotosAlbum(img, nil, nil, nil);
    
    NSLog(@"%f, %f", imgTaken.size.width, imgTaken.size.height);
    
    [img release];
    
    return( 1 );
}


image_type *  CVtool::LoadPng( char *file )
{
	//m_drag = drag;
    
    NSString *filename = [NSString stringWithUTF8String:file];
    UIImage *image = [UIImage imageWithContentsOfFile:filename];
    

    image_type *im =  DecomposeUIimage( image );
    
	return( im );
    
}




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


CFrameLabelMac * CVtool::CreateFrameLabel( UIView *view, NSBundle *bundle, NSString *name,
                                    int fx, int fy )
{
    
    CFrameLabelMac  *fl  = new CFrameLabelMac( view );
    

    NSString *path = [bundle  pathForResource:name ofType:@"png"];

   
    fl->Init( fx, fx, (char*)path.UTF8String );
                      

    
    return( fl );
}


CImageLabelIos * CVtool::CreateImageLabel( UIView *view, NSBundle *bundle, NSString *name,
                                          int fx, int fy )
{
    
    CImageLabelIos  *fl  = new CImageLabelIos( view );
    
    
    NSString *path = [bundle  pathForResource:name ofType:@"png"];
    
    
    fl->Init(  (char*)path.UTF8String, fx, fx);
    
    
    
    return( fl );
}


UIColor * CVtool::CreateUIColor( int color, float alpha )
{
  	CGFloat red = (color >> 16) & 0xFF;
	CGFloat green = (color >> 8) & 0xFF;
	CGFloat blue = color & 0xFF;
    
	UIColor *ucolor = [UIColor colorWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:alpha];
    
	return( ucolor );
}



CVPixelBufferRef
CVtool::CVPixelBufferRef_from_image( image_type *im )
{
    int i,   j;
	CVPixelBufferRef pixelBuffer;
    
	CVPixelBufferCreate(kCFAllocatorSystemDefault, im->width, im->height, kCVPixelFormatType_32BGRA, NULL, &pixelBuffer);
    
	
	u_char *sp = im->data;
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0 );
	unsigned char *buffer = (u_char *)CVPixelBufferGetBaseAddress(pixelBuffer);
    int k;
	for( i = 0, k = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, sp += 3, k+= 4 ){
			buffer[k] = sp[0];   // R
			buffer[k+1] = sp[1]; // G
			buffer[k+2] = sp[2]; // B
			buffer[k+3] = 0;     // A
            
            
		}
	}
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer,0);

	
	return( pixelBuffer );
}



image_type *
CVtool::CVPixelBufferRef_to_image( CVPixelBufferRef pixelBuffer, image_type *im )
{
    int i,   j, k;
    
//	size_t bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer);
	size_t width = CVPixelBufferGetWidth(pixelBuffer);
	size_t height = CVPixelBufferGetHeight(pixelBuffer);
    
	im = image_realloc( im, width, height, 3, IMAGE_TYPE_U8, 1 );
	
	u_char *tp = im->data;
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0 );
	unsigned char *buffer = (u_char *)CVPixelBufferGetBaseAddress(pixelBuffer);
	for( i = 0, k = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, tp += 3, k += 4 ){
            
            tp[0] = buffer[k];	// R
            tp[1] = buffer[k+1];	// G
            tp[2] = buffer[k+2]; 	// B
//            sp[3] = buffer[k+3];	// A
            
            
		}
	}
	
    CVPixelBufferUnlockBaseAddress(pixelBuffer,0);

	return( im );
}


image_type *
CVtool::CVPixelBufferRef_to_image_sample2( CVPixelBufferRef pixelBuffer, image_type *im )
{
    
    image_type *tim = CVPixelBufferRef_to_image( pixelBuffer, NULL );
	
    
    im = image_sample2( tim, im );
    
    image_destroy( tim, 1 );
    
	return( im );
}

image_type *
CVtool::CVPixelBufferRef_to_image_crop( CVPixelBufferRef pixelBuffer,
                        int x0, int y0, int width, int height, image_type *im )
{
 
    image_type *tim = CVPixelBufferRef_to_image( pixelBuffer, NULL );
	

    im = image_crop( tim, x0, y0, width, height, im );
    
    
    image_destroy( tim, 1 );
    
    
	return( im );
}

