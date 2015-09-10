DIR := $(call my-dir)


LOCAL_C_INCLUDES += $(DIR)/../. 



LOCAL_LDLIBS += -L$(DIR)/obj/local/$(TARGET_ARCH_ABI) -lPngCodec



