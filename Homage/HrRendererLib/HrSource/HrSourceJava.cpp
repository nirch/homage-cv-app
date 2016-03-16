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
	
	m_im = NULL;

	//Init();
}



CHrSourceJava::~CHrSourceJava()
{
	DeleteContents();
}





void CHrSourceJava::DeleteContents()
{
	Close();

	//if( m_im != NULL ){
	//	image_destroy( m_im, 1 );
	//	m_im = NULL;
	//}
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

	m_methodGetInfo = m_env->GetMethodID(clazz, "getInfo", "()[I" );
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
	jintArray infoRet = (jintArray)m_env->CallObjectMethod( m_jSrc, m_methodGetInfo );
	jint *info = (jint *)m_env->GetIntArrayElements(infoRet, NULL);

	m_width = info[0];
	m_height = info[1];

	m_env->ReleaseIntArrayElements(infoRet, info, 0);
}

int CHrSourceJava::ReadFrame( int iFrame, long long timeStamp, image_type **im )
{
	
	GPLOGF( ("<java ReadFrame" ));


	jbyteArray dataArr = (jbyteArray)m_env->CallObjectMethod( m_jSrc, m_methodRead, timeStamp);

	if (dataArr == NULL)
		return -1;

	jbyte *pixles = (jbyte *)m_env->GetByteArrayElements(dataArr, NULL);

	if (m_im == NULL)
		m_im = image_create(m_height, m_width, 4, 1, NULL);

	memcpy(m_im->data, pixles, m_width * m_height * m_im->channel);

	m_env->ReleaseByteArrayElements(dataArr, pixles, 0);

	// Process effects.
	ProcessEffect( m_im, iFrame, im );

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

	if (m_im != NULL){
		image_destroy(m_im, 1);
		m_im = NULL;
	}

	return( ret );
}