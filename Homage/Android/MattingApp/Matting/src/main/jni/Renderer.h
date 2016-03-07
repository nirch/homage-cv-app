/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_mychic_vimglasses_VimGlasses */

#ifndef _Included_com_homage_matting_Renderer
#define _Included_com_homage_matting_Renderer

#ifdef __cplusplus
extern "C" {
#endif




/*
 * Class:     com_mychic_vimglasses_VimGlasses
 * Method:    init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_create
  (JNIEnv *, jclass );



JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_addSource
	(JNIEnv * env, jclass c, jint iR, jint type, jstring file );

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_addSourceFrame
	(JNIEnv * env, jclass c, jint iR, jint type, jstring jfile, jintArray jframe );


JNIEXPORT jint JNICALL  Java_com_homage_renderer_Renderer_addEffect
		(JNIEnv * env, jclass c, jint iR, jint sourceId, jint type, jstring file );



JNIEXPORT jint JNICALL  Java_com_homage_renderer_Renderer_addEffectData
	(JNIEnv * env, jclass c, jint iR, jint sourceId, jint type, jstring jdata );

JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_addOutput
		(JNIEnv * env, jclass c, jint iR, jint type, jstring file, jint width, jint height, int fjrameSpeed );


/*JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_addJavaOutput
	(JNIEnv * env, jclass c, jint iR, jobject obj,
	jstring jfile, jint width, jint height, jint frameRate );


JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_addJavaOutputA
	(JNIEnv * env, jclass c, jint iR, jstring className,
	jstring jfile, jint width, jint height, jint frameRate );*/



JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_setOutputPallete
	(JNIEnv * env, jclass c, jint iR, int id, jbyteArray jdata );


JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_setFrameSize
	(JNIEnv * env, jclass c, jint iR, jint width, jint height );

JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_process
	(JNIEnv * env, jclass c, jint iR );



#ifdef __cplusplus
}
#endif
#endif
