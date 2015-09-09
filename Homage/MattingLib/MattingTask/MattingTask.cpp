
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG 
//#define _DUMP
#endif

#ifndef _GPLOG
#define _GPLOG
#endif
#include "Ulog/Log.h"


//#include "UThread/VMutex.h"
#include "UThread/VConditionMutex.h"


#include "MattingTask.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "YUV/Image2YUV.h"

#include "ImageAcquisition/ImageAcquisition.h"


#include "../UniformBackground/UniformBackground.h"

#include "PngCodec/PngWriter/PngWriter.h"



CMattingTask::CMattingTask()
{
	m_width = 640;
	m_height = 480;


	m_matting = NULL;

	m_operation = 1;


	m_imageAcquisition = NULL;

	m_iFrame = -1;


	m_vf.im = NULL;

	m_vf.aim = NULL;
	m_vf.iFrame = -1;


	m_color = 0xFFFFFF;
	
	m_bim = NULL;

	m_im = NULL;

	m_sim = NULL;

	m_dumpDir[0] = 0;

	m_frameRate = 15;


	m_recorder = NULL;
	m_record = 0;


//	vTimer_init( &m_timer, 66 );

	m_mutex = new CVConditionMutex();

	gpTime_init( &m_tCapture );
	gpTime_init( &m_tProcess );
	gpTime_init( &m_tGet );
}


CMattingTask::~CMattingTask()
{
	if( m_mutex != NULL ){
		delete m_mutex;
		m_mutex = NULL;
	}

	if( m_vf.im != NULL ){
		image_destroy( m_vf.im, 1 );
		m_vf.im = NULL;
	}

	if( m_matting != NULL ){
		delete m_matting;
		m_matting = NULL;
	}

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}


	if( m_bim != NULL ){
		image_destroy( m_bim, 1 );
		m_bim = NULL;
	}

}





void CMattingTask::SetImageAcquisition( CImageAcquisition *imageAcquisition )
{
	GPLOGF(("< SetImageAcquisition  .."));
	m_imageAcquisition = imageAcquisition;
	GPLOGF((">"));


}


int CMattingTask::Init( char *prmFile,  char *ctrFile, int width, int height )
{

	if( m_matting != NULL )
		return( 2 );



	m_matting = new CUniformBackground( );

	m_matting->Init( prmFile, ctrFile, width, height );

	m_width = width;
	m_height = height;

	m_iFrame = 0;

	m_operation = 0;

	return( 1 );
}


void CMattingTask::SetOperation( int operation )
{
	m_operation = operation;

	GPLOGF(("CMattingTask: SetOpretion: %d \n", m_operation ) );
}

void CMattingTask::SetBackground( int color )
{
	m_color = color;
}


void CMattingTask::SetBackground( image_type *bim )
{
	if( bim->depth != 3 )
		m_bim = image3_from( bim, m_bim );
	else
		m_bim = image_make_copy( bim, m_bim );
}


void CMattingTask::SetDumpDir( char *dir )
{
	strcpy( m_dumpDir, dir );
}


void CMattingTask::DumpImage( image_type *sim, char *fname )
{

	if( m_dumpDir[0] == 0 )
		return;

	GPLOGF( ("<DumpImage ..") );

	char	file[256];
	//gpFilename_force_extension( fname, ".png", file );
	//image_write_png( sim, file );

	sprintf( file, "%s/%s", m_dumpDir, fname );

	//gpFilename_force_extension( fname, ".bmp", file );
	image_write_bmp( sim, file );

	GPLOGF( (" %s >", file ) );
}


int CMattingTask::StartTask()
{
	m_process = 1;

	vTimer_init( &m_timer, 1000.0/m_frameRate );//66.66 );

	StartThread();

	GPLOGF(("CMattingTask: StartTAsk\n") );

	return( 1 );
}



int CMattingTask::StopTask()
{
	GPLOGF(("<CMattingTask: Stop Task ... ") );

	if( m_process < 0 ){
		GPLOGF((" m_process == -1 >\n") );
		return( 1 );
	}

	m_process = 0;

	while( m_process >= 0 )
		Sleep( 10 );



	GPLOGF((" stop task succuss>\n") );

	return( 1 );
}


void CMattingTask::Run()
{

	m_process = 1;

	while( m_process == 1 ) {

		if( vTimer_test( &m_timer, 0) < 0 ){
			Sleep( 10 );
			continue;
		}

		if( Process() == -2 )
			Sleep( 10 );

	}

	m_process = -1;
}


