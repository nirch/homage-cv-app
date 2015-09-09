TOP_LOCAL_PATH := $(call my-dir)


LOCAL_CFLAGS := -w 


VIM_PATH := D:\_work\Gp_vim

$(info $(VIM_PATH) )

include $(CLEAR_VARS)

include $(VIM_PATH)/Image/Png/Android/Png.mk

include	$(VIM_PATH)\Homage\MattingLib\MattingLib.mk

include	$(VIM_PATH)\Homage\HrRendererLib\HrRendererLib.mk



include $(CLEAR_VARS)
