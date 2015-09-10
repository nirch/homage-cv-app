TOP_LOCAL_PATH := $(call my-dir)

LOCAL_CFLAGS := -w

#VIM_PATH := D:\_work\Gp_vim
#$(info $(VIM_PATH) )

include $(CLEAR_VARS)


include	$(VIM_PATH)/VimGui/Gpw/Gpw.mk
include	$(VIM_PATH)/VimGui/ImageAcquisition/ImageAcquisition.mk




