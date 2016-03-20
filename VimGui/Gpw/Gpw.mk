LOCAL_PATH := $(call my-dir)
ifeq ($(UNAME),Windows_NT)
LOCAL_PATH := $(call  my-dir)
else
LOCAL_PATH := $(abspath $(call my-dir)/../../Gpw/)
endif

LOCAL_PATH := $(call  my-dir)


include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/./.  $(LOCAL_PATH)/../../Core/ImageLib/.  $(LOCAL_PATH)/../../Core/Util/.  $(LOCAL_PATH)/../../VimGui/.  $(LOCAL_PATH)/../../Image/.

LOCAL_SRC_FILES := 	\
	./FrameBuffer/FrameBuffer.cpp \
	./FrameLabel/FrameLabel.cpp \
	./Gpw.cpp \
	./GpwEllipse.cpp \
	./GpwPln.cpp \
	./GpwImage.cpp \
	./GpwMarker.cpp \
	./TextLabel/TextLabel.cpp


LOCAL_CFLAGS := -w

LOCAL_MODULE := Gpw

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

