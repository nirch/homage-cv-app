/********************
 *** UjniTool.c   ***
 ********************/


#include	<string.h>
#include "Uigp/igp.h"
#include "UjniTool.h"


jobject
ujni_newObject(JNIEnv *env, char *className, char *method_sig, jvalue *args)
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



jobjectArray
ujni_newObjectArray(JNIEnv *env, char *className, int arrLen)
{
	jclass cls = env->FindClass(className);
	if (cls)
	{

		return env->NewObjectArray(arrLen, cls, NULL);
	}
	return NULL;
}


void
ujni_releaseJArray(JNIEnv *env, jintArray arr, jint *data, int len)
{
	if (env == NULL) return;
	env->ReleaseIntArrayElements(arr, data, len);
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}
}

void
ujni_releaseJArray(JNIEnv *env, jbyteArray arr, jbyte *data, int len)
{
	if (env == NULL) return;
	env->ReleaseByteArrayElements(arr, data, len);
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}
}


jbyteArray
ujni_createJByteArray(JNIEnv *env, char *data, int len)
{
	jbyteArray byteArr = env->NewByteArray(len);
	if (!byteArr)	return( NULL);


	env->SetByteArrayRegion(byteArr, 0, len, (jbyte *)data );

	//delete jdata;

	return byteArr;
}


//void
//ujni_releaseJByteArray(JNIEnv *env, jbyteArray arr )
//{
//jbyte *data;
//int len;
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

void
	ujni_releaseJByteArray(JNIEnv *env, jbyteArray arr )
{
	jbyte *data = NULL;
	int len = 1;

	if (env == NULL) return;

	env->ReleaseByteArrayElements(arr, data, len);

	if (data != NULL){
		delete data;
		data = NULL;
	}
}



int
ujni_getJByteArrayData(JNIEnv *env, jbyteArray byteArr, jbyte **data, int *len)
{
	*len = env->GetArrayLength( byteArr );
	// env->GetArrayLength( byteArr );
	*data = new jbyte[*len];
	env->GetByteArrayRegion(byteArr, 0, *len, *data );


	//*data = new char[*len];
	//for (int i = 0; i < *len; i++)
		//(*data)[i] = jdata[i];


	//delete jdata;

	return 1;
}

int
ujni_getJIntArrayData(JNIEnv *env, jintArray byteArr, jint **data, int *len)
{
	*len = env->GetArrayLength( byteArr);
	env->GetArrayLength( byteArr);
	//jint *jdata = new jint[*len];

	env->GetIntArrayRegion(byteArr, 0, *len, *data );


	//*data = new jint[*len];
	//for (int i = 0; i < *len; i++)
		//(*data)[i] = jdata[i];


	//delete jdata;

	return 1;
}


jstring
ujni_createJString( JNIEnv *env, char *data, int len )
{

/*

	jvalue args[2];
	args[0].l = ujni_createJByteArray( env, data, len );

	args[1].l = NULL;


	jstring jstr = (jstring)ujni_newObject(
			env,
			"java.lang.String",
			"([B)V",
			args);


	return( jstr );
*/
	if( len <= 0 )
		return( env->NewStringUTF(data) );

	char buf[256];
	strncpy( buf, data, len );
	return( env->NewStringUTF(buf) );
}


const char *
ujni_getJString(JNIEnv *env, jstring js)
{
	if (js==NULL) {
		return NULL;
	} else {
		return env->GetStringUTFChars(js, NULL);
	}
}

// strings returned from getJString and createJString must be released!
void
ujni_releaseJString(JNIEnv *env, jstring jstr, const char *str)
{
	if ((env == NULL) || (jstr == NULL)) return;

	env->ReleaseStringUTFChars(jstr, str);
}


char *
ujni_convertJByteToChar( jbyte *data, int len )
{
	char *dataChar = new char[len];
	for (int i = 0; i < len; i++)
		dataChar[i] = data[i];

	return dataChar;
}


void
ujni_throwException(JNIEnv *env, const char *exceptionClass, const char *message)
{
	if ((env == NULL) || (exceptionClass == NULL)) return;
	jclass ex = env->FindClass(exceptionClass);
	env->ThrowNew(ex, message);
}

void
ujni_throwComposerException(JNIEnv *env, const char *message)
{
	ujni_throwException(env, "com/vimatix/composer/ComposerException", message);
}

void
ujni_throwComposerParamException(JNIEnv *env, const char *message)\
{
	ujni_throwException(env, "com/vimatix/composer/InvalidParamException", message);
}
