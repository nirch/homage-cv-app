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


#include "HrOutputI.h"


#include "HrOutputJava.h"


CHrOutputJava::CHrOutputJava()
{
	m_frame = NULL;
}



CHrOutputJava::~CHrOutputJava()
{
	DeleteContents();
}





void CHrOutputJava::DeleteContents()
{
	Close();

	//if( m_im != NULL ){
	//	image_destroy( m_im, 1 );
	//	m_im = NULL;
	//}
}


int CHrOutputJava::SetJavaOutput( JNIEnv *env, jobject javaOutput )
{

	m_env = env;
	m_jOutput = env->NewGlobalRef(javaOutput);


	GPLOGF( ("<SetJavaOutput \n" ));

	jclass clazz = (m_env)->GetObjectClass(m_jOutput);


	if( clazz == NULL ){
		GPLOGF( ("FindClass failed\n" ));
		return( -1 );
	}


	GPLOGF( ("FindClass succeed\n" ));


	GPLOGF( ("close id\n" ));
	m_methodClose = m_env->GetMethodID(clazz, "close", "()I");
	if( m_methodClose == NULL )	return( -1 );

	GPLOGF( ("close exist hh\n" ));

	m_methodWrite = m_env->GetMethodID(clazz, "write", "([BI)I");
	if( m_methodWrite == NULL )	return( -1 );
	GPLOGF( ("write exist\n" ));


	m_methodOpen = m_env->GetMethodID(clazz, "open", "()I" );
	if( m_methodOpen == NULL )	return( -1 );


	GPLOGF( ("open exist\n" ));

	return( 1 );
}

int	CHrOutputJava::Open()
{
	int ret = -1;
	if( m_env != NULL ){
		ret = m_env->CallIntMethod( m_jOutput, m_methodOpen );
		GPLOGF( (" java open %d \n", ret ));
	}

	return( ret );
}

int CHrOutputJava::WriteFrame( image_type *im, int iFrame )
{

	GPLOGF( ("<java ReadFrame" ));

	if (m_frame == NULL) {
		m_frameLen = im->width * im->height * im->channel;
		m_frame = m_env->NewByteArray(m_frameLen);
	}

	m_env->SetByteArrayRegion(m_frame, 0, m_frameLen, (jbyte*)im->data );

	jint ret = m_env->CallIntMethod( m_jOutput, m_methodWrite, m_frame, iFrame);

	//m_env->ReleaseByteArrayElements(m_frame, (jbyte*)im->data, 0);

	GPLOGF( (" %d\n>", ret ));

	return( ret );
}

int	CHrOutputJava::Close()
{
	int ret = -1;
	if( m_env != NULL ){
		ret = m_env->CallIntMethod( m_jOutput, m_methodClose );
		GPLOGF( (" java close %d \n", ret ));

		m_env->DeleteGlobalRef(m_jOutput);
		m_jOutput = NULL;
		m_env = NULL;
	}

	return( ret );
}