LOCAL_PATH := $(call my-dir)

ifeq ($(UNAME),Windows_NT)
LOCAL_PATH := $(call  my-dir)
else
LOCAL_PATH := $(abspath $(call my-dir)/../../MattingLib/)
endif


include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/.  $(LOCAL_PATH)/../../Core/ImageLib/.  $(LOCAL_PATH)/../../Core/Util/.  $(LOCAL_PATH)/../../Tracking/.

LOCAL_SRC_FILES := 	\
	Background/BackgroundRemoval.cpp \
	ColorBackground/ColorBackground.cpp \
	PlnAdjust/PlnAdjustCoherent.cpp \
	PlnAdjust/PlnAdjustEdge.cpp \
	PlnAdjust/PlnAdjustIn.cpp \
	PlnAdjust/PlnAdjustStart.cpp \
	PlnAdjust/PlnAdjustThin.cpp \
	PlnAdjust/PlnAdjustBottom.cpp \
	UniformBackground/UbBackground.cpp \
	UniformBackground/UbBlob.cpp \
	UniformBackground/UbBlob1.cpp \
	UniformBackground/UbBlob2.cpp \
	UniformBackground/UbContour.cpp \
	UniformBackground/UbEdge.cpp \
	UniformBackground/UbPrm.cpp \
	UniformBackground/UbSmooth.cpp \
	UniformBackground/UbThin.cpp \
	UniformBackground/UbUpdate.cpp \
	UniformBackground/UniformBackground.cpp


LOCAL_CFLAGS := -w -fexceptions

LOCAL_MODULE := MattingLib

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