int CMattingTask::Process()
{		
	//GPLOGF(("<Process T ..."));
	if( m_imageAcquisition == NULL )
		return( -1 );
 

	image_type *im;

	gpTime_start( &m_tCapture );
	if( m_imageAcquisition->Get( &im, -1 ) < 0 ){
		GPLOGF(("G"));
		return( -2 );
	}
//	GPLOGF(("."));

	gpTime_stop( &m_tCapture );
	GPLOG_TIMEM(( "m_imageAcquisition->Get", &m_tCapture, 16) );

	m_iFrame++;

	IMAGE_DUMP( im, "AA", m_iFrame, NULL );

	gpTime_start( &m_tProcess );


//	GPLOGF(("width: %d  height: %d   im->width: %d  im->height: %d  ", m_width, m_height,  im->width, im->height));

	if( im->height == 480 || im->width == 480 ){
		m_sim = image_crop( im, 0.5*(im->width - m_width), 0.5*(im->height - m_height), m_width, m_height, m_sim );
	}
	else if( im->height == 720 )
		m_sim = image_sample2( im, m_sim );
	else m_sim = image_make_copy( im, m_sim );


	Record( m_sim );
	 
	int operation = m_operation;

	GPLOGF(("< Process: %d ", operation ) );

	switch( operation ){
	case 1:

		DumpImage( m_sim, "dump-Background.bmp" );

		m_matting->ProcessBackgroundState( m_sim, m_iFrame );
	break;

	case 2:
		DumpImage( m_sim, "dump-matting.bmp" );
		m_matting->Process( m_sim, m_iFrame, NULL );
		break;
	}

	GPLOGF((" 1" ) );


	SetOutput( operation, &m_vf, m_sim );

	GPLOGF((">" ) );
	
	gpTime_stop( &m_tProcess );

	GPLOG_TIMEM(( "\nProcess", &m_tProcess, 16) );



	return( 1 );
}



int	CMattingTask::SetOutput( int operation, hiFrame_type *vf, image_type *im )
{
	m_mutex->Lock();

	switch( operation ){
	case 0:
		vf->im = image_android( im, vf->im );
		vf->status = 32;
		break;
	case 1:
		vf->im = image_android( im, vf->im );
		vf->status = m_matting->m_state;

		break;
	case 2:
	

		if( m_bim != NULL )
			m_im = m_matting->GetImage( m_bim, m_im );
		else m_im = m_matting->GetImage( m_color, m_im );

		vf->im = image_android( m_im, vf->im );


		vf->aim = m_matting->GetImageAlpha( vf->aim );
//		vf->aim = image_android( vf->aim, vf->aim );

		vf->status = 16;
		break;
	}




	GPLOGF(("Output: %d %d\n", operation, vf->status ) );




	vf->iFrame = m_iFrame;
	vf->timeStamp = im->timeStamp;

	m_mutex->Signal();
	m_mutex->Unlock();

	return( 1 );
	
}




int	CMattingTask::GetOutput( u_char *data, int *nData )
{

	if( m_process < 0 )
		return( -1 );

	gpTime_start( &m_tGet );

	m_mutex->Lock();

	if( m_vf.iFrame < 0 ){
		if( m_mutex->Wait() < 0 ){
			m_mutex->Unlock();
			return( -1 );
		}
	}



	*nData = m_vf.im->width * m_vf.im->height * 4;
	memcpy( data, m_vf.im->data, *nData );

	int status = m_vf.status;

	m_vf.iFrame = -1; 

	m_mutex->Unlock();


	gpTime_stop( &m_tGet );
	GPLOG_TIMEM(( "Homage-GET", &m_tGet, 16) );


	return( status );
}



int	CMattingTask::GetOutput( u_char *data, int *nData, u_char *data1, int *nData1 )
{

	if( m_process < 0 )
		return( -1 );

	gpTime_start( &m_tGet );

	m_mutex->Lock();

	if( m_vf.iFrame < 0 ){
		if( m_mutex->Wait() < 0 ){
			m_mutex->Unlock();
			return( -1 );
		}
	}



	*nData = m_vf.im->width * m_vf.im->height * 4;
	memcpy( data, m_vf.im->data, *nData );

	if( m_vf.aim != NULL ){
		image_type *aim = image_sample2( m_vf.aim, NULL );
		*nData1 = aim->width * aim->height * 4;
		memcpy( data1, aim->data, *nData1 );
		image_destroy( aim, 1 );
	}

	int status = m_vf.status;
	m_vf.iFrame = -1;

	m_mutex->Unlock();


	gpTime_stop( &m_tGet );
	GPLOG_TIMEM(( "HiPark-GET", &m_tGet, 16) );


	return( status );
}








int	CMattingTask::Record( image_type *im )
{

	GPLOGF(("< Record  " ));

	if( m_record == 1 && m_recorder != NULL )
		m_recorder->WriteFrame( im );

	GPLOGF((">" ));
	return( 1 );
}

int	CMattingTask::StartRecord( char *dir )
{
	GPLOGF(("<start recorde -- " ));

	if( m_recorder == NULL ){

		m_recorder = new CPngWriter();

		m_recorder->Open( 100, (char *)dir );
	}

	m_record = 1;

	GPLOGF((" >"));
	return( 1 );
}


int	CMattingTask::StopRecord()
{
	GPLOGF(("<Stop recorde -- " ));

	m_record = 0;

	GPLOGF((" >"));

	return( 1 );
}