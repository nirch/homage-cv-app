#ifndef __VIM_JNI_H__
#define __VIM_JNI_H__


#ifdef _DEBUG 
//#pragma comment( lib, "jvm.lib" )
#else
//#pragma comment( lib, "jvm.lib" )
#endif



#include <jni.h>


class CVimJni
{
public:
	CVimJni() {};
	virtual ~CVimJni() {};

	// Create object
	static jobject		CreateObject(JNIEnv *env, char *className, char *method_sig, jvalue *args);

	jobject CreateObject(JNIEnv *env, jclass cls, char *method_sig, jvalue *args );


	static jobjectArray	CreateObjectArray(JNIEnv *env, char *className, int length );

	static jobjectArray CreateObjectArray(JNIEnv *env, jclass cls, int arrLen );


	static jbyteArray CreateJByteArray(JNIEnv *env, char *data, int len);

	static jstring	CreateJString( JNIEnv *env, char *s, int length );


	// get jni variable data
	int GetJByteArrayData( JNIEnv* env, jobject object, jclass cls, char *method,
						   char **data, int *bytes );

	static int		GetJByteArrayData(JNIEnv *env, jbyteArray byteArr, char **data, int *bytes);
	static int		GetJIntArrayData(JNIEnv *env, jintArray byteArr, int **data, int *bytes);


	static char * GetJStringData( JNIEnv* env, jobject object, jclass cls, char *method );

	static char *GetJStringData(JNIEnv *env, jstring js );

	static int	GetJStringData( JNIEnv* env, jobject object, jclass cls, char *method,
						char *data, int len  );

	static int	GetJStringData(JNIEnv *env, jstring js, char *data, int len );


	int	GetJIntData( JNIEnv* env, jobject object, jclass cls, char *method );

	int GetJByteData( JNIEnv* env, jobject object, jclass cls, char *method );


	int	GetJBoolData( JNIEnv* env, jobject object, jclass cls, char *method );


	// get by calling to class method
	static jstring GetJstring( JNIEnv* env, jobject object, jclass cls, char *method );

	static jbyteArray	GetJbyteArray( JNIEnv* env, jobject object, jclass cls, char *method );
};




#endif // __UJNITOOL_H__