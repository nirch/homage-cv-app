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
static image_type *m_imgRGBCropped[MAX_MATTING];

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
    m_imgRGBCropped[i] = NULL;

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

    if (m_imgRGBCropped[id]) {
        image_destroy(m_imgRGBCropped[id], 1);
        m_imgRGBCropped[id] = NULL;
    }

    return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

image_type *yuv2rgb(int iM, u_char* yuv, int width, int height, int orientation, bool crop) {

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

    if (!crop)
        return m_imgRGB[iM];

    m_imgRGBCropped[iM] = image_crop(m_imgRGB[iM], 0, 0, m_intWidth[iM], m_intHeight[iM],
                                            m_imgRGBCropped[iM]);
    return m_imgRGBCropped[iM];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

image_type *getImageRGB(JNIEnv *env, jint iM, jbyteArray buffer, jint width, jint height){
    if (m_ub[iM] == NULL)
        return (NULL);

    image_type *img = m_imgRGBCropped[iM];
    if (buffer != NULL){
        u_char *data = (u_char *)(env->functions)->GetByteArrayElements(env, buffer, NULL);

        m_imgRGB[iM] = image_realloc( m_imgRGB[iM], height, width, 3, IMAGE_TYPE_U8, 1 );

        memcpy(m_imgRGB[iM]->data, data, width * height * 3);
        img = m_imgRGBCropped[iM] = image_crop(m_imgRGB[iM], 0, 0, m_intWidth[iM], m_intHeight[iM], m_imgRGBCropped[iM]);

        (env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *) data, 0);
    }

    return img;
}

image_type *getImageYUV(JNIEnv *env, jint iM, jbyteArray buffer, jint width, jint height, jint orientation, jbyteArray resRGBImage){
    if (m_ub[iM] == NULL)
        return (NULL);

    image_type *img = m_imgRGBCropped[iM];
    if (buffer != NULL) {
        u_char *data = (u_char * )(env->functions)->GetByteArrayElements(env, buffer, NULL);

        img = yuv2rgb(iM, data, width, height, orientation, true);

        (env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *) data, 0);

        if (resRGBImage != NULL) {
            // Copying the rgb
            u_char *resRGB = (u_char * )(env->functions)->GetByteArrayElements(env, resRGBImage, NULL);
            memcpy(resRGB, img->data, m_intWidth[iM] * m_intHeight[iM] * 3);
            (env->functions)->ReleaseByteArrayElements(env, resRGBImage, (jbyte *) resRGB, 0);
        }
    }

    return img;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int processImg(JNIEnv *env, int iM, image_type *img, jbyteArray resMaskChannel){

    if (img == NULL)
        return (-1);

    image_type *resImg;
    int retVal = m_ub[iM]->Process(img, 1, &resImg);

    // Copying the result mask
    u_char *resMask = (u_char *)(env->functions)->GetByteArrayElements(env, resMaskChannel, NULL);
    memcpy(resMask, resImg->data, m_intWidth[iM] * m_intHeight[iM]);
    (env->functions)->ReleaseByteArrayElements(env, resMaskChannel, (jbyte *) resMask, 0);

    return retVal;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_yuv2rgb
        (JNIEnv *env, jclass c, jint iM, jbyteArray buffer, jint width, jint height, jint orientation, jboolean crop,
         jbyteArray resRGBImage) {

    if (m_ub[iM] == NULL)
        return (-1);

    u_char *data = (u_char * )(env->functions)->GetByteArrayElements(env, buffer, NULL);
    u_char *resRGB = (u_char * )(env->functions)->GetByteArrayElements(env, resRGBImage, NULL);

    // Converting
    image_type *rgbImage = yuv2rgb(iM, data, width, height, orientation, crop);

    // Copying the result
    memcpy(resRGB, rgbImage->data, rgbImage->width * rgbImage->height * 3);

    (env->functions)->ReleaseByteArrayElements(env, buffer, (jbyte *) data, 0);
    (env->functions)->ReleaseByteArrayElements(env, resRGBImage, (jbyte *) resRGB, 0);

    return 1;
}



JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_inspectRGB
        (JNIEnv *env, jclass c, jint iM, jbyteArray buffer, jint width, jint height) {

    image_type *img = getImageRGB(env, iM, buffer, width, height);

    if (img == NULL)
        return (-1);

    return m_ub[iM]->ProcessBackground(img, 1);
}



JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_inspectYUV
        (JNIEnv *env, jclass c, jint iM, jbyteArray buffer, jint width, jint height, jint orientation, jbyteArray resRGBImage) {

    image_type *img = getImageYUV(env, iM, buffer, width, height, orientation, resRGBImage);

    if (img == NULL)
        return (-1);

    return m_ub[iM]->ProcessBackground(img, 1);
}


JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_removeBackgroundRGB
        (JNIEnv *env, jclass c, jint iM, jbyteArray buffer, jint width, jint height, jbyteArray resMaskChannel) {

    image_type *img = getImageRGB(env, iM, buffer, width, height);

    if (img == NULL)
        return (-1);

    return processImg(env, iM, img, resMaskChannel);
}

JNIEXPORT jint JNICALL Java_com_homage_matting_Matting2_removeBackgroundYUV
        (JNIEnv *env, jclass c, jint iM, jbyteArray buffer, jint width, jint height, jint orientation,
         jbyteArray resRGBImage, jbyteArray resMaskChannel) {

    image_type *img = getImageYUV(env, iM, buffer, width, height, orientation, resRGBImage);

    if (img == NULL)
        return (-1);

    return processImg(env, iM, img, resMaskChannel);
}
