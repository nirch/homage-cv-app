/************************************
 ***   SncCameraAcquisition.cpp   ***
 ************************************/
#include	<string.h>

#define _GPLOG

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"
#include	"Utime/GpTime.h"
#include	"ImageType/ImageType.h"
#include	"YUV/Image2YUV.h"

#include	"AndroidCameraAcquisition.h"











CAndroidCameraAcquisition::CAndroidCameraAcquisition()
{


	m_image = NULL;

	m_cameraId = 1;


	m_active = 1;

	m_orientation = 0;

	m_rWidth = 640;
	m_rHeight = 480;

	m_yuv = NULL;

	
	m_mutex = new CVConditionMutex();
	
	m_capture = 0;



	gpTime_init( &m_gTime );
	gpTime_init( &m_tCapture );
}


CAndroidCameraAcquisition::~CAndroidCameraAcquisition()
{
	if( m_mutex != NULL ){
		delete m_mutex;
		m_mutex = NULL;
	}

	if( m_yuv != NULL ){
		image_destroy( m_yuv, 1 );
		m_yuv = NULL;
	}
}


void CAndroidCameraAcquisition::Close()
{

}


int CAndroidCameraAcquisition::Open( char *ip, int port, int raw )
{



	
	GPLOG(("Camera Open\n" ) );
	GPLOG_FLUSH();


	//m_width = m_snc->GetWidth();
	//m_height = m_snc->GetHeight();




	GPLOG_FLUSH();


	return 1;
}


int CAndroidCameraAcquisition::PreviewCallBack( int width, int height, u_char *data )
{
//	GPLOGF(("<PreviewCallBack ... w:%d   h %d", width, height ));




	m_mutex->Lock();



	gpTime_start( &m_tCapture );

	if( m_yuv == NULL ){
		m_yuv = image_createYUV420( height,  width, NULL );
	}

//	GPLOGF((" 2"));


	memcpy( m_yuv->data, data, width*height*1.5 );

	m_yuv->timeStamp = vTime();

	m_width = width;
	m_height = height;

//	GPLOGF((" 3"));

	m_capture = 1;

	

	gpTime_stop( &m_tCapture );
	GPLOG_TIMEM(( "\nCamera-Capture", &m_tCapture, 16) );

	m_mutex->Signal();

	m_mutex->Unlock();

	//GPLOGF((">"));

	return( 1 );
}


int CAndroidCameraAcquisition::WaitToImage()
{
	return( 1 );
}


void CAndroidCameraAcquisition::SetFrameSize( int width, int height )
{
	m_rWidth = width;
	m_rHeight = height;

	m_width = m_rWidth;
	m_height = m_rHeight;
}


int CAndroidCameraAcquisition::Get( image_type **im, int iFrame )
{
	//GPLOGF(("<Get ..."));
	m_mutex->Lock();

	gpTime_start( &m_gTime );

	if( m_capture == 0 ){
		if( m_mutex->Wait( 2000 ) < 0 ){
			m_mutex->Unlock();
			GPLOGF(("<wait failed ..."));
			return( -1 );
		}
	}


	switch( m_orientation ){
	case 0:
		m_image = imageNV21_to_RGB( m_yuv, m_image );
		break;
	case 90:
		m_image = imageNV21_to_RGB_swap(  m_yuv, m_image  );
		image_flipV( m_image );
		break;
	case 270:
		m_image = imageNV21_to_RGB_swap(  m_yuv, m_image  );
		break;
	}

#ifdef _AA_
	if( m_orientation == 0 ){
		m_image = imageNV21_to_RGB( m_yuv, m_image );

		
	}
	else {
		m_image = imageNV21_to_RGB_swap(  m_yuv, m_image  );
//		image_flipV( m_image );
		GPLOGF((" SWAP "));
	}
#endif
	image_flipH( m_image );


//	m_image = imageNV21_to_RGB( m_yuv, m_image );
	m_image->timeStamp = m_yuv->timeStamp;

	m_capture = 0;

	m_mutex->Unlock();


	gpTime_stop( &m_gTime );
	GPLOG_TIMEM(( "\nCamera-Get", &m_gTime, 16) );

	//GPLOGF((">"));
	

	*im = m_image;



	m_iFrame++;


	return( 1 );
}





int CAndroidCameraAcquisition::LogStatus()
{

//	long value,	flag;

	GPLOG(("Camera Setting - BEGIN\n" ) );

	GPLOG(("Frame  %d  %d\n", m_width, m_height ) );

	
	//if( m_cameraGrabber->GetWhiteBalance( &value, &flag ) >= 0 )
	//	GPLOG(("WhiteBalance  %d  %d\n", (int)flag, (int)value ) );


	//if( m_cameraGrabber->GetSharpness( &value, &flag ) >= 0 )
	//	GPLOG(("Sharpness  %d  %d\n", (int)flag, (int)value ) );


	//if( m_cameraGrabber->GetFocus( &value, &flag ) >= 0 )
	//	GPLOG(("Focus  %d  %d\n", (int)flag, (int)value ) );


	//if( m_cameraGrabber->GetPowerLine( &value, &flag  ) > 0 )
	//	GPLOG(("Power Line Frequency  %d  %d\n", (int)flag, (int)value ) );



	//if( m_cameraGrabber->GetLp1Exposure( &value, &flag ) >= 0 ){
	//	GPLOG(("ExposureLp1  %d  %d\n", (int)flag, (int)value ) );
	//}
	//else {
	//	if( m_cameraGrabber->GetExposure( &value, &flag ) >= 0 )
	//		GPLOG(("Exposure  %d  %d\n", (int)flag, (int)value ) );
	//}



	GPLOG(("Camera Setting - END\n" ) );

	GPLOG_FLUSH();


	return( 1 );
}





