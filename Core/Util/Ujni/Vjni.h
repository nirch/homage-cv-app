#ifndef __UJNITOOL_H__
#define __UJNITOOL_H__

#include <jni.h>


jobjectArray	ujni_newObjectArray(JNIEnv *env, char *className, int arrLen);

jobject		ujni_newObject(JNIEnv *env, char *className, char *method_sig, jvalue *args);



jbyteArray ujni_createJByteArray(JNIEnv *env, char *data, int len);

void	ujni_releaseJByteArray(JNIEnv *env, jbyteArray arr );


int		ujni_getJByteArrayData(JNIEnv *env, jbyteArray byteArr, char **data, int *len);
int		ujni_getJIntArrayData(JNIEnv *env, jintArray byteArr, int **data, int *len);



jstring	ujni_createJString( JNIEnv *env, char *s, int length );

const char *	ujni_getJString( JNIEnv *env, jstring jstr );



jobjectArray	ujni_createObjectArray(JNIEnv *env, char *className, int length );


#endif // __UJNITOOL_H__