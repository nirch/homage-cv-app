#ifndef __CAMERA_IMPL_H__
#define __CAMERA_IMPL_H__

#include <jni.h>
#include "Xtr3DLogger.h"
#include "JavaEnvironment.h"

typedef void(CameraImplNewImageCallback)(void*, unsigned char*, long long int);

class CameraImpl
{
public:
    DECLARE_CLASS_LOGGER();

    static void OnReceivedNewFrame(unsigned char* frame, CameraImpl* thiz);

	CameraImpl();
    ~CameraImpl();
    bool Init(void *jvmContext, void* activity, void* retCameraView);
    bool Start(bool shouldOutputBGR);

    void SetOnCameraReceivedNewImageCallback(CameraImplNewImageCallback* cb, void* context);

    bool Stop(void);

private:
	
	JavaEnvironment jEnv;
    struct Impl; // Forward declaration
    Impl* impl;
};

#endif //__CAMERA_IMPL_H__