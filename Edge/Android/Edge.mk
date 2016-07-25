DIR := $(call my-dir)


LOCAL_C_INCLUDES += $(DIR)/../. 


#LOCAL_LDLIBS += $(DIR)/obj/local/armeabi/libEdgeLib.a
#LOCAL_LDLIBS += $(DIR)/obj/local/armeabi/libPlnLib.a

LOCAL_LDLIBS += -L$(DIR)/obj/local/$(TARGET_ARCH_ABI) -lEdgeLib
LOCAL_LDLIBS += -L$(DIR)/obj/local/$(TARGET_ARCH_ABI) -lPlnLib



