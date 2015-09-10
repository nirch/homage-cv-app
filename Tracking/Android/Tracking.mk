DIR := $(call my-dir)


LOCAL_C_INCLUDES += $(DIR)/../FaceTracking/.  $(DIR)/../Lk/.


#LOCAL_LDLIBS += $(DIR)/obj/local/$(APP_ABI)/libFaceTracking.a \
# 		$(DIR)/obj/local/$(APP_ABI)/libLk.a \
#		$(DIR)/obj/local/$(APP_ABI)/libPlnTracker.a



LOCAL_LDLIBS += -L$(DIR)/obj/local/$(TARGET_ARCH_ABI) -lPlnTracker
