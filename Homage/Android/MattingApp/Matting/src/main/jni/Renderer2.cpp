#include <jni.h>

#include "Renderer2.h"

#define _GPLOG

#include "Uigp/igp.h"
#include "Ulog/Log.h"

#include "UjniTool.h"

#include "ImageAcquisition/Android/AndroidCameraAcquisition.h"
#include "MattingLib/MattingTask/MattingTask.h"


#include "HrRendererLib/HomageRenderer.h"
#include "HrRendererLib/HrSource/HrSourceGif.h"
#include "HrRendererLib/HrSource/HrSourcePng.h"
#include "HrRendererLib/HrSource/HrSourceJava.h"


#include "HrRendererLib/HrOutput/HrOutputGif.h"
#include "HrRendererLib/HrOutput/HrOutputJava.h"
#include "HrRendererLib/HrOutput/HrOutputPng.h"

#include "HrRendererLib/HrEffect/HrEffectMask.h"
#include "HrRendererLib/HrEffect/HrEffectPose.h"
#include "HrRendererLib/HrEffect/HrEffectMaskGif.h"
#include "HrRendererLib/HrEffect/HrEffectSepia.h"
#include "HrRendererLib/HrEffect/HrEffectGray.h"
#include "HrRendererLib/HrEffect/HrEffectAlpha.h"
#include "HrRendererLib/HrEffect/HrEffectCartoon.h"
#include "HrRendererLib/HrEffect/HrEffectMaskWithSource.h"

/*
JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_init
		(JNIEnv *, jclass ){
	return 123;
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_calc( JNIEnv * env, jclass c, jobject obj){

    jint result = 1;
    jclass clazz = (env)->GetObjectClass(obj);
    jmethodID methodId = env->GetMethodID(clazz, "messageMe", "(I)I");

    result = env->CallIntMethod(obj, methodId, 123);

	result++;

	return result;
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_writeFrame( JNIEnv * env, jclass c, jobject obj){

    jint result = 1;
    jclass clazz = (env)->GetObjectClass(obj);
    jmethodID methodId = env->GetMethodID(clazz, "messageMeArray", "([B)I");

    int length = 10;
    jbyteArray dataArr = env->NewByteArray(length);
    jbyte *simpleArr = (jbyte *)env->GetByteArrayElements(dataArr, NULL);

    for (jbyte i = 0; i < 10; ++i) {
        simpleArr[i] = 100 - i;
    }
    env->ReleaseByteArrayElements(dataArr, simpleArr, 0);

    result = env->CallIntMethod(obj, methodId, dataArr);

    result++;

    return result;
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_readFrame( JNIEnv * env, jclass c, jobject obj){

    jint result = 1;
    jclass clazz = (env)->GetObjectClass(obj);
    jmethodID methodId = env->GetMethodID(clazz, "messageMeRetArray", "(I)[B");

    int length = 10;


    jbyteArray dataArr = (jbyteArray)env->CallObjectMethod(obj, methodId, 5);

    jbyte *simpleArr = (jbyte *)env->GetByteArrayElements(dataArr, NULL);

    result = simpleArr[0] + 1;

    env->ReleaseByteArrayElements(dataArr, simpleArr, 0);

    result++;

    return result;
}*/

#define MAX_RENDERERS 8

static int	m_nR2 = 0;
static CHomageRenderer *m_ar2[16];

CHrSourceI  *HrSource_create2( int type );
//CHrOutputI  *HrOutput_create2( int type );
CHrEffectI  *HrEffect_create2( JNIEnv *env, int type, jobject jObjData, jint jIntData );
CHrSourceJava *HrSource_createJavaSource2(JNIEnv * env, jobject javaSource);



JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_create
        (JNIEnv *env, jclass c, jfloat duration, jint fps) {
    GPLOGF(("<Renderer Create"));

    int i;
    for (i = 0; i < m_nR2; i++) {
        if (m_ar2[i] == NULL)
            break;
    }

    if (i >= MAX_RENDERERS) return (-1);

    if (i >= m_nR2) m_nR2 = i + 1;


    m_ar2[i] = new CHomageRenderer();
    m_ar2[i]->setDuration(duration, fps);

    GPLOGF((" %d>\n", i));

    return (i);
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_addSourceImage
        (JNIEnv * env, jclass c, jint iR, jint type, jstring jfile ) {

    GPLOGF(("<addSourceImage"));

    CHomageRenderer *hr  = m_ar2[iR];

    if( hr == NULL || hr->IsProcess() ){
        GPLOGF(("In process  >\n"));
        return( -1 );
    }


    const char *file = ujni_getJString( env, jfile );

    CHrSourceI  *source = HrSource_create2( type );

    if( source == NULL ){
        ujni_releaseJString( env, jfile, file );
        GPLOGF((" >\n"));
        return( -1 );
    }

    GPLOGF((" %s", file ));

    if( source->Init( (char *)file ) < 0 ){
        delete source;
        ujni_releaseJString( env, jfile, file );
        GPLOGF(("%s failed >\n", file ));
        return( -1 );
    }

    GPLOGF((" Init %s", file ));

    GPLOGF((" nS: %d ", hr->m_nS ));

    int id = hr->AddSource( source );

    GPLOGF((" Id: %d.%d ", iR, id ));

    ujni_releaseJString( env, jfile, file );

    GPLOGF((" >\n"));


    return( id );
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_addSourceJava
        (JNIEnv * env, jclass c, jint iR, jobject javaSource){
    GPLOGF(("<addSourceJava"));


    CHomageRenderer *hr  = m_ar2[iR];

    if( hr == NULL || hr->IsProcess() ){
        GPLOGF(("In process  >\n"));
        return( -1 );
    }

    CHrSourceJava  *source = HrSource_createJavaSource2(env, javaSource);

    if (source == NULL){
        return -1;
    }

    int id = hr->AddSource( source );

    //image_type *imm = NULL;
    //id = source->ReadFrame(0, 1231, &imm);

    GPLOGF((" >\n"));

    return( id );
}


JNIEXPORT jint JNICALL  Java_com_homage_renderer_Renderer2_addFX
        (JNIEnv * env, jclass c, jint iR, jint sourceId, jint type, jobject jObjData, jint jIntData ) {
    GPLOGF(("<addEffect"));

    CHomageRenderer *hr = m_ar2[iR];
    if (hr == NULL || hr->IsProcess()) {
        GPLOGF(("In process  >\n"));
        return (-1);
    }

    CHrSourceI *src = hr->GetSource(sourceId);
    if (src == NULL)
        return (-1);


    CHrEffectI *ef = HrEffect_create2(env, type, jObjData, jIntData);

    if (ef == NULL)
        return (-1);

    int id = src->AddEffect(ef);

    GPLOGF((" Id: %d ", id));


    GPLOGF((" >\n"));
    return (id);
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_addOutput
    (JNIEnv * env, jclass c, jint iR, jint type, jstring jfile, jint width, jint height, jint fps ) {
    GPLOGF(("<addOutput"));

    CHomageRenderer *hr = m_ar2[iR];
    if (hr == NULL || hr->IsProcess()) {
        GPLOGF(("In process  >\n"));
        return (-1);
    }


    const char *file = ujni_getJString(env, jfile);

    if (type == HR_GIF) {
        CHrOutputGif *out = new CHrOutputGif();
        GPLOGF((" GIF  %d %d", width, height));
        if (out->Init((char *) file, width, height, 1000 / fps) < 0) {
            ujni_releaseJString(env, jfile, file);
            return (-1);
        }

        GPLOGF((" Init"));

        int id = hr->AddOutput(out);

        GPLOGF((" id = %d >\n", id));

        ujni_releaseJString(env, jfile, file);
        return (id);
    }

    if (type == HR_PNG) {
        CHrOutputPng *out = new CHrOutputPng();
        GPLOGF((" PNG"));
        if (out->Init((char *) file, width, height, 1000 / fps) < 0) {
            ujni_releaseJString(env, jfile, file);
            return (-1);
        }

        GPLOGF((" Init"));

        int id = hr->AddOutput(out);

        GPLOGF((" id = %d >\n", id));

        ujni_releaseJString(env, jfile, file);
        return (id);
    }


    ujni_releaseJString(env, jfile, file);
    GPLOGF((" >\n"));
    return (-1);
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_addOutputJava
        (JNIEnv *env, jclass c, jint iR, jobject javaOutput){
    CHomageRenderer *hr = m_ar2[iR];
    if (hr == NULL || hr->IsProcess()) {
        GPLOGF(("In process  >\n"));
        return (-1);
    }

    CHrOutputJava  *output = new CHrOutputJava();

    output->SetJavaOutput(env, javaOutput);

    if (!output->Open())
        return -1;

    int id = hr->AddOutput( output );

    GPLOGF((" >\n"));
    return (id);
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_process
    (JNIEnv * env, jclass c, jint iR, jint width, jint height ) {
    GPLOGF(("<Render Process: %d", iR));

    CHomageRenderer *hr = m_ar2[iR];
    if (hr == NULL)
        return (-1);

    hr->SetFrameSize(width, height);

    if (hr->IsProcess())
        return (-1);

    hr->Process();

    // Delete will close all the outputs and sources
    delete m_ar2[iR];
    m_ar2[iR] = NULL;


    GPLOGF((" %d >\n", iR));

    return (1);
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_delete
    (JNIEnv * env, jclass c, jint iR ) {
    delete m_ar2[iR];
    m_ar2[iR] = NULL;
    return (1);
}

CHrSourceJava *HrSource_createJavaSource2(JNIEnv * env, jobject javaSource){
    CHrSourceJava  *source = new CHrSourceJava();

    source->SetJavaSource(env, javaSource);

    if (!source->Open()){
        delete source;
        return NULL;
    }

    return source;
}


CHrSourceI  *HrSource_create2( int type ) {

    if (type == HR_GIF) {
        CHrSourceGif *b = new CHrSourceGif();

        GPLOGF((" source gif"));

        return (b);
    }


    if (type == HR_PNG) {
        CHrSourcePng *b = new CHrSourcePng();
        GPLOGF((" source png"));


        return (b);
    }

    GPLOGF((" source none"));

    return (NULL);
}

CHrEffectI  *HrEffect_create2(JNIEnv * env, int type, jobject jObjData, jint jIntData ) {

    CHrEffectI *effect = NULL;

    bool convertToString = false;

    // Creating
    switch (type){
        case EFFECT_MASK:{
            effect = new CHrEffectMask();

            GPLOGF((" effect mask"));
            break;
        }
        case EFFECT_MASKGIF:{
            effect = new CHrEffectMaskGif();

            GPLOGF((" effect MASK GIF"));
            break;
        }
        case EFFECT_MASK_SOURCE: {
            effect = new CHrEffectMaskWithSource();

            GPLOGF((" effect SOURCE MASK"));
            break;
        }
        case EFFECT_POSE:{
            effect = new CHrEffectPose();
            convertToString = true;

            GPLOGF((" effect pose"));
            break;
        }
        case EFFECT_SEPIA:{
            effect = new CHrEffectSepia();

            GPLOGF((" effect sepia"));
            break;
        }
        case EFFECT_GRAY:{
            effect = new CHrEffectGray();

            GPLOGF((" effect gray"));
            break;
        }
        case EFFECT_ALPHA:{
            effect = new CHrEffectAlpha();
            convertToString = true;

            GPLOGF((" effect alpha"));
            break;
        }
        case EFFECT_CARTOON:{
            effect = new CHrEffectCartoon();

            GPLOGF((" effect cartoon"));
            break;
        }
    }


    const char *dataStr = NULL;

    if (convertToString) {
        dataStr = ujni_getJString(env, (jstring)jObjData);
    }

    int initRes = -1;

    // Initializing
    switch (type){
        case EFFECT_MASK:
        case EFFECT_MASKGIF:{

            initRes = effect->Init((char *) dataStr);
            break;
        }
        case EFFECT_MASK_SOURCE:{

            CHrSourceJava  *source = HrSource_createJavaSource2(env, jObjData);

            if (source != NULL) {
                initRes = ((CHrEffectMaskWithSource*)effect)->InitWithSource(source);
            }

            break;
        }
        case EFFECT_POSE:
        case EFFECT_ALPHA: {
            initRes = effect->InitFromData((char *) dataStr);
            break;
        }
        case EFFECT_SEPIA:{

            // Converting the int color to rgb normalized values
            float r = ((jIntData & 0xFF0000) >> 16) / 255.0;
            float g = ((jIntData & 0x00FF00) >> 8) / 255.0;
            float b = (jIntData & 0x0000FF) / 255.0;

            initRes = ((CHrEffectSepia*)effect)->Init(r, g, b, NULL);
            break;
        }
        case EFFECT_CARTOON:{

            initRes = ((CHrEffectCartoon*)effect)->Init(jIntData, NULL);
            break;
        }
    }

    if (dataStr != NULL){
        ujni_releaseJString(env, (jstring)jObjData, dataStr);
    }

    if (initRes <= 0)
    {
        delete effect;
        effect = NULL;

        GPLOGF(("Init effect failed >\n"));
    }

    return (effect);
}