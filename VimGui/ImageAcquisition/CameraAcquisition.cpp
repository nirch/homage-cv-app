/**********************************
 ***   CameraAcquisition.cpp   ***
 **********************************/
#include	<string.h>

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"
#include	"Utime/GpTime.h"
#include	"ImageType/ImageType.h"

#include	"CameraAcquisition.h"





#ifdef WINCE
#include "CameraGrabber/CameraGrabberWM.h"
#else
#include "CameraGrabber/CameraGrabber.h"
#endif




CImageAcquisition *CreateCameraAcquisition()
{
	CImageAcquisition	*vc;

	vc = new CCameraAcquisition();

	return( vc );
}



CCameraAcquisition::CCameraAcquisition()
{
	m_cameraGrabber = NULL;

	m_image = NULL;

	m_acq = 1;


	m_active = 1;

	m_rWidth = 640;
	m_rHeight = 480;
}


CCameraAcquisition::~CCameraAcquisition()
{
	Close();

	if( m_cameraGrabber != NULL ){
		delete m_cameraGrabber;
		m_cameraGrabber = NULL;
	}

	if( m_image != NULL ){
		image_destroy( m_image, 0);
		m_image = NULL;
	}
}




int CCameraAcquisition::Open( int iCamera )
{
#ifdef WINCE
	m_cameraGrabber = new CCameraGrabberWM();
#else
	m_cameraGrabber = new CCameraGrabber();
#endif



	if( m_cameraGrabber->Open( L"", iCamera, m_rWidth, m_rHeight ) < 0 ){
		delete m_cameraGrabber;
		m_cameraGrabber = NULL;
		return -1;
	}


	
	GPLOG(("Camera Open\n" ) );
	GPLOG_FLUSH();


	m_width = m_cameraGrabber->GetWidth();
	m_height = m_cameraGrabber->GetHeight();



	image_type *im;
	int	i,	n;
	for( i = 0, n = 0 ; i < 100 && n < 2 ; i++ ){
		if( m_cameraGrabber->GrabImage( &im )> 0 )
			n++;

		Sleep( 50 );

	}

	GPLOG(("GrabImage: %d  %d\n", i, n  ) );
	GPLOG_FLUSH();

	if( n < 2 ){
		delete m_cameraGrabber;
		m_cameraGrabber = NULL;
		return -1;
	}




	//if( m_cameraGrabber->GrabImage( &im ) < 0 )
	//	delete m_cameraGrabber;
	//	m_cameraGrabber = NULL;
	//	return -1;
	//}


	

	Calibrate();
	


	GPLOG_FLUSH();

	return 1;
}





int CCameraAcquisition::Get( image_type **im, int iFrame )
{
	if( m_cameraGrabber->GrabImage( im ) < 0 )
		return( -1 );


	image3_VH_RGB_flip(*im);


	m_iFrame++;


	return( 1 );
}



int CCameraAcquisition::CalibrateWhiteBalance()
{
#ifndef WINCE

	m_cameraGrabber->SetWhiteBalance( 0, CG_AUTO );


	Sleep( 500 );
//	Sleep( 1000 );


	long value,	flag;
	m_cameraGrabber->GetWhiteBalance( &value, &flag );

	m_cameraGrabber->SetWhiteBalance( value, CG_MANUAL );

	GPLOG(("Setting: White Balance %d  Successed\n", (int)value ) );
	return( 1 );
#else
	return( 1 );
#endif

}


int CCameraAcquisition::CalibrateSharpness()
{
#ifndef WINCE

//	m_cameraGrabber->SetSharpness( 0, CG_AUTO );


//	Sleep( 100 );


	long value,	flag;
	m_cameraGrabber->GetSharpness( &value, &flag );


	value = 0;
	m_cameraGrabber->SetSharpness( value, CG_MANUAL );
//	m_cameraGrabber->SetSharpness( value, CG_AUTO );



	m_cameraGrabber->GetSharpness( &value, &flag );

	GPLOG(("Setting: Sharpness %d  Successed\n", (int)value ) );

	return( 1 );
#else
	return( 1 );
#endif

}


int CCameraAcquisition::CalibrateFocus()
{
#ifndef WINCE

	if( m_cameraGrabber->SetFocus( 0, CG_AUTO ) < 0 )
		return(-1);

	Sleep( 100 );

	long value,	flag;
	m_cameraGrabber->GetFocus( &value, &flag );


	m_cameraGrabber->SetFocus( value, CG_MANUAL );

	m_cameraGrabber->GetFocus( &value, &flag );


	GPLOG(("Setting: Focus %d  Successed\n", (int)value) );
	return( 1 );
#else
	return( 1 );
#endif
}


