DIR := $(call my-dir)


LOCAL_C_INCLUDES += $(DIR)/../FaceTracking/.  $(DIR)/../Lk/.

LOCAL_LDLIBS += -L$(DIR)/obj/local/$(TARGET_ARCH_ABI) -lPlnTracker


#LOCAL_LDLIBS += $(DIR)/obj/local/armeabi/libPlnTracker.a




