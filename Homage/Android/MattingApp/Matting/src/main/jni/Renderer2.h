/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_mychic_vimglasses_VimGlasses */

#ifndef _Included_com_homage_matting_Renderer2
#define _Included_com_homage_matting_Renderer2

#ifdef __cplusplus
extern "C" {
#endif

/*JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_init (JNIEnv *, jclass );
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_calc( JNIEnv* env, jclass c, jobject obj);
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_writeFrame( JNIEnv * env, jclass c, jobject obj);
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_readFrame( JNIEnv * env, jclass c, jobject obj);*/

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_create(JNIEnv *, jclass, jfloat, jint);
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_addImageSource(JNIEnv *, jclass, jint, jint, jstring);
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_addFX(JNIEnv *, jclass, jint, jint, jint, jstring);
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_addOutput(JNIEnv *, jclass, jint, jint, jstring, jint, jint, jint);
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_process(JNIEnv *, jclass, jint, jint, jint);
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_delete(JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif
