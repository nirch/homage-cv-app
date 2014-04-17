/**********************************
 ***   CameraAcquisition.cpp   ***
 **********************************/
#include	<string.h>

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"
#include	"Utime/GpTime.h"
#include	"ImageType/ImageType.h"

#include	"CameraAcquisitionMac.h"




static image_type *    image3_iphone_camera( image_type *sim, int width, int height, image_type *im );

static image_type *image3_iphone_camera_1( image_type *sim, image_type *im );

static image_type * image3_iphone_camera_1( image_type *sim, int width, int height, image_type *im );


static image_type * image3_iphone_camera_1( image_type *sim, int x0, int y0,
                                           int width, int height, image_type *im );




#ifdef  __APPLE__
CImageAcquisition *CreateCameraAcquisition()
{
	CImageAcquisition	*vc;

	vc = new CCameraAcquisitionMac();

	return( vc );
}
#endif


CCameraAcquisitionMac::CCameraAcquisitionMac()
{


	m_image = NULL;
    temp_img = NULL;
    
	m_acq = 1;


	m_active = 1;
    
    m_resolution = 1;       // default 480x360
    //m_resolution = 2;       // default 640x480

	m_rWidth = -1;//320;
	m_rHeight = -1;//480;
    
    m_protrait = 1;
    
    m_width = m_rWidth;
    m_height = m_rHeight;
    
    m_cameraOpen = false;
    m_cWidth = m_cHeight = -1;
    
    camera = [[CameraControllerMac alloc] init];
}


CCameraAcquisitionMac::~CCameraAcquisitionMac()
{
	Close();



	if( m_image != NULL ){
		//image_destroy( m_image, 0);
		m_image = NULL;
	}
    
    if (temp_img){
        image_destroy(temp_img, 0);
        temp_img = NULL;
    }
    
    [camera release];
}




int CCameraAcquisitionMac::Open( int iCamera )
{

	
	GPLOG(("Camera Open\n" ) );
	GPLOG_FLUSH();



	m_iCamera = iCamera;

	
	

	GPLOG_FLUSH();
    
    [camera setResolution:m_resolution ];
    //[camera setResolution:5 ];

    [camera setCameraDevice:(iCamera == 0) ? UIImagePickerControllerCameraDeviceFront : UIImagePickerControllerCameraDeviceRear];
    [camera setupCaptureSession];
    
    m_cameraOpen = true;
    
    m_cWidth = 480;
    m_cHeight = 360;
      
	return 1;
}



int CCameraAcquisitionMac::AddPreviewLayer( CGRect *rect )
{
    [camera addVideoPreviewLayer:*rect];
    return( 1 );
}


AVCaptureVideoPreviewLayer * CCameraAcquisitionMac::GetPreviewLayer()
{
    AVCaptureVideoPreviewLayer *previewLayer = [ camera previewLayer];
    
    return( previewLayer );
    
}


int CCameraAcquisitionMac::Get( image_type **im, int iFrame )
{
    
    UIImage *image = camera.lastFrame;
    //NSData *imageData = camera.lastFrameData;
    //CGSize imageSize = camera.imageSize;

    if (!image) {
        return (-1);
    }
    
    if (temp_img && ((m_cWidth != image.size.width) || (m_cHeight != image.size.height))){
        if (temp_img) {             // camera gave a new frame size, we need to reset the temp_img
            delete temp_img;
            temp_img = NULL;
        }
    }
    
    
	if ( temp_img == NULL ){
        m_cWidth = image.size.width;
        m_cHeight = image.size.height;
		temp_img = image_alloc( m_cWidth, m_cHeight, 4, IMAGE_TYPE_U8, 1 );
        
 //       if( m_iCamera == 1 ){
        if( m_rWidth == -1 || m_rHeight == -1 ){
            m_height = m_cHeight;
            m_width = m_cWidth;
        }
        
        if( m_height > m_cHeight )
            m_height = m_cHeight;
        
        if( m_width > m_cWidth )
            m_cWidth = m_width;
        
        
        SetFoucusPixel( m_fx, m_fy );
	}
    
    
   // NSLog(@"%.1f %.1f", image.size.width, image.size.height);

    CGImageRef imageRef = image.CGImage;
    CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(imageRef));
    NSData *data      = (NSData *)dataRef;
    //NSData *data = imageData;
    unsigned char *pixels        = (unsigned char *)[data bytes];
    //NSLog(@"%d", [data length]);
    memcpy(temp_img->data, pixels, [data length]);

    CFRelease(dataRef);
    //CGImageRelease(imageRef);
    
    

    

    if( m_protrait == 1 )
        m_image = image3_iphone_camera( temp_img, m_width, m_height, m_image );
          
    else
