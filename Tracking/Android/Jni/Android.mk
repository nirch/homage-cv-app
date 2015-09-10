TOP_LOCAL_PATH := $(call my-dir)


LOCAL_CFLAGS := -w

#VIM_PATH := D:\_work\Gp_vim
#$(info $(VIM_PATH) )

include $(CLEAR_VARS)
 
#$(info "TARGET_ARCH_ABI"  $(TARGET_ARCH_ABI) )

include	$(VIM_PATH)/Tracking/PlnTracker/PlnTracker.mk




