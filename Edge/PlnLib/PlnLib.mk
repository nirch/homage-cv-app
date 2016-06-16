LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../../Core/Util/. 

LOCAL_SRC_FILES := 	\
	PlnConnect/PlnConnect.cpp \
	PlnConnect/PlnConnectPrm.cpp \
	PlnCurvature/PlnCurvature.cpp \
	PlnCurvature/PlncurvaturePrm.cpp \
	PlnCurvature/PlnStraightVl.cpp \
	PlnFeature/PlnFeature.cpp \
	PlnFeature/PlnFeaturePrm.cpp \
	PlnMerging/PlnMerging.cpp \
	PlnMerging/PlnMergingPrm.cpp \
	PlnMerging/PlnMergingScale.cpp \
	PlnUnion/PlnUnion.cpp


LOCAL_CFLAGS := -w

LOCAL_MODULE := PlnLib

#$(info "Building Static Library")

include $(BUILD_STATIC_LIBRARY)

