/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_mychic_vimglasses_VimGlasses */

#ifndef _Included_com_homage_recorder_PngRecorder
#define _Included_com_homage_recorder_PngRecorder

#ifdef __cplusplus
extern "C" {
#endif




/*
 * Class:     com_mychic_vimglasses_VimGlasses
 * Method:    init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_homage_recorder_PngRecorder_start
		(JNIEnv *, jclass, jstring file );



JNIEXPORT jint JNICALL Java_com_homage_recorder_PngRecorder_record
	(JNIEnv *, jclass, jint width, jint height, jbyteArray  out );


JNIEXPORT jint JNICALL Java_com_homage_recorder_PngRecorder_stop
	(JNIEnv *, jclass );





#ifdef __cplusplus
}
#endif
#endif