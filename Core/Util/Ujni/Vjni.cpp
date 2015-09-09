/********************
 *** UjniTool.c   ***
 ********************/


#include	<string.h>
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





jbyteArray 
ujni_createJByteArray(JNIEnv *env, char *data, int len)
{

	jbyte *jdata = new jbyte[len];
	for (int i = 0; i < len; i++)
		jdata[i] = data[i];


	jbyteArray byteArr = env->NewByteArray(len);
	if (!byteArr)	NULL;


	env->SetByteArrayRegion(byteArr, 0, len, jdata );

	delete jdata;

	return byteArr;
}


int 
ujni_getJByteArrayData(JNIEnv *env, jbyteArray byteArr, char **data, int *len)
{
	*len = env->GetArrayLength( byteArr);
	env->GetArrayLength( byteArr);
	jbyte *jdata = new jbyte[*len];

	env->GetByteArrayRegion(byteArr, 0, *len, jdata );


	*data = new char[*len];
	for (int i = 0; i < *len; i++)
		(*data)[i] = jdata[i];


	delete jdata;

	return 1;
}

int 
ujni_getJIntArrayData(JNIEnv *env, jintArray byteArr, int **data, int *len)
{
	*len = env->GetArrayLength( byteArr);
	env->GetArrayLength( byteArr);
	jint *jdata = new jint[*len];

	env->GetIntArrayRegion(byteArr, 0, *len, jdata );


	*data = new int[*len];
	for (int i = 0; i < *len; i++)
		(*data)[i] = jdata[i];


	delete jdata;

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
