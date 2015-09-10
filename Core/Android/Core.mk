DIR := $(call my-dir)


LOCAL_C_INCLUDES += $(DIR)/../VimageLib/.  $(DIR)/../AgpLib/. $(DIR)/../ExpandiLib/. $(DIR)/../ImageLib/.  $(DIR)/../Util/.



LOCAL_LDLIBS += -L$(DIR)/obj/local/$(TARGET_ARCH_ABI) -lVimage  -lCoding  -lAgp  -lEp  -lEpnLib   -lExpandi



#LOCAL_LDLIBS += $(DIR)/obj/local/armeabi/libVimage.a   \
#		$(DIR)/obj/local/armeabi/libCoding.a   \
#		$(DIR)/obj/local/armeabi/libAgp.a   \
#		$(DIR)/obj/local/armeabi/libEp.a   \
#		$(DIR)/obj/local/armeabi/libEpnLib.a \
#		$(DIR)/obj/local/armeabi/libExpandi.a


