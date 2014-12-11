LOCAL_PATH := $(call my-dir)

ifeq ($(UNAME),Windows_NT)
LOCAL_PATH := $(call  my-dir)
else
LOCAL_PATH := $(abspath $(call my-dir)/../../PlnTracker/)
endif

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/../.  $(LOCAL_PATH)/../../Core/imageLib/.  $(LOCAL_PATH)/../../Core/Util/.

LOCAL_SRC_FILES := 	\
	./PlnHeadTracker/PlnHeadTracker.cpp 
	


LOCAL_CFLAGS := -w

LOCAL_MODULE := PlnTracker

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

