LOCAL_PATH := $(call my-dir)

ifeq ($(UNAME),Windows_NT)
LOCAL_PATH := $(call  my-dir)
else
LOCAL_PATH := $(abspath $(call my-dir)/../../PlnTracker/)
endif

LOCAL_PATH := $(call  my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/../.  $(LOCAL_PATH)/../../Core/imageLib/.  $(LOCAL_PATH)/../../Core/Util/.

LOCAL_SRC_FILES := 	\
	./PlnHeadTracker/PlnHeadTracker.cpp \
	./PlnHeadTracker/PlnHeadBox.cpp \
	./PlnHeadTracker/PlnHeadPose.cpp \
	./HeadPose/HeadPose.cpp \
	./PlnHeadDetect/PlnHeadDetect.cpp \
	./PlnHeadDetect/PlnHeadDetectPrm.cpp

LOCAL_CFLAGS := -w

LOCAL_MODULE := PlnTracker

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

