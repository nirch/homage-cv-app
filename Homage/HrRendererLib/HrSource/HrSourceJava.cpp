//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>


#define _GPLOG


#ifdef _DEBUG
//#define _DUMP
#endif

#include "Ulog/Log.h"

#include "Ujni/Vjni.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"


#include "HrSourceI.h"


#include "HrSourceJava.h"


CHrSourceJava::CHrSourceJava()
{
	m_emptyImage = NULL;
	m_im = NULL;
	lastReadFrameTS = -1;
}



CHrSourceJava::~CHrSourceJava()
{
	Close();
	DeleteContents();
}


void CHrSourceJava::DeleteContents()
{
	if (m_im != NULL){
		image_destroy(m_im, 1);
		m_im = NULL;
	}

	if (m_emptyImage != NULL){
		image_destroy(m_emptyImage, 1);
		m_emptyImage = NULL;
	}
}


int CHrSourceJava::SetJavaSource( JNIEnv *env, jobject javaSource )
{

	m_env = env;
	m_jSrc = env->NewGlobalRef(javaSource);


	GPLOGF( ("<SetJavaSource \n" ));

	jclass clazz = (m_env)->GetObjectClass(m_jSrc);


	if( clazz == NULL ){
		GPLOGF( ("FindClass failed\n" ));
		return( -1 );
	}


	GPLOGF( ("FindClass succeed\n" ));


	GPLOGF( ("close id\n" ));
	m_methodClose = m_env->GetMethodID(clazz, "close", "()I");
	if( m_methodClose == NULL )	return( -1 );

	GPLOGF( ("close exist hh\n" ));

	m_methodRead = m_env->GetMethodID(clazz, "read", "(J)[B");
	if( m_methodRead == NULL )	return( -1 );
	GPLOGF( ("read exist\n" ));


	m_methodOpen = m_env->GetMethodID(clazz, "open", "()I" );
	if( m_methodOpen == NULL )	return( -1 );

	m_methodGetInfo = m_env->GetMethodID(clazz, "getInfo", "()[J" );
	if( m_methodGetInfo == NULL )	return( -1 );

	GPLOGF( ("open exist\n" ));

	return( 1 );
}

int	CHrSourceJava::Open()
{
	int ret = 0;
	if( m_env != NULL ){
		ret = m_env->CallIntMethod( m_jSrc, m_methodOpen );
		GPLOGF( (" java open %d \n", ret ));

		if (ret){
			LoadInfo();
		}
	}

	return( ret );
}

int CHrSourceJava::LoadInfo(){
	jlongArray infoRet = (jlongArray)m_env->CallObjectMethod( m_jSrc, m_methodGetInfo );
	jlong *info = (jlong *)m_env->GetLongArrayElements(infoRet, NULL);

	m_width = (int)info[0];
	m_height = (int)info[1];
	this->SetSourceDuration(info[2]);

	m_env->ReleaseLongArrayElements(infoRet, info, 0);
}

int CHrSourceJava::ReadFrame( int iFrame, long long timeStamp, image_type **im )
{
	
	GPLOGF( ("<java ReadFrame" ));

	long long readTimeStamp = timeStamp;
	if (this->shouldUseTiming) {
		readTimeStamp = this->CalculatedTS(timeStamp);
	}

	if (readTimeStamp == -1) {
		// Creating an empty image frame
		if (m_emptyImage == NULL) {
			m_emptyImage = image_create(m_height, m_width, 4, 1, NULL);
			memset(m_emptyImage->data, 0, m_height * m_width * 4);
		}

		*im = m_emptyImage;
	}
	else {
		if (lastReadFrameTS != readTimeStamp) {
			lastReadFrameTS = readTimeStamp;

			jbyteArray dataArr = (jbyteArray) m_env->CallObjectMethod(m_jSrc, m_methodRead,
																	  readTimeStamp);

			if (dataArr == NULL)
				return -1;

			jbyte *pixels = (jbyte *) m_env->GetByteArrayElements(dataArr, NULL);

			if (m_im == NULL)
				m_im = image_create(m_height, m_width, 4, 1, NULL);

			memcpy(m_im->data, pixels, m_width * m_height * m_im->channel);

			m_env->ReleaseByteArrayElements(dataArr, pixels, 0);
		}

		// Process effects.
		ProcessEffect(m_im, iFrame, readTimeStamp, im);
	}

	GPLOGF( (" %d\n>", 1 ));

	return( 1 );
}

int	CHrSourceJava::Close()
{
	int ret = -1;
	if( m_env != NULL ){
		ret = m_env->CallIntMethod( m_jSrc, m_methodClose );
		GPLOGF( (" java close %d \n", ret ));

		m_env->DeleteGlobalRef(m_jSrc);
		m_jSrc = NULL;
		m_env = NULL;
	}

	return ret;
}