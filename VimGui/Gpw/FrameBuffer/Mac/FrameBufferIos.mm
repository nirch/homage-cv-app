/*****************************
 ***   FrameBufferMac.cpp   ***
 *****************************/

#include "Utime/GpTime.h"
#include "FrameBufferIos.h"
#include "Vtool/Vtool.h"




CFrameBufferIos::CFrameBufferIos()
{
    m_view = nil;
    m_b.x0 = m_b.x1 = -1;
    m_b.y0 = m_b.y1 = -1;
    
}

CFrameBufferIos::~CFrameBufferIos()
{
	if( m_view != nil ) { [m_view release]; }
}



void CFrameBufferIos::AddSubview(  UIView *view )
{
 

	if( m_view == nil ){
        
        int width = view.frame.size.width;
        int height = view.frame.size.height;
        
        m_view = [[UIImageView alloc] initWithFrame:CGRectMake(0.0, 0.0, width, height )];
        
         
        [view addSubview:m_view];
        
    }
}
        
        
        
void
CFrameBufferIos::Draw ( UIView *view, int x, int y )
{
	if( m_view == nil ){
        return;
	}
    
    
    if (m_view.superview != view){
        [view addSubview:m_view];
    }
    
    //[m_view setFrame:CGRectMake(x, y, m_view.frame.size.width, m_view.frame.size.height)];
    [m_view setFrame:CGRectMake(x, y, view.frame.size.width, view.frame.size.height)];
    
    this->CreateImageFromData();

}


image_type *	CFrameBufferIos::Create( int width, int height )
{
    int depth = 4;
    
    if( m_image == NULL ){
         m_image = image_recreate( m_image, height, width, depth, 1);
        return( m_image );
    }

    
	if( m_view == nil ){
       m_view = [[UIImageView alloc] initWithFrame:CGRectMake(0.0, 0.0, width, height)];
        
		m_image = image_create( height, width, depth, 1, NULL );
        
		return( m_image );
	}
    
    
	if ((m_view.frame.size.width == width) && (m_view.frame.size.height == height)){
        return m_image;
    }
    

    m_image = image_recreate( m_image, height, width, depth, 1);
    
	return( m_image );
}





void CFrameBufferIos::CreateImageFromData()
{
//    UIImage* newImage = CFrameBufferIos::createImageFromImageType(m_image);
    UIImage* newImage;
    
    if( m_b.x0 >= 0 ){
        image_type *im = image_crop( m_image, m_b.x0, m_b.y0, m_b.x1 - m_b.x0, m_b.y1-m_b.y0, NULL );
        
  //      image_type *im = image_resize_bilinearB( m_image, &m_b,
  //                      m_view.frame.size.width, m_view.frame.size.height, NULL );
        newImage = CVtool::CreateUIImage(im );
        image_destroy( im, 1 );
    }
    else
        newImage = CVtool::CreateUIImage(m_image );
    
    //NSLog(@"%.1f, %.1f", newImage.size.width, newImage.size.height);
    if (newImage){
        [m_view setContentMode:UIViewContentModeScaleToFill];
        [m_view setClipsToBounds:YES];
        [m_view setImage:newImage];
    }
    
    
}

void CFrameBufferIos::Fill()
{
    CreateImageFromData();
}



void CFrameBufferIos::SetBox( box2i_type *b )
{
    if( m_b.x0 == b->x0 && m_b.x1 == b->x1 && m_b.y0 == b->y0 && m_b.y1 == b->y1 )
        return;
    
    
    m_b = *b;
    
    CreateImageFromData();
}



int CFrameBufferIos::HandleLButtonUp(u_int nFlags, int x, int y )
{
    

    
    
	return( -1 );
}





#ifdef _AA_

UIImage* CFrameBufferIos::createImageFromImageType(image_type *imageData)
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
#endif
