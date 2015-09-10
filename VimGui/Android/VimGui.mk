DIR := $(call my-dir)


LOCAL_C_INCLUDES += $(DIR)/../.  $(DIR)/../../Gpw/.


LOCAL_LDLIBS += -L$(DIR)/obj/local/$(TARGET_ARCH_ABI) -lGpw  -lImageAcquisition


#LOCAL_LDLIBS += $(DIR)/obj/local/armeabi/libVimRenderer.a \
#		$(DIR)/obj/local/armeabi/libGpw.a \
#		$(DIR)/obj/local/armeabi/libImageAcquisition.a






