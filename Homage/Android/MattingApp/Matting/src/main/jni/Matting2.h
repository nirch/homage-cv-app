/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_mychic_vimglasses_VimGlasses */

#ifndef _Included_com_homage_matting_Matting2
#define _Included_com_homage_matting_Matting2

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Class:     com_mychic_vimglasses_VimGlasses
 * Method:    init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_init
  (JNIEnv *, jclass, jstring, jstring, jint, jint, jint);

JNIEXPORT jint JNICALL  Java_com_homage_matting_Matting2_release
        (JNIEnv *, jclass);

JNIEXPORT jint JNICALL  Java_com_homage_matting_Matting2_getFormattedImage
        (JNIEnv *, jclass, jint, jbyteArray, jint, jint, jint, jbyteArray);

JNIEXPORT jint JNICALL  Java_com_homage_matting_Matting2_process
        (JNIEnv *, jclass, jint, jbyteArray, jint, jint, jint , jbyteArray, jbyteArray);


#ifdef __cplusplus
}
#endif
#endif