int CCameraAcquisition::CalibratePowerLineFrequency()
{
	long value,	flag;

	if( m_cameraGrabber->IsPowerLineSupport() < 0 ){
		GPLOG(("Setting: Power Line Frequency doesn't suported\n") );
		return( -1 );
	}


	if( m_cameraGrabber->GetPowerLine( &value, &flag  ) > 0 )
		GPLOG(("Power Line Frequency  %d  %d\n", (int)flag, (int)value ) );

	return( 1 );
}



int CCameraAcquisition::CalibrateExposure()
{
#ifndef WINCE
	
long value,	flag;
//long	PLvalue,	PLflag;



	if( m_cameraGrabber->IsLp1ExposureSupport() > 0 ){
//		m_cameraGrabber->GetLp1Exposure( &value, &flag );
//		if( flag == CG_MANUAL )
		m_cameraGrabber->SetExposure( 0, CG_AUTO );


		Sleep( 1000 );


		m_cameraGrabber->GetLp1Exposure( &value, &flag );

		m_cameraGrabber->SetLp1Exposure( value, CG_MANUAL );

			GPLOG(("Setting: Lp1 Expusre %d Successed\n", (int)value ) );

		return( 1 );

	}


	if( m_cameraGrabber->IsExposureSupport() < 0 ){
		GPLOG(("Setting: Expusre doesn't suported\n") );

		return( -1 );
	}



	m_cameraGrabber->SetExposure( 0, CG_AUTO );


	Sleep( 1000 );


	m_cameraGrabber->GetExposure( &value, &flag );

	m_cameraGrabber->SetExposure( value-1, CG_MANUAL );



	//m_cameraGrabber->GetExposure( &value, &flag );

	GPLOG(("Setting: Expusre %d Successed\n", (int)value ) );

	return( 1 );
#else
	return( 1 );
#endif
}

int CCameraAcquisition::CalibrateRightLigt()
{
#ifndef WINCE
	

	if( m_cameraGrabber->IsExposureSupport() < 0 ){
		GPLOG(("Setting: Right Light doesn't suported\n") );

		return( -1 );
	}
	
	long value,	flag;
	m_cameraGrabber->GetRightLight( &value, &flag );

	m_cameraGrabber->SetRightLight( 0, CG_AUTO );


	m_cameraGrabber->GetRightLight( &value, &flag );


	GPLOG(("Setting: Right Light Successed\n") );

	return( 1 );
#else
	return( 1 );
#endif
}










int CCameraAcquisition::LogStatus()
{
#ifndef WINCE
	long value,	flag;

	GPLOG(("Camera Setting - BEGIN\n" ) );
	
	if( m_cameraGrabber->GetWhiteBalance( &value, &flag ) >= 0 )
		GPLOG(("WhiteBalance  %d  %d\n", (int)flag, (int)value ) );


	if( m_cameraGrabber->GetSharpness( &value, &flag ) >= 0 )
		GPLOG(("Sharpness  %d  %d\n", (int)flag, (int)value ) );


	if( m_cameraGrabber->GetFocus( &value, &flag ) >= 0 )
		GPLOG(("Focus  %d  %d\n", (int)flag, (int)value ) );


	if( m_cameraGrabber->GetPowerLine( &value, &flag  ) > 0 )
		GPLOG(("Power Line Frequency  %d  %d\n", (int)flag, (int)value ) );



	if( m_cameraGrabber->GetLp1Exposure( &value, &flag ) >= 0 ){
		GPLOG(("ExposureLp1  %d  %d\n", (int)flag, (int)value ) );
	}
	else {
		if( m_cameraGrabber->GetExposure( &value, &flag ) >= 0 )
			GPLOG(("Exposure  %d  %d\n", (int)flag, (int)value ) );
	}


	GPLOG(("Camera Setting - END\n" ) );

	GPLOG_FLUSH();

#endif
	return( 1 );
}


int CCameraAcquisition::LogStatus( char *fname )
{
	char	file[256];
	FILE	*fp;
	long value,	flag;

	gpFilename_force_extension( fname, ".clog", file );

	if( (fp = fopen( file, "wb" ) ) == NULL )
		return( -1 );


	if( m_cameraGrabber->GetWhiteBalance( &value, &flag ) >= 0 ){
		fprintf( fp, "WhiteBalance  %d  %d\n", (int)flag, (int)value  );
	}


	if( m_cameraGrabber->GetSharpness( &value, &flag ) >= 0 ){
		fprintf( fp, "Sharpness  %d  %d\n", (int)flag, (int)value  );
	}


	if( m_cameraGrabber->GetFocus( &value, &flag ) >= 0 ){
		fprintf( fp, "Focus  %d  %d\n", (int)flag, (int)value  );
	}


	if( m_cameraGrabber->GetPowerLine( &value, &flag  ) > 0 ){
		fprintf( fp, "Power Line Frequency  %d  %d\n", (int)flag, (int)value );
	}



	if( m_cameraGrabber->GetLp1Exposure( &value, &flag ) >= 0 ){
		fprintf( fp, "ExposureLp1  %d  %d\n", (int)flag, (int)value );
	}
	else 
		if( m_cameraGrabber->GetExposure( &value, &flag ) >= 0 ){
			fprintf( fp, "Exposure  %d  %d\n", (int)flag, (int)value );
		}



	fclose( fp );

	return( 1 );
}


