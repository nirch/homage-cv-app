#ifndef __UJNITOOL_H__
#define __UJNITOOL_H__

#include <jni.h>

jobject			ujni_newObject(JNIEnv *env, char *className, char *method_sig, jvalue *args);

jobjectArray	ujni_newObjectArray(JNIEnv *env, char *className, int arrLen);
jobjectArray	ujni_createObjectArray(JNIEnv *env, char *className, int length );

jbyteArray		ujni_createJByteArray(JNIEnv *env, char *data, int len);
void			ujni_releaseJByteArray(JNIEnv *env, jbyteArray arr );


int				ujni_getJByteArrayData(JNIEnv *env, jbyteArray byteArr, jbyte **data, int *len);
int				ujni_getJIntArrayData(JNIEnv *env, jintArray byteArr, jint **data, int *len);
void			ujni_releaseJArray(JNIEnv *env, jintArray arr, jint *data, int len);
void			ujni_releaseJArray(JNIEnv *env, jbyteArray arr, jbyte *data, int len);

jstring			ujni_createJString( JNIEnv *env, char *s, int length );
const char *	ujni_getJString( JNIEnv *env, jstring jstr );
void			ujni_releaseJString( JNIEnv *env, jstring jstr, const char *str );

char *			ujni_convertJByteToChar( jbyte *data, int len );

void			ujni_throwException(JNIEnv *env, const char *exceptionClass, const char *message);
void			ujni_throwComposerException(JNIEnv *env, const char *message);
void			ujni_throwComposerParamException(JNIEnv *env, const char *message);

#endif // __UJNITOOL_H__