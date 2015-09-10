LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/./.  $(LOCAL_PATH)/PngLib/.  $(LOCAL_PATH)/../../../Core/ImageLib/.  $(LOCAL_PATH)/../../../Core/Util/.

LOCAL_SRC_FILES := 	\
	PngLib/png.c \
	PngLib/pngerror.c \
	PngLib/pnggccrd.c \
	PngLib/pngget.c \
	PngLib/pngmem.c \
	PngLib/pngpread.c \
	PngLib/pngread.c \
	PngLib/pngrio.c \
	PngLib/pngrtran.c \
	PngLib/pngrutil.c \
	PngLib/pngset.c \
	PngLib/pngtest.c \
	PngLib/pngtrans.c \
	PngLib/pngvcrd.c \
	PngLib/pngwio.c \
	PngLib/pngwrite.c \
	PngLib/pngwtran.c \
	PngLib/pngwutil.c \
	Png/ImageReadPng.c \
	Png/ImageWritePng.c \
	Png/PngDecoder.cpp \
	Png/PngEncoder.cpp \
	Mng/ImageWriteMng.c \
	Mng/mng_ImageWritePng.c \
	Mng/mng_pngwrite.c \
	Mng/MngEncoder.cpp \
	PngWriter/PngWriter.cpp


LOCAL_CFLAGS := -w

LOCAL_MODULE := PngCodec

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

