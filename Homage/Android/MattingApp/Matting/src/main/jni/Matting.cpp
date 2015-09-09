#include <jni.h>




#include	"Matting.h"

#define _GPLOG

#include "Uigp/igp.h"
#include "Ulog/Log.h"

#include	"UjniTool.h"

#include "ImageAcquisition/Android/AndroidCameraAcquisition.h"
#include "MattingLib/MattingTask/MattingTask.h"


#include "Png/PngCodec/Png/ImageReadPng.h"


static CMattingTask *m_vg;
static CAndroidCameraAcquisition *m_iaq = NULL;






JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_openLog
  (JNIEnv * env, jclass c, jstring logFileStr )
{
	const char *logFile = ujni_getJString( env, logFileStr );

	GPLOG_OPEN( (char *)logFile );

	ujni_releaseJString( env, logFileStr, logFile );
	return( 1 );
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_closeLog
  (JNIEnv * env, jclass c )
{

	GPLOG_CLOSE();

	return( 1 );
}



JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_initCamera
  (JNIEnv * env, jclass c, jint orientation )
{
	GPLOGF(("<  initCamera-J ..."));


	if( m_iaq != NULL ){
		m_iaq->SetOrientation( orientation );
		return( 1 );
	}


	m_iaq = new CAndroidCameraAcquisition();

	m_iaq->SetOrientation( orientation );

	if( m_vg != NULL ){
		m_vg->SetImageAcquisition( (CImageAcquisition *)m_iaq );
		GPLOGF(("SetImageAcquisition "));
	}


	GPLOGF((" >\n"));


	return 1;
}



JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_init
  (JNIEnv * env, jclass c, jstring jprmFile, jstring jctrFile, jint width, jint height )
{
	GPLOGF(("<  init-J ..."));

	if( m_vg == NULL ){
		m_vg = new CMattingTask();
	}


	const char *prmFile = ujni_getJString( env, jprmFile );
	const char *ctrFile = ujni_getJString( env, jctrFile );


	if( m_iaq != NULL ){
		m_vg->SetImageAcquisition( (CImageAcquisition *)m_iaq );
		GPLOGF(("SetImageAcquisition "));
	}

	int ret = m_vg->Init( (char *)prmFile, (char *)ctrFile, width, height );

//	m_vg->StartTask();


	ujni_releaseJString( env, jprmFile, prmFile );
	ujni_releaseJString( env, jctrFile, ctrFile );

	GPLOGF((" >\n"));





	return ret;
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_setBackground
	(JNIEnv * env, jclass c, jstring jinFile )
{

	const char *inFile = ujni_getJString( env, jinFile );

	image_type *sim = image_read_png_file( (char *)inFile );

	if( sim == NULL ){
		ujni_releaseJString( env, jinFile, inFile );
		return( -1 );
	}

	m_vg->SetBackground( sim );


	ujni_releaseJString( env, jinFile, inFile );
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_setDumpDir
	(JNIEnv * env, jclass c, jstring jdir )
{
	const char *dir = ujni_getJString( env, jdir );

	m_vg->SetDumpDir( (char *)dir );


	ujni_releaseJString( env, jdir, dir );
}


JNIEXPORT jint JNICALL  Java_com_homage_matting_Matting_cameraCallBack
  (JNIEnv * env, jclass c, jint width, jint height, jbyteArray buffer )
{
//	GPLOGF( ("<cameraCallBack-J ..") );
//	m_env = env;
//	m_c = c;



	if( m_iaq == NULL )
		return( -1 );

//	GPLOGF( (" 1") );
	u_char *data = (u_char *)(env->functions)->GetByteArrayElements(env, buffer, NULL);
	if( data == NULL )
		return( -1 );

//	GPLOGF( (" 2") );
	int ret = m_iaq->PreviewCallBack( width, height, data );

//	GPLOGF( (" 3") );
	(env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *)data, 0);

//	GPLOGF( (" >") );

	return ret;
}



JNIEXPORT int JNICALL Java_com_homage_matting_Matting_getUpdate
  (JNIEnv * env, jclass c, jbyteArray  out, jbyteArray  out1 )
{
//	GPLOGF(("<GetImage-J ..."));

	if( m_vg == NULL ){
		GPLOGF( ("getImage null") );
		return( -1 );
	}


	// get image
	u_char *outData = (u_char *)(env->functions)->GetByteArrayElements(env, out, NULL);


	u_char *outData1 = (u_char *)(env->functions)->GetByteArrayElements(env, out1, NULL);

	int	nData,	nData1,	state;

//	state = m_vg->GetUpdate( outData, &nData, fl );

//	state = m_vg->GetOutput( outData, &nData );
	state = m_vg->GetOutput( outData, &nData, outData1, &nData1 );

	(env->functions)->ReleaseByteArrayElements(env, out, (jbyte *)outData, 0);

	(env->functions)->ReleaseByteArrayElements(env, out1, (jbyte *)outData1, 0);




//	GPLOGF((" >"));
	return( state );
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_start
  (JNIEnv * env, jclass c, jint operation )
{

	m_vg->StartTask();

	return( 1 );
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_stop
  (JNIEnv * env, jclass c, jint operation )
{

	m_vg->StopTask();

	return( 1 );
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_setOperation
  (JNIEnv * env, jclass c, jint operation )
{

	m_vg->SetOperation( operation );
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_setFrameRate
	(JNIEnv * env, jclass c, jint frameRate )
{

	m_vg->SetFrameRate( frameRate );
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_setColor
  (JNIEnv * env, jclass c, jint color )
{

	m_vg->SetBackground( color );
}



JNIEXPORT jint JNICALL  Java_com_homage_matting_Matting_setBackgound
  (JNIEnv * env, jclass c, jint width, jint height, jbyteArray buffer )
{


	if( m_iaq == NULL )
		return( -1 );


	u_char *data = (u_char *)(env->functions)->GetByteArrayElements(env, buffer, NULL);
	if( data == NULL )
		return( -1 );


	image_type *im = image_create( height, width, 3, 1, data );
	m_vg->SetBackground( im );

	image_destroy( im, 0 );


	(env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *)data, 0);



	return( 1 );
}





JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_startRecord
	(JNIEnv * env, jclass c, jstring jdir )
{

	GPLOGF(("<startRecord"));
	const char *dir = ujni_getJString( env, jdir );

	GPLOGF(("%s", dir ));

	m_vg->StartRecord( (char *)dir );


	ujni_releaseJString( env, jdir, dir );

	GPLOGF((" >\n"));

	return( 1 );
}



JNIEXPORT jint JNICALL Java_com_homage_matting_Matting_stopRecord
	(JNIEnv * env, jclass c )
{
	GPLOGF(("<stopRecord"));

	m_vg->StopRecord();

	GPLOGF((">\n"));

	return( 1 );
}