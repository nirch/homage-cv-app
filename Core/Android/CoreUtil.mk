DIR := $(call my-dir)


LOCAL_C_INCLUDES += $(DIR)/../VimageLib/.  $(DIR)/../AgpLib/. $(DIR)/../ExpandiLib/. $(DIR)/../ImageLib/.  $(DIR)/../Util/.



LOCAL_LDLIBS += -L$(DIR)/obj/local/$(TARGET_ARCH_ABI) -lImage -lUtil