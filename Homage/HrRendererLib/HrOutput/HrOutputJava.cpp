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
	
//	m_im = NULL;

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



int CHrOutputJava::Init( char *outFile, int width, int height, int frameRate )
{	
	m_width = width;
	m_height = height;

	
	strcpy( m_file, outFile );

	GPLOGF( ("  call open hh\n" ));

	jstring jfile = m_env->NewStringUTF( m_file );

	GPLOGF( (" %s  %d %d %d \n", m_file, width, height, frameRate ));




//	int ret2 = m_env->CallIntMethod( m_obj, m_methodClose );
//	GPLOGF( ("  call close succeed \n" ));


	int ret = m_env->CallIntMethod( m_obj, m_methodOpen, width, height, frameRate, jfile );
	GPLOGF( ("  call open succeed \n" ));



	//WriteFrame( "123456", 5 );
	//GPLOGF( ("  call write succeed \n" ));

	m_env->DeleteLocalRef( jfile );

	GPLOG( ("call open %d\n", 1 ));

	return( 1 );
}




int CHrOutputJava::SetJavaEncoder( JNIEnv* env, jobject mainJavaClass )
{

	m_env = env;
	m_obj = mainJavaClass;

//	strcpy( m_className, className );

	GPLOGF( ("<SetJavaEncoder \n" ));
	//jclass cls = (m_env)->FindClass( className );
	//if( cls == NULL )	return( -1 );

	jclass clazz = (m_env)->GetObjectClass(mainJavaClass); 


	if( clazz == NULL ){
		GPLOGF( ("FindClass failed\n" ));
		return( -1 );
	}


	GPLOGF( ("FindClass succeed\n" ));


	GPLOGF( ("close id\n" ));
	m_methodClose = (m_env)->GetMethodID(clazz, "close", "()I");
	if( m_methodClose == NULL )	return( -1 );

	GPLOGF( ("close exist hh\n" ));




	m_methodWrite = m_env->GetMethodID(clazz, "write", "(II[B)I");
	if( m_methodWrite == NULL )	return( -1 );
	GPLOGF( ("write exist\n" ));


	m_methodOpen = (m_env)->GetMethodID(clazz, "open", "(IIILjava/lang/String;)I" );
	if( m_methodOpen == NULL )	return( -1 );

	GPLOGF( ("open exist\n" ));









	return( 1 );
}


int CHrOutputJava::WriteFrame( image_type *im, int iFrame )
{
	
	WriteFrameA( "123456", 5 );


	GPLOGF( ("<java WriteFrame" ));

	u_char *data = im->data;
	int nData = im->width*im->height*im->channel;

	GPLOGF( ("1  %d  %d %d %d  \n", nData, im->width, im->height, im->channel ));
	jbyteArray jdata = ujni_createJByteArray( m_env, (char *)data, nData );

	if( jdata == NULL )
		GPLOGF( ("jdata = NULL" ));
	GPLOGF( ("2" ));


	int	width = im->width;
	int	height = im->height;

	GPLOGF( ("3" ));

	int ret = m_env->CallIntMethod( m_obj, m_methodWrite, width, height, jdata );

	GPLOGF( ("4" ));
	

	///ujni_releaseJByteArray( m_env, jdata );

	GPLOGF( (" %d\n>", ret ));

	return( 1 );
}


int CHrOutputJava::WriteFrameA( char *data, int nData )
{
	GPLOGF( ("write 1\n" ));
	jbyteArray jdata = ujni_createJByteArray( m_env, (char *)data, nData );

	if( jdata == NULL )
		GPLOGF( ("jdata == NULL\n" ));

	GPLOGF( ("write 2\n" ));

	if( m_methodWrite == NULL )
		GPLOGF( ("m_methodWrite == NULL\n" ));

	m_env->CallIntMethod( m_obj, m_methodWrite, 10, 10, jdata );

	GPLOGF( ("write 3\n" ));

//	ujni_releaseJByteArray( m_env, jdata );

	GPLOGF( ("write 4\n" ));

	return( 1 );
}


int	CHrOutputJava::Close()
{

	if( m_env != NULL ){
		int ret = m_env->CallIntMethod( m_obj, m_methodClose );
		GPLOGF( (" java close %d \n", ret ));
	}


	//if( m_im != NULL ){
	//	image_destroy( m_im, 1 );
	//	m_im = NULL;
	//}


	return( 1 );
}