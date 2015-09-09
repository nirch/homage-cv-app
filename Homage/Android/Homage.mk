LOCAL_PATH := $(call my-dir)


LOCAL_C_INCLUDES += $(LOCAL_PATH)/../MattingLib/.  $(LOCAL_PATH)/../HrRendererLib/. $(LOCAL_PATH)/../.


#LOCAL_LDLIBS += $(LOCAL_PATH)/obj/local/armeabi/libMattingLib.a

LOCAL_LDLIBS += -L$(LOCAL_PATH)/obj/local/$(TARGET_ARCH_ABI) -lMattingLib  

LOCAL_LDLIBS += -L$(LOCAL_PATH)/obj/local/$(TARGET_ARCH_ABI) -lHrRendererLib


