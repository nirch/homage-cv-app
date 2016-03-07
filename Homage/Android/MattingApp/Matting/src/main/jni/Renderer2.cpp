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
#include "HrRendererLib/HrEffect/HrEffectAlignment.h"
#include "HrRendererLib/HrEffect/HrEffectMaskGif.h"

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

static int	m_nR2 = 0;
static CHomageRenderer *m_ar2[16];

CHrSourceI  *HrSource_create2( int type );
CHrEffectI  *HrEffect_create2( int type, const char *data );



JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_create
        (JNIEnv *env, jclass c, jfloat duration, jint fps) {
    GPLOGF(("<Renderer Create"));

    int i;
    for (i = 0; i < m_nR2; i++) {
        if (m_ar2[i] == NULL)
            break;
    }

    if (i >= 8) return (-1);

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

    CHrSourceJava  *source = new CHrSourceJava();

    source->SetJavaSource(env, javaSource);

    if (!source->Open())
        return -1;

    int id = hr->AddSource( source );

    image_type *imm = NULL;
    //id = source->ReadFrame(0, 1231, &imm);

    GPLOGF((" >\n"));

    return( id );
}


JNIEXPORT jint JNICALL  Java_com_homage_renderer_Renderer2_addFX
        (JNIEnv * env, jclass c, jint iR, jint sourceId, jint type, jstring jdata ) {
    GPLOGF(("<addEffect"));

    CHomageRenderer *hr = m_ar2[iR];
    if (hr == NULL || hr->IsProcess()) {
        GPLOGF(("In process  >\n"));
        return (-1);
    }

    CHrSourceI *src = hr->GetSource(sourceId);
    if (src == NULL)
        return (-1);

    const char *data = ujni_getJString(env, jdata);

    CHrEffectI *ef = HrEffect_create2(type, data);

    ujni_releaseJString(env, jdata, data);

    if (ef == NULL)
        return (-1);

    int id = src->AddEffect(ef);

    GPLOGF((" Id: %d ", id));


    GPLOGF((" >\n"));
    return (id);
}

JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer2_addOutput
    (JNIEnv * env, jclass c, jint iR, jint type, jstring jfile, jint width, jint height, int fjrameSpeed ) {
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
        if (out->Init((char *) file, width, height, fjrameSpeed) < 0) {
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
        if (out->Init((char *) file, width, height, fjrameSpeed) < 0) {
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

    // Closing all the sources
    int idxSrc = 0;
    CHrSourceI *src = hr->GetSource(idxSrc++);
    while (src != NULL){
        src->Close();
        src = hr->GetSource(idxSrc++);
    }

    //int iii = 7;
    // Closing all the outputs
    int idxOutput = 0;
    CHrOutputI *output = hr->GetOuput(idxOutput++);
    while (output != NULL){
        output->Close();
        output = hr->GetOuput(idxOutput++);
    }

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

CHrEffectI  *HrEffect_create2( int type, const char *data ) {

    CHrEffectI *effect = NULL;

    if (type == EFFECT_MASK) {
        effect = new CHrEffectMask();

        GPLOGF((" effect mask"));
    }
    else if (type == EFFECT_POSE) {
        effect = new CHrEffectPose();

        GPLOGF((" effect pose"));
    }
    else if (type == EFFECT_ALIGNMENT) {
        effect = new CHrEffectAlignment();

        GPLOGF((" effect ALIGNMENT"));

    }
    else if (type == EFFECT_MASKGIF) {
        effect = new CHrEffectMaskGif();

        GPLOGF((" effect MASK GIF"));
    }


    if (type == EFFECT_MASK || type == EFFECT_MASKGIF) {
        if (effect->Init((char *) data) < 0) {

            delete effect;

            GPLOGF(("Init mask/maskgif effect failed >\n"));
            return (NULL);
        }
    }
    else if (type == EFFECT_POSE) {
        effect->InitFromData((char *) data);
    }

    return (effect);
}