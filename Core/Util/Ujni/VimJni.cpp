/********************
 *** VimJni.c   ***
 ********************/
#include	<string.h>
#include	<stdlib.h>

#include "Uigp\igp.h"

#include "VimJni.h"


jobject 
CVimJni::CreateObject(JNIEnv *env, char *className, char *method_sig, jvalue *args)
{
	jclass cls = env->FindClass(className);
	if (cls)
	{
		jmethodID method = env->GetMethodID(cls, "<init>", method_sig);
		if (method)
		{
			return env->NewObjectA(cls, method, args);
		}
	}
	return NULL;
}


jobject 
CVimJni::CreateObject(JNIEnv *env, jclass cls, char *method_sig, jvalue *args)
{
	jmethodID method = env->GetMethodID(cls, "<init>", method_sig);
	if (method)
		return env->NewObjectA(cls, method, args);

	return NULL;
}



jobjectArray 
CVimJni::CreateObjectArray(JNIEnv *env, char *className, int arrLen)
{
	jclass cls = env->FindClass(className);
	if (cls)
	{
		
		return env->NewObjectArray(arrLen, cls, NULL);
	}
	return NULL;
}

jobjectArray 
CVimJni::CreateObjectArray(JNIEnv *env, jclass cls, int arrLen )
{
	return env->NewObjectArray(arrLen, cls, NULL);
}


jbyteArray 
CVimJni::CreateJByteArray(JNIEnv *env, char *data, int len )
{
	jbyteArray byteArr = env->NewByteArray(len);

	if (env->ExceptionOccurred() != 0) { 
		fprintf(stderr, "Unexpected exception ");
		env->ExceptionDescribe();
		env->ExceptionClear();
		return( NULL );
	}

	if (byteArr == NULL)	return NULL;

	env->SetByteArrayRegion(byteArr, 0, len, (signed char *)data );


	return byteArr;
}


//void
//ujni_releaseJByteArray(JNIEnv *env, jbyteArray arr )
//{
//	jbyte *data;
//	int len = 1;
//
//	if (env == NULL) return;
//
//	env->ReleaseByteArrayElements(arr, data, len);
//
//	if (data != NULL)
//	{
//		delete data;
//		data = NULL;
//	}
//}


jstring 
CVimJni::CreateJString( JNIEnv *env, char *data, int len )
{
char	buf[256];

	if( len <= 0 )
		return( env->NewStringUTF(data) );


	if( len > 255 )	len = 255;
	strncpy( buf, data, len );

	return( env->NewStringUTF(buf) );
}





int 
CVimJni::GetJByteArrayData(JNIEnv *env, jbyteArray byteArr, char **data, int *bytes)
{
	*bytes = env->GetArrayLength(byteArr);
	*data = ( char * )malloc( *bytes + 1 );

	env->GetByteArrayRegion(byteArr, 0, *bytes, (jbyte *)*data);

	return 1;
}



int 
CVimJni::GetJIntArrayData(JNIEnv *env, jintArray byteArr, int **data, int *bytes )
{
	*bytes = env->GetArrayLength( byteArr);
	*data = ( int * )malloc( sizeof(int)*(*bytes + 1) );

	env->GetIntArrayRegion(byteArr, 0, *bytes, (jint*)*data );


	return 1;
}



char *
CVimJni::GetJStringData( JNIEnv* env, jobject object, jclass cls, char *method )
{
	jmethodID methodID = env->GetMethodID(cls, method, "()Ljava/lang/String;" );
	jstring js = (jstring)(env->CallObjectMethod(object, methodID));
	if( js == NULL )	return( NULL );


	const char * strTemp = env->GetStringUTFChars( js, NULL );
	if( strTemp == NULL )	return( NULL );

	char *str = strdup(strTemp);

	env->ReleaseStringUTFChars( js, strTemp );

	return( str );
}

char *
CVimJni::GetJStringData(JNIEnv *env, jstring js )
{
	const char * strTemp = env->GetStringUTFChars( js, NULL );
	if( strTemp == NULL )
		return( NULL );

	char *str = strdup(strTemp);

	env->ReleaseStringUTFChars( js, strTemp );

	return( str );
}


int
CVimJni::GetJStringData( JNIEnv* env, jobject object, jclass cls, char *method,
						char *data, int len  )
{
	jmethodID methodID = env->GetMethodID(cls, method, "()Ljava/lang/String;" );
	jstring js = (jstring)(env->CallObjectMethod(object, methodID));
	if( js == NULL ){
		data[0] = 0;
		return( -1 );
	}

	const char * strTemp = env->GetStringUTFChars( js, NULL );
	if( strTemp == NULL )	return( NULL );

	strncpy( data, strTemp, len );

	env->ReleaseStringUTFChars( js, strTemp );

	return( 1 );
}

int
CVimJni::GetJStringData(JNIEnv *env, jstring js, char *data, int len )
{
	const char * strTemp = env->GetStringUTFChars( js, NULL );
	if( strTemp == NULL )
		return( -1 );

	strncpy( data, strTemp, len );

	env->ReleaseStringUTFChars( js, strTemp );

	return( 1 );
}

int
CVimJni::GetJIntData( JNIEnv* env, jobject object, jclass cls, char *method )
{
	jmethodID methodID = env->GetMethodID(cls, method,"()I");
	int vlaue = env->CallIntMethod(object, methodID);

	return( vlaue );
}

int
CVimJni::GetJByteData( JNIEnv* env, jobject object, jclass cls, char *method )
{
	jmethodID methodID = env->GetMethodID(cls, method,"()B");
	jbyte vlaue = env->CallByteMethod(object, methodID);

	return( vlaue );
}


int
CVimJni::GetJBoolData( JNIEnv* env, jobject object, jclass cls, char *method )
{
	jmethodID methodID = env->GetMethodID(cls, method,"()Z");
	jboolean vlaue = env->CallBooleanMethod(object, methodID);

	return( (int)vlaue );
}

jstring
CVimJni::GetJstring( JNIEnv* env, jobject object, jclass cls, char *method )
{
	jmethodID methodID = env->GetMethodID(cls, method, "()Ljava/lang/String;" );
	jstring js = (jstring)(env->CallObjectMethod(object, methodID));

	return( js );
}


jbyteArray
CVimJni::GetJbyteArray( JNIEnv* env, jobject object, jclass cls, char *method )
{
	jmethodID methodID = env->GetMethodID(cls, method, "()[B");
	jbyteArray arrData = (jbyteArray)(env->CallObjectMethod(object, methodID));

	return( arrData );
}


int
CVimJni::GetJByteArrayData( JNIEnv* env, jobject object, jclass cls, char *method,
						   char **data, int *bytes )
{
	jmethodID methodID = env->GetMethodID(cls, method, "()[B");
	jbyteArray arrData = (jbyteArray)(env->CallObjectMethod(object, methodID));

	*bytes = env->GetArrayLength(arrData);
	*data = ( char * )malloc( *bytes + 1 );

	env->GetByteArrayRegion(arrData, 0, *bytes, (jbyte *)*data);

	return 1;
}
