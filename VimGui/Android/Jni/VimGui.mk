LOCAL_PATH := $(call my-dir)


LOCAL_C_INCLUDES += $(LOCAL_PATH)/../.  $(LOCAL_PATH)/../../Gpw/.


LOCAL_LDLIBS += $(LOCAL_PATH)/obj/local/armeabi/libVimRenderer.a \
		$(LOCAL_PATH)/obj/local/armeabi/libGpw.a \
		$(LOCAL_PATH)/obj/local/armeabi/libImageAcquisition.a






