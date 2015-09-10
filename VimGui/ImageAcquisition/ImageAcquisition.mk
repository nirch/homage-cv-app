LOCAL_PATH := $(call my-dir)


LOCAL_PATH := $(call  my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/./.  $(LOCAL_PATH)/../../Core/ImageLib/.  $(LOCAL_PATH)/../../Core/Util/.  $(LOCAL_PATH)/../../VimGui/.  $(LOCAL_PATH)/../../Image/.

LOCAL_SRC_FILES := 	\
	./ImageAcquisition.cpp \
	./Android/AndroidCameraAcquisition.cpp


LOCAL_CFLAGS := -w

LOCAL_MODULE := ImageAcquisition

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

