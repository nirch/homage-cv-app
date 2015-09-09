LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/./.  $(LOCAL_PATH)/../../Core/ImageLib/.  $(LOCAL_PATH)/../../Core/Util/. $(LOCAL_PATH)/../../Image/. $(LOCAL_PATH)/../../Image/Png/.

LOCAL_SRC_FILES := 	\
	HomageRenderer.cpp \
	HrEffect/HrEffectAlignment.cpp \
	HrEffect/HrEffectI.cpp \
	HrEffect/HrEffectMask.cpp \
	HrEffect/HrEffectPose.cpp \
	HrEffect/HrEffectMaskGif.cpp \
	HrOutput/HrOutputGif.cpp \
	HrOutput/HrOutputPng.cpp \
	HrOutput/HrOutputJava.cpp \
	HrOutput/HrOutputI.cpp \
	HrSource/HrSourceGif.cpp \
	HrSource/HrSourcePng.cpp \
	HrSource/HrSourceI.cpp \
	HrSource/HrSourceLive.cpp


LOCAL_CFLAGS := -w

LOCAL_MODULE := HrRendererLib

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