//        m_image = image3_iphone_camera_1( temp_img, m_width, m_height, m_image );
        m_image = image3_iphone_camera_1( temp_img, m_x0, m_y0, m_width, m_height, m_image );

    
	*im = m_image;

	m_iFrame++;


	return( 1 );
}


void CCameraAcquisitionMac::SetFoucusPixel( int x, int y )
{
	m_fx = x;
	m_fy = y;
    
    if( m_cWidth <= 0 )
        return;
    
    if( m_fx > 0 ){
        m_x0 = m_fx - 0.5*m_width;
        if( m_x0 < 0 )  m_x0 = 0;
    }
    else
        m_x0 = 0.5 * ( m_cWidth - m_width );
    
    
    if( m_fy > 0 ){
        m_y0 = m_fy - 0.5*m_height;
        if( m_y0 < 0 )  m_y0 = 0;
    }
    else
        m_y0 = 0.5 * ( m_cHeight - m_height );
}
        


void CCameraAcquisitionMac::SetResolution( int resolution )
{
    m_resolution = resolution;
    if (m_cameraOpen){                          // if camera not open, Open method will call the camera set method
        [camera setPreset:resolution];          // we use setPreset because the session is already working
    }
}


int CCameraAcquisitionMac::GetWhiteBalance( long *value, long *type )
{

	return( 1 );
}

int CCameraAcquisitionMac::SetWhiteBalance( long value, long type )
{

	return( 1 );
}





int CCameraAcquisitionMac::SetAutoFocus( float x, float y )
{
    if( x < 0 || y < 0 )
        x = y = 0.5;
    

    [ camera SetAutoFocusAtPoint:CGPointMake(x, y)];
    
//    [ camera SetContinuousFocusAtPoint:CGPointMake(.5f, .5f)];
    
	return( 1 );
}

int CCameraAcquisitionMac::SetContinuousAutoFocus( float x, float y )
{
    if( x < 0 || y < 0 )
        x = y = 0.5;
    
    
    [ camera SetContinuousFocusAtPoint:CGPointMake(x, y)];
        
	return( 1 );
}


int CCameraAcquisitionMac::GetFocusState()
{
    if( [ camera isAdjustingFocus] == true )
        return( 1 );
    
	return( -1 );
}


int CCameraAcquisitionMac::GetFocus( long *value, long *type )
{
    //  [ camera SetContinuousFocusAtPoint:CGPointMake(.5f, .5f)];
    //    [ camera SetAutoFocusAtPoint:CGPointMake(.5f, .5f)];
	return( 1 );
}

int CCameraAcquisitionMac::SetFocus( long value, long type )
{
  //  [ camera SetContinuousFocusAtPoint:CGPointMake(.5f, .5f)];
//    [ camera SetAutoFocusAtPoint:CGPointMake(.5f, .5f)];
	return( 1 );
}




int CCameraAcquisitionMac::GetSharpness( long *value, long *type )
{

	return( 1 );
}

int CCameraAcquisitionMac::SetSharpness( long value, long type )
{

	return( 1 );
}



int CCameraAcquisitionMac::GetPowerLineFrequency( long *value, long *type )
{

	return( 1 );
}

int CCameraAcquisitionMac::SetPowerLineFrequency( long value, long type )
{

	return( 1 );
}


int CCameraAcquisitionMac::GetExposure( long *value, long *type )
{

	return( 1 );
}

int CCameraAcquisitionMac::SetExposure( long value, long type )
{

	return( 1 );
}












