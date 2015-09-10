LOCAL_PATH := $(call my-dir)


#include $(CLEAR_VARS)
$(info "Util"  $(APP_ABI) )
 
LOCAL_MODULE            := Util
LOCAL_SRC_FILES         := $(LOCAL_PATH)/obj/local/$(APP_ABI)/libUtil.a
#LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Util/.
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Util/.
include $(PREBUILT_STATIC_LIBRARY)
LOCAL_STATIC_LIBRARIES := Util-prebuilt