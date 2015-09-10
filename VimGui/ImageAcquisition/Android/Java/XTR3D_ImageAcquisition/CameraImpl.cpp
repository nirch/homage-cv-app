#include "CameraImpl.hpp"
#include <stdlib.h>
#include <sys/time.h> //For custom timestamps

#ifdef __cplusplus
#define EXTERN_C_PREFIX extern "C"
#else
#define EXTERN_C_PREFIX
#endif

IMPLEMENT_CLASS_LOGGER(CameraImpl,"XTR3D_ImageAcquisition.CameraImpl");

struct CameraImpl::Impl
{
    Impl(): m_magic(0xDEADBEEF), m_activity(NULL), m_jvm(NULL), m_onReceivedNewImage(NULL), m_context(NULL), m_javaCameraImplInstance(NULL) {}

    long m_magic;
    jobject m_activity;
    JavaVM *m_jvm;
    CameraImplNewImageCallback *m_onReceivedNewImage;
    void* m_context;
    jobject m_javaCameraImplInstance;
};

void CameraImpl::OnReceivedNewFrame(unsigned char* frame, CameraImpl* thiz)
{
    LOG_TRACE(__FUNCTION__ << ": entered with params: frame addr: " << (unsigned long)frame);

    if (!thiz || !(thiz->impl))
    {
        LOG_ERROR(__FUNCTION__ << ": either CameraImpl ot its Impl are NULL!");
        return;
    }

    if (thiz->impl->m_magic != 0xDEADBEEF)
    {
        LOG_ERROR(__FUNCTION__ << ": Magic number indicates corrupted instance! (got: " << thiz->impl->m_magic << ") Expected: 0xDEADBEEF");
        return;
    }

    if (frame == NULL)
    {
        LOG_WARN(__FUNCTION__ << ": received NULL frame. Doing nothing...");
        return;
    }

    if (thiz->impl->m_onReceivedNewImage)
    {
        struct timeval timestampStruct;
        gettimeofday(&timestampStruct, NULL);
        long long int timestampInMsecs = timestampStruct.tv_sec*1000 + timestampStruct.tv_usec/1000;

        thiz->impl->m_onReceivedNewImage(thiz->impl->m_context, frame, timestampInMsecs);
    }

    LOG_TRACE(__FUNCTION__ << ": exited");
}

CameraImpl::CameraImpl(): impl(new Impl)
{
}

CameraImpl::~CameraImpl()
{
    bool wasAlreadyAttached;
    JNIEnv *env;
    if (!impl)
    {
        return;
    }

    if (!impl->m_jvm)
    {
        LOG_WARN(__FUNCTION__ << ": JVM context is NULL! Trying to continue...");
        delete impl;
        impl = NULL;
        return;
    }
    //else impl->m_jvm not NULL

    Stop();

    if (!jEnv.CheckInstanceAndAcquireEnv(impl->m_jvm, wasAlreadyAttached, env))
    {
        LOG_ERROR(__FUNCTION__ << ": CheckInstanceAndAcquireEnv failed!");
        delete impl;
        impl = NULL;
        return;
    }

    env->DeleteGlobalRef(impl->m_javaCameraImplInstance);
    impl->m_javaCameraImplInstance = NULL;

    jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);

    delete impl;
    impl = NULL;
}

bool CameraImpl::Init(void* jvmContext, void* activity, void* retCameraView)
{
    bool wasAlreadyAttached;
    JNIEnv *env;

    if (jvmContext == NULL || activity == NULL || retCameraView == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": at least one of the input params is NULL!");
        return false;
    }

    if (!impl)
    {
        LOG_ERROR(__FUNCTION__ << ": impl is NULL!");
        return false;
    }

    if (impl->m_jvm)
    {
        LOG_WARN(__FUNCTION__ << ": JVM context already set! Trying to continue...");
    }
    else
    {
        impl->m_jvm = (JavaVM*)jvmContext;
    }

    if (!jEnv.CheckInstanceAndAcquireEnv(impl->m_jvm, wasAlreadyAttached, env))
    {
        LOG_ERROR(__FUNCTION__ << ": CheckInstanceAndAcquireEnv failed!");
        return false;
    }

    jclass javaCameraImplClass = env->FindClass("com/xtr/api/CameraImpl");
    if (javaCameraImplClass == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": FindClass returned NULL!");
        jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
        return false;
    }

    jmethodID ctorMID = env->GetMethodID(javaCameraImplClass, "<init>",
        "(J)V");
    if (ctorMID == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": GetMethodID returned NULL!");
        jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
        //delete env???
        return false;
    }

    //Pass "this" as a byte array, as arg to Ctor, since it's just to save the "this" on JAVA side:
	jobject tmp=env->NewObject(javaCameraImplClass, ctorMID, (jlong)this);
	if (tmp == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": NewObject returned NULL!");
        jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
        //delete env???
        return false;
    }
    impl->m_javaCameraImplInstance = env->NewGlobalRef(tmp);
	if (impl->m_javaCameraImplInstance == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": env->NewGlobalRef returned NULL!");
        jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
        //delete env???
        return false;
    }
    

    if (impl->m_activity)
    {
        LOG_WARN(__FUNCTION__ << ": activity already set! Trying to continue...");
    }
    else
    {
        impl->m_activity = (jobject)activity;
        //Invoke Java setter:
        jmethodID setActivityMID = env->GetMethodID(javaCameraImplClass, "SetActivity",
            "(Landroid/app/Activity;)V");
        if (setActivityMID == NULL)
        {
            LOG_ERROR(__FUNCTION__ << ": GetMethodID returned NULL!");
            jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
            //delete env???
            return false;
        }

        env->CallVoidMethod(impl->m_javaCameraImplInstance, setActivityMID, impl->m_activity);
    }

    jmethodID getTextureViewMID = env->GetMethodID(javaCameraImplClass, "GetView", "()Landroid/view/View;");
    if (getTextureViewMID == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": GetMethodID for GetView returned NULL!");
        jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
        //delete env???
        return false;
    }

    jobject view = env->CallObjectMethod(impl->m_javaCameraImplInstance, getTextureViewMID);
    if (view == NULL)
    {
        LOG_ERROR(__FUNCTION__ << ": CallObjectMethod for getTextureViewMID returned NULL!");
        jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
        //delete env???
        return false;
    }

    env->SetObjectArrayElement((jobjectArray)retCameraView, 0, view);

    jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
    return true;
}

