#include <jni.h>

#include	"Matting2.h"

#define _GPLOG

#include "Uigp/igp.h"
#include "Ulog/Log.h"

#include	"UjniTool.h"


#include "MattingLib/UniformBackground/UniformBackground.h"
#include "../Core/ImageLib/ImageType/ImageType.h"
#include "../Core/ImageLib/YUV/Image2YUV.h"
#include "../Core/ImageLib/ImageType/ImageTool.h"


static int m_intWidth;
static int m_intHeight;
static int m_intThreadsCount;

static CUniformBackground **m_ub;
static image_type **m_imgYUV;
static image_type **m_imgRGB;
static image_type **m_imgRGBCropped;
static image_type **m_imgAndroid;


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_init
        (JNIEnv * env, jclass c, jstring jprmFile, jstring jctrFile, jint width, jint height,
         jint threadsCount) {
    GPLOGF(("<  init-J ..."));

    const char *prmFile = ujni_getJString(env, jprmFile);
    const char *ctrFile = ujni_getJString(env, jctrFile);

    if (m_ub == NULL) {

        m_intWidth = width;
        m_intHeight = height;
        m_intThreadsCount = threadsCount;

        m_ub = new CUniformBackground*[threadsCount];
        m_imgYUV = new image_type*[threadsCount]; // First height because this is how the parameters of the method
        m_imgRGB = new image_type*[threadsCount];
        m_imgRGBCropped = new image_type*[threadsCount];
        m_imgAndroid = new image_type*[threadsCount];


        for (int i = 0; i < threadsCount; ++i) {
            m_ub[i] = new CUniformBackground();
            m_ub[i]->Init((char *) prmFile, (char *) ctrFile, width, height);

            m_imgYUV[i] = NULL;
            m_imgRGB[i] = NULL;
            m_imgRGBCropped[i] = NULL;
            m_imgAndroid[i] = NULL;
        }
    }

    ujni_releaseJString(env, jprmFile, prmFile);
    ujni_releaseJString(env, jctrFile, ctrFile);

    GPLOGF((" >\n"));

    return 1;
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_release
        (JNIEnv *env, jclass c) {

    for (int i = 0; i < m_intThreadsCount; ++i) {

        if (m_ub[i]) {
            delete m_ub[i];
            m_ub[i] = NULL;
        }
        if (m_imgYUV[i]) {
            image_destroy(m_imgYUV[i], 1);
            m_imgYUV[i] = NULL;
        }

        if (m_imgRGB[i]) {
            image_destroy(m_imgRGB[i], 1);
            m_imgRGB[i] = NULL;
        }

        if (m_imgRGBCropped[i]) {
            image_destroy(m_imgRGBCropped[i], 1);
            m_imgRGBCropped[i] = NULL;
        }

        if (m_imgAndroid[i]) {
            image_destroy(m_imgAndroid[i], 1);
            m_imgAndroid[i] = NULL;
        }
    }

    return 1;
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_getFormattedImage
        (JNIEnv *env, jclass c, jint threadIdx, jbyteArray buffer, jint width, jint height, jint orientation,
         jbyteArray resRGBAImage) {

    u_char *data = (u_char * )(env->functions)->GetByteArrayElements(env, buffer, NULL);
    u_char *resRGBA = (u_char * )(env->functions)->GetByteArrayElements(env, resRGBAImage, NULL);

    if (m_imgYUV[threadIdx] == NULL)
        m_imgYUV[threadIdx] = image_createYUV420(height, width, NULL);

    memcpy(m_imgYUV[threadIdx]->data, data, width * height * 1.5);

    switch (orientation) {
        case 0:
            m_imgRGB[threadIdx] = imageNV21_to_RGB(m_imgYUV[threadIdx], m_imgRGB[threadIdx]);
            break;
        case 90:
            m_imgRGB[threadIdx] = imageNV21_to_RGB_swap(m_imgYUV[threadIdx], m_imgRGB[threadIdx]);
            image_flipV(m_imgRGB[threadIdx]);
            break;
        case 270:
            m_imgRGB[threadIdx] = imageNV21_to_RGB_swap(m_imgYUV[threadIdx], m_imgRGB[threadIdx]);
            break;
    }

    m_imgRGBCropped[threadIdx] = image_crop(m_imgRGB[threadIdx], 0, 0, m_intWidth, m_intHeight, m_imgRGBCropped[threadIdx]);

    //image_flipH(m_imgRGBCropped[threadIdx]);

    // Returning the rgba Image
    //m_imgAndroid[threadIdx] = image_android(m_imgRGBCropped[threadIdx], m_imgAndroid[threadIdx]);
    //memcpy(resRGBA, m_imgAndroid[threadIdx]->data, m_intWidth * m_intHeight * 4);

    memcpy(resRGBA, m_imgRGBCropped[threadIdx]->data, m_intWidth * m_intHeight * 3);

    (env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *) data, 0);
    (env->functions)->ReleaseByteArrayElements(env, resRGBAImage, (jbyte *) resRGBA, 0);

    return 1;
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_process
        (JNIEnv *env, jclass c, jint threadIdx, jbyteArray buffer, jint width, jint height, jint orientation,
         jbyteArray resRGBAImage, jbyteArray resMaskChannel) {
    if (m_ub[threadIdx] == NULL)
        return (-1);

//	GPLOGF( (" 1") );
    u_char *data = (u_char * )(env->functions)->GetByteArrayElements(env, buffer, NULL);
    u_char *resRGBA = (u_char * )(env->functions)->GetByteArrayElements(env, resRGBAImage, NULL);
    u_char *resMask = (u_char * )(env->functions)->GetByteArrayElements(env, resMaskChannel, NULL);
    if (!data || !resRGBA || !resMask)
        return (-1);

    if (m_imgYUV[threadIdx] == NULL)
        m_imgYUV[threadIdx] = image_createYUV420(height, width, NULL);

    memcpy(m_imgYUV[threadIdx]->data, data, width * height * 1.5);

    switch (orientation) {
        case 0:
            m_imgRGB[threadIdx] = imageNV21_to_RGB(m_imgYUV[threadIdx], m_imgRGB[threadIdx]);
            break;
        case 90:
            m_imgRGB[threadIdx] = imageNV21_to_RGB_swap(m_imgYUV[threadIdx], m_imgRGB[threadIdx]);
            image_flipV(m_imgRGB[threadIdx]);
            break;
        case 270:
            m_imgRGB[threadIdx] = imageNV21_to_RGB_swap(m_imgYUV[threadIdx], m_imgRGB[threadIdx]);
            break;
    }

    m_imgRGBCropped[threadIdx] = image_crop(m_imgRGB[threadIdx], 0, 0, m_intWidth, m_intHeight, m_imgRGBCropped[threadIdx]);

    //image_flipH(m_imgRGBCropped[threadIdx]);

    // Returning the rgba Image
    //m_imgAndroid[threadIdx] = image_android(m_imgRGBCropped[threadIdx], m_imgAndroid[threadIdx]);
    //memcpy(resRGBA, m_imgAndroid[threadIdx]->data, m_intWidth * m_intHeight * 4);

    memcpy(resRGBA, m_imgRGBCropped[threadIdx]->data, m_intWidth * m_intHeight * 3);


    // Returning the mask alpha single channel - Image
    image_type *resImg;
    m_ub[threadIdx]->Process(m_imgRGBCropped[threadIdx], 1, &resImg);
    memcpy(resMask, resImg->data, m_intWidth * m_intHeight);

    (env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *) data, 0);
    (env->functions)->ReleaseByteArrayElements(env, resRGBAImage, (jbyte *) resRGBA, 0);
    (env->functions)->ReleaseByteArrayElements(env, resMaskChannel, (jbyte *) resMask, 0);

    return 123;
}