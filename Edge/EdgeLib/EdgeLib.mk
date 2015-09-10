LOCAL_PATH := $(call my-dir)
ifeq ($(UNAME),Windows_NT)
LOCAL_PATH := $(call  my-dir)
else
LOCAL_PATH := $(abspath $(call my-dir)/../../EdgeLib/)
endif

LOCAL_PATH := $(call  my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/../.  $(LOCAL_PATH)/../../Core/ImageLib/.  $(LOCAL_PATH)/../../Core/Util/.  $(LOCAL_PATH)/../../VimGui/.  $(LOCAL_PATH)/../../Core/AgpLib/.

LOCAL_SRC_FILES := 	\
	./LineDetector/LineDetector.cpp \
	./LineDetector/pEdgeDetector.cpp \
	./LineDetector/pEdgeDump.cpp \
	./LineDetector/pEdgeLine.cpp \
	./LineDetector/pEdgeSLine.cpp \
	./LineDetector/pEdgeLinking.cpp \
	./LineDetector/pEdgePrm.cpp \
	./LineDetector/pEdgeStatic.cpp \
	./LineDetector/plEdge.cpp \
	./Ridge/plRidge.cpp \
	./Ridge/pRidgeDetector.cpp \
	./Ridge/pRidgeEdgeMask.cpp \
	./Ridge/pRidgeLine.cpp \
	./Ridge/pRidgeLineN.cpp \
	./Ridge/pRidgeLinking.cpp \
	./Ridge/pRidgeMask.cpp \
	./Ridge/pRidgePrm/pRidgeDetectPrm.cpp \
	./Ridge/pRidgePrm/pRidgeLinePrm.cpp \
	./Ridge/pRidgePrm/pRidgePrm.cpp \
	./Ridge/plRidgeUnion.cpp \
	./Ridge/RidgeDetector.cpp


LOCAL_CFLAGS := -w

LOCAL_MODULE := EdgeLib

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