void CameraImpl::SetOnCameraReceivedNewImageCallback(CameraImplNewImageCallback* cb, void* context)
{
	LOG_TRACE(__FUNCTION__ << ": entered.");
    if (!impl)
    {
        LOG_ERROR(__FUNCTION__ << ": impl is NULL!");
        return;
    }

    //TODO: Upper layer makes sure that if camera already running. Callback NOT set!
    impl->m_onReceivedNewImage = cb;
    impl->m_context = context;
}

bool CameraImpl::Start(bool shouldOutputBGR)
{
	LOG_TRACE(__FUNCTION__ << ": entered.");
    bool wasAlreadyAttached;
    JNIEnv *env;
    if (!jEnv.CheckInstanceAndAcquireEnv(impl->m_jvm, wasAlreadyAttached, env))
    {
        LOG_ERROR(__FUNCTION__ << ": CheckInstanceAndAcquireEnv failed!");
        return false;
    }

    try
    {
        jclass javaCameraImplClass = env->FindClass("com/xtr/api/CameraImpl");
        if (javaCameraImplClass == NULL)
        {
            LOG_ERROR(__FUNCTION__ << ": FindClass returned NULL!");
            jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
            //delete env???
            return false;
        }

        jmethodID startMID = env->GetMethodID(javaCameraImplClass, "Start",
            "(Z)V");
        if (startMID == NULL)
        {
            LOG_ERROR(__FUNCTION__ << ": GetMethodID returned NULL!");
            jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
            //delete env???
            return false;
        }

        env->CallVoidMethod(impl->m_javaCameraImplInstance, startMID, false);
    }
    catch (const std::exception& exc)
    {
        LOG_FATAL(__FUNCTION__  << ": caught exception: " << exc.what());
        jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
        return false;
    }

    jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
	LOG_TRACE(__FUNCTION__ << ": exited.");
    return true;
}

bool CameraImpl::Stop()
{
	LOG_TRACE(__FUNCTION__ << ": entered.");
    bool wasAlreadyAttached;
    JNIEnv *env;
    if (!jEnv.CheckInstanceAndAcquireEnv(impl->m_jvm, wasAlreadyAttached, env))
    {
        LOG_ERROR(__FUNCTION__ << ": CheckInstanceAndAcquireEnv failed!");
        return false;
    }

    try
    {
        jclass javaCameraImplClass = env->FindClass("com/xtr/api/CameraImpl");
        if (javaCameraImplClass == NULL)
        {
            LOG_ERROR(__FUNCTION__ << ": FindClass returned NULL!");
            jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
            //delete env???
            return false;
        }

        jmethodID stopMID = env->GetMethodID(javaCameraImplClass, "Stop",
            "()Z");
        if (stopMID == NULL)
        {
            LOG_ERROR(__FUNCTION__ << ": GetMethodID returned NULL!");
            jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
            //delete env???
            return false;
        }

        env->CallBooleanMethod(impl->m_javaCameraImplInstance, stopMID);
    }
    catch (const std::exception& exc)
    {
        LOG_FATAL(__FUNCTION__  << ": caught exception: " << exc.what());
        jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
        return false;
    }

    jEnv.Cleanup(impl->m_jvm, wasAlreadyAttached);
	LOG_TRACE(__FUNCTION__ << ": exited.");
    return true;
}
//Implementation of com_xtr_api_CameraPreviewCallback
EXTERN_C_PREFIX JNIEXPORT void JNICALL Java_com_xtr_api_CameraPreviewCallback_callMe
  (JNIEnv* env, jobject, jbyteArray frame, jlong context)
{
    jbyte* buffer = env->GetByteArrayElements(frame, NULL);

    //Invoke CameraImpl static function:
    CameraImpl::OnReceivedNewFrame((unsigned char*)buffer, (CameraImpl*)context);

    env->ReleaseByteArrayElements(frame, buffer, 0); //TODO: if there is no memcpy, then it should change to JNI_COMMIT
}