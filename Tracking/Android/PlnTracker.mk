LOCAL_PATH := $(call my-dir)


$(info "PlnTracker"  $(APP_ABI) )
 
LOCAL_MODULE            := PlnTracker-prebuilt
LOCAL_SRC_FILES         := $(LOCAL_PATH)/obj/local/$(APP_ABI)/libPlnTracker.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../PlnTracker/.

include $(PREBUILT_STATIC_LIBRARY)
LOCAL_STATIC_LIBRARIES := PlnTracker-prebuilt

