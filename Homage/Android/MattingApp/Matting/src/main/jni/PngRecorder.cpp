#include <jni.h>


#include "PngRecorder.h"


#define _GPLOG

#include "Uigp/igp.h"
#include "Ulog/Log.h"

#include	"UjniTool.h"

#include "ImageType/ImageType.h"

#include "PngCodec/PngWriter/PngWriter.h"




static CPngWriter *m_recorder = NULL;


					   
JNIEXPORT jint JNICALL Java_com_homage_recorder_PngRecorder_start
	(JNIEnv * env, jclass c, jstring jdir )
{

		GPLOGF(("<start"));
	const char *dir = ujni_getJString( env, jdir );

	GPLOGF(("%s", dir ));

	m_recorder = new CPngWriter();

	m_recorder->Open( 100, (char *)dir );

	ujni_releaseJString( env, jdir, dir );

		GPLOGF((" >"));

	return( 1 );
}


JNIEXPORT jint JNICALL Java_com_homage_recorder_PngRecorder_record
	(JNIEnv * env, jclass c, jint width, jint height, jbyteArray  buffer )
{
	GPLOGF(("<record"));
	u_char *data = (u_char *)(env->functions)->GetByteArrayElements(env, buffer, NULL);
	if( data == NULL )
		return( -1 );


	image_type *im = image_create( height, width, 4, 1, data );
	im->timeStamp = vTime();
	m_recorder->WriteFrame( im );

	image_destroy( im, 0 );

	(env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *)data, 0);

	GPLOGF((" >"));

	return( 1 );

}


JNIEXPORT jint JNICALL Java_com_homage_recorder_PngRecorder_stop
	(JNIEnv * env, jclass c )
{
	GPLOGF(("<stop"));

	if( m_recorder == NULL )
		return( 1 );

	m_recorder->Close();

	delete m_recorder;

	m_recorder = NULL;


	GPLOGF(("Stop>/n"));

	return( 1 );
}
