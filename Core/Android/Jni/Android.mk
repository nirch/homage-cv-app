TOP_LOCAL_PATH := $(call my-dir)

LOCAL_CFLAGS := -w

#VIM_PATH := D:\_work\Gp_vim

#$(info $(VIM_PATH) )

include $(CLEAR_VARS)

include	$(VIM_PATH)/Core/ImageLib/Image.mk
include	$(VIM_PATH)/Core/Util/Util.mk