int CCameraAcquisition::Calibrate()
{
int	i;

	GPLOGF(("CALIBRATE - BEGIN\n" ) );

	int t0 = gpTime( 0 );

	CalibrateSharpness();


	m_cameraGrabber->SetWhiteBalance( 0, CG_AUTO );


	m_cameraGrabber->SetFocus( 0, CG_AUTO );


	m_cameraGrabber->SetExposure( 0, CG_AUTO );



	long	value,	flag;
	
	if(  m_cameraGrabber->IsLp1ExposureSupport() > 0 ){
		long	av[40],	value0;

		value0 = 0;
		for( i = 0 ; i < 40 ; i++ ){
			m_cameraGrabber->GetLp1Exposure( &value, &flag );
			GPLOGF(("Lp1 Expusre %d\n", (int)value ) );
			if( i > 0 )
				if( (av[i] = value - value0 ) < 0 )	av[i] = -av[i];
			value0 = value;

			if( i > 4 && av[i-1] < 2 && av[i] < 2 )
				break;

			Sleep( 100 );
		}
	}
	else Sleep( 2000 );




#ifdef _CALIBRATE_WHITEBALANCE
	m_cameraGrabber->GetWhiteBalance( &value, &flag );
	m_cameraGrabber->SetWhiteBalance( value, CG_MANUAL );
#endif

	m_cameraGrabber->GetFocus( &value, &flag );
	m_cameraGrabber->SetFocus( value, CG_MANUAL );



	if( m_cameraGrabber->IsLp1ExposureSupport() > 0 ){
		m_cameraGrabber->GetLp1Exposure( &value, &flag );
		m_cameraGrabber->SetLp1Exposure( value, CG_MANUAL );

	}
	else	{
		if( m_cameraGrabber->IsExposureSupport() > 0 ){
			m_cameraGrabber->GetExposure( &value, &flag );
			m_cameraGrabber->SetExposure( value, CG_MANUAL );
		}
	}


	GPLOGF(("CALIBRATE- END     time: %d\n", gpTime(0) - t0 ) );

	LogStatus();

	return( 1 );
}





int CCameraAcquisition::GetWhiteBalanceRange( long *min, long *max )
{
	long steppingDelta, default, capsFlags;
	int	ret;


	ret = m_cameraGrabber->GetRangeWhiteBalance( min, max, &steppingDelta, &default, &capsFlags );
	return( ret );
}


int CCameraAcquisition::GetWhiteBalance( long *value, long *flag )
{
int	ret;

	ret = m_cameraGrabber->GetWhiteBalance( value, flag );

	return( ret );
}

int CCameraAcquisition::SetWhiteBalance( long value, long flag )
{
	m_cameraGrabber->SetWhiteBalance( value, flag );

	return( 1 );
}

int CCameraAcquisition::GetGainRange( long *min, long *max )
{
	long steppingDelta, default, capsFlags;
	int	ret;


	ret = m_cameraGrabber->GetRangeGain( min, max, &steppingDelta, &default, &capsFlags );
	return( ret );
}


int CCameraAcquisition::GetGain( long *value, long *flag )
{
int	ret;

	ret = m_cameraGrabber->GetGain( value, flag );

	return( ret );
}

int CCameraAcquisition::SetGain( long value, long flag )
{
	m_cameraGrabber->SetGain( value, flag );

	return( 1 );
}

int CCameraAcquisition::GetExposureRange( long *min, long *max )
{
	long steppingDelta, default, capsFlags;
	int	ret;

	if( m_cameraGrabber->IsLp1ExposureSupport() > 0 ){
		*min = 10;
		*max = 2000;
		return(1);
	}

	ret = m_cameraGrabber->GetRangeExposureN( min, max, &steppingDelta, &default, &capsFlags );
	return( ret );
}


int CCameraAcquisition::GetExposure( long *value, long *flag )
{
	int	ret;
	if( m_cameraGrabber->IsLp1ExposureSupport() > 0 ){
		m_cameraGrabber->GetLp1Exposure( value, flag );
		return( 1 );
	}

	ret = m_cameraGrabber->GetExposureN( value, flag );

	return( ret );
}

int CCameraAcquisition::SetExposure( long value, long flag )
{
	if( m_cameraGrabber->IsLp1ExposureSupport() > 0 ){
		m_cameraGrabber->SetLp1Exposure( value, flag );
		return( 1 );
	}

	m_cameraGrabber->SetExposureN( value, flag );

	return( 1 );
}