int CCameraAcquisitionMac::LogStatus()
{
#ifndef WINCE
	long value,	flag;

	GPLOG(("Camera Setting - BEGIN\n" ) );
	
	if( GetWhiteBalance( &value, &flag ) >= 0 )
		GPLOG(("WhiteBalance  %d  %d\n", (int)flag, (int)value ) );


	if(GetSharpness( &value, &flag ) >= 0 )
		GPLOG(("Sharpness  %d  %d\n", (int)flag, (int)value ) );


	if( GetFocus( &value, &flag ) >= 0 )
		GPLOG(("Focus  %d  %d\n", (int)flag, (int)value ) );


	if( GetPowerLineFrequency( &value, &flag  ) > 0 )
		GPLOG(("Power Line Frequency  %d  %d\n", (int)flag, (int)value ) );




	if( GetExposure( &value, &flag ) >= 0 )
		GPLOG(("Exposure  %d  %d\n", (int)flag, (int)value ) );


	GPLOG(("Camera Setting - END\n" ) );

	GPLOG_FLUSH();

#endif
	return( 1 );
}



static image_type *
image3_iphone_camera( image_type *sim, int width, int height, image_type *im )
{
	u_char	*tp;
	u_int	*sp;
	int	i,	j,	y0;
    
    
    
	im = image_realloc( im, width, height, 3, IMAGE_TYPE_U8, 1 );
    
    
  //  y0 = sim->height - 0.5*( sim->height - width );
//	int align = -sim->width;
    y0 =0.5*( sim->height - width );
	int align = sim->width;

    
	sp = (u_int *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
	//	sp = (u_int *)IMAGE_PIXEL( sim, y0, im->height-1 - i );
        sp = (u_int *)IMAGE_PIXEL( sim, y0, i );
		for( j = 0 ; j < im->column ; j++, sp += align ){
			*tp++ = IMAGE4_BLUE(*sp );
			*tp++ = IMAGE4_GREEN(*sp );
			*tp++ = IMAGE4_RED(*sp );
		}
	}
    
	return( im );
}


static image_type *
image3_iphone_camera_1( image_type *sim, image_type *im )
{
	u_char	*tp;
	u_int	*sp;
	int	i,	j;
    
    
    
	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1 );
    
    
	sp = (u_int *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){
			*tp++ = IMAGE4_BLUE(*sp );
			*tp++ = IMAGE4_GREEN(*sp );
			*tp++ = IMAGE4_RED(*sp );
		}
	}
    
	return( im );
}


static image_type *
image3_iphone_camera_1( image_type *sim, int width, int height, image_type *im )
{
	u_char	*tp;
	u_int	*sp;
	int	i,	j;
    
    
    
	im = image_realloc( im, width, height, 3, IMAGE_TYPE_U8, 1 );
    
    int x0 = 0.5*( sim->width - width );
    int y0 = 0.5*( sim->height - height );
    
    
	sp = (u_int *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->height ; i++ ){
        sp = IMAGE4_PIXEL( sim, i+y0, x0 );
		for( j = 0 ; j < im->width ; j++, sp++ ){
			*tp++ = IMAGE4_BLUE(*sp );
			*tp++ = IMAGE4_GREEN(*sp );
			*tp++ = IMAGE4_RED(*sp );
		}
	}
    
	return( im );
}



static image_type *
image3_iphone_camera_1( image_type *sim, int x0, int y0, int width, int height, image_type *im )
{
	u_char	*tp;
	u_int	*sp;
	int	i,	j;
    
    
    
	im = image_realloc( im, width, height, 3, IMAGE_TYPE_U8, 1 );
    
 //   int x0 = 0.5*( sim->width - width );
 //   int y0 = 0.5*( sim->height - height );
    
    
	sp = (u_int *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->height ; i++ ){
        sp = IMAGE4_PIXEL( sim, i+y0, x0 );
		for( j = 0 ; j < im->width ; j++, sp++ ){
			*tp++ = IMAGE4_BLUE(*sp );
			*tp++ = IMAGE4_GREEN(*sp );
			*tp++ = IMAGE4_RED(*sp );
		}
	}
    
	return( im );
}

