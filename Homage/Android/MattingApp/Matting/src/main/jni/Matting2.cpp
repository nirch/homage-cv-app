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



#define MAX_MATTING 8

static int	m_nM2 = 0;
static CUniformBackground *m_ub[MAX_MATTING];
static int m_intWidth[MAX_MATTING];
static int m_intHeight[MAX_MATTING];
static image_type *m_imgYUV[MAX_MATTING];
static image_type *m_imgRGB[MAX_MATTING];
static image_type *m_imgRGBResized[MAX_MATTING];

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_create
        (JNIEnv * env, jclass c, jstring jprmFile, jstring jctrFile, jint width, jint height) {
    GPLOGF(("<Matting2 Create"));

    int i;
    for (i = 0; i < m_nM2; i++) {
        if (m_ub[i] == NULL)
            break;
    }

    if (i >= MAX_MATTING) return (-1);

    if (i >= m_nM2) m_nM2 = i + 1;

    // Creates the new uniform background
    m_ub[i] = new CUniformBackground();

    // Initializing it
    const char *prmFile = ujni_getJString(env, jprmFile);
    const char *ctrFile = ujni_getJString(env, jctrFile);
    int ret = m_ub[i]->Init((char *) prmFile, (char *) ctrFile, width, height);
    ujni_releaseJString(env, jprmFile, prmFile);
    ujni_releaseJString(env, jctrFile, ctrFile);

    if (ret != 1)
        return -1;

    m_intWidth[i] = width;
    m_intHeight[i] = height;

    // Preparing its members
    m_imgYUV[i] = NULL;
    m_imgRGB[i] = NULL;
    m_imgRGBResized[i] = NULL;

    GPLOGF((" %d>\n", i));

    return (i);
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_delete
        (JNIEnv *env, jclass c, jint id) {

    if (m_ub[id]) {
        delete m_ub[id];
        m_ub[id] = NULL;
    }
    if (m_imgYUV[id]) {
        image_destroy(m_imgYUV[id], 1);
        m_imgYUV[id] = NULL;
    }

    if (m_imgRGB[id]) {
        image_destroy(m_imgRGB[id], 1);
        m_imgRGB[id] = NULL;
    }

    if (m_imgRGBResized[id]) {
        image_destroy(m_imgRGBResized[id], 1);
        m_imgRGBResized[id] = NULL;
    }

    return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

image_type *yuv2rgb(int iM, u_char* yuv, int width, int height, int orientation) {

    // If the existing YUV image is not on the same size - so we destroy it
    if (m_imgYUV[iM] != NULL && (m_imgYUV[iM]->width != width || m_imgYUV[iM]->height != height)){
        image_destroy(m_imgYUV[iM], 1);
        m_imgYUV[iM] = NULL;
    }

    // Creates new YUV image
    if (m_imgYUV[iM] == NULL)
        m_imgYUV[iM] = image_createYUV420(height, width, NULL);

    memcpy(m_imgYUV[iM]->data, yuv, width * height * 1.5);


    switch (orientation) {
        case 0:
            m_imgRGB[iM] = imageNV21_to_RGB(m_imgYUV[iM], m_imgRGB[iM]);
            break;
        case 90:
            m_imgRGB[iM] = imageNV21_to_RGB(m_imgYUV[iM], m_imgRGB[iM]);
            image_flipV(m_imgRGB[iM]);
            break;
        case 180:
            m_imgRGB[iM] = imageNV21_to_RGB_swap(m_imgYUV[iM], m_imgRGB[iM]);
            image_flipV(m_imgRGB[iM]);
            break;
        case 270:
            m_imgRGB[iM] = imageNV21_to_RGB_swap(m_imgYUV[iM], m_imgRGB[iM]);
            break;
    }

    m_imgRGBResized[iM] = image_crop(m_imgRGB[iM], 0, 0, m_intWidth[iM], m_intHeight[iM], m_imgRGBResized[iM]);

    return m_imgRGBResized[iM];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_prepareYUV
        (JNIEnv *env, jclass c, jint iM, jbyteArray buffer, jint width, jint height, jint orientation, jbyteArray resRGBImage) {

    if (m_ub[iM] == NULL)
        return (-1);

    image_type *img = m_imgRGBResized[iM];
    if (buffer != NULL) {
        u_char *data = (u_char * )(env->functions)->GetByteArrayElements(env, buffer, NULL);

        img = yuv2rgb(iM, data, width, height, orientation);

        (env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *) data, 0);

        // Copying the rgb
        u_char *resRGB = (u_char * )(env->functions)->GetByteArrayElements(env, resRGBImage, NULL);
        memcpy(resRGB, img->data, m_intWidth[iM] * m_intHeight[iM] * 3);
        (env->functions)->ReleaseByteArrayElements(env, resRGBImage, (jbyte *) resRGB, 0);
    }

    if (img == NULL)
        return (-1);

    return 1;
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_prepareRGB
        (JNIEnv *env, jclass c, jint iM, jbyteArray buffer, jint width, jint height, jbyteArray resRGBImage) {

    if (m_ub[iM] == NULL)
        return (-1);

    image_type *img = m_imgRGBResized[iM];
    if (buffer != NULL){
        u_char *data = (u_char *)(env->functions)->GetByteArrayElements(env, buffer, NULL);

        m_imgRGB[iM] = image_realloc( m_imgRGB[iM], height, width, 3, IMAGE_TYPE_U8, 1 );

        memcpy(m_imgRGB[iM]->data, data, width * height * 3);
        img = m_imgRGBResized[iM] = image_crop(m_imgRGB[iM], 0, 0, m_intWidth[iM], m_intHeight[iM], m_imgRGBResized[iM]);

        (env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *) data, 0);

        u_char *resRGB = (u_char * )(env->functions)->GetByteArrayElements(env, resRGBImage, NULL);
        memcpy(resRGB, img->data, m_intWidth[iM] * m_intHeight[iM] * 3);
        (env->functions)->ReleaseByteArrayElements(env, resRGBImage, (jbyte *) resRGB, 0);
    }

    if (img == NULL)
        return (-1);

    return 1;
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_inspect
        (JNIEnv *env, jclass c, jint iM) {

    if (m_ub[iM] == NULL || m_imgRGBResized[iM] == NULL)
        return (-1);


    return m_ub[iM]->ProcessBackground(m_imgRGBResized[iM], 1);
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_getBackgroundSimilarity
        (JNIEnv *env, jclass c, jint iM){

    if (m_ub[iM] == NULL)
        return (-1);

    return m_ub[iM]->GetProcessBackgroundSimilarity();
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_process
        (JNIEnv *env, jclass c, jint iM, jbyteArray resMaskChannel) {

    if (m_ub[iM] == NULL || m_imgRGBResized[iM] == NULL)
        return (-1);

    image_type *resImg;
    int retVal = m_ub[iM]->Process(m_imgRGBResized[iM], 1, &resImg);

    // Copying the result mask
    u_char *resMask = (u_char *)(env->functions)->GetByteArrayElements(env, resMaskChannel, NULL);
    memcpy(resMask, resImg->data, m_intWidth[iM] * m_intHeight[iM]);
    (env->functions)->ReleaseByteArrayElements(env, resMaskChannel, (jbyte *) resMask, 0);

    return retVal;
}

