TOP_LOCAL_PATH := $(call my-dir)

VIM_PATH := D:/_work/Gp_vim

include $(CLEAR_VARS)


include $(VIM_PATH)/Homage/Android/Homage.mk

include $(VIM_PATH)/Tracking/Android/Tracking.mk

include $(VIM_PATH)/Edge/Android/Edge.mk

include $(VIM_PATH)/VimGui/Android/VimGui.mk

include $(VIM_PATH)/Image/Png/Android/Png.mk

include $(VIM_PATH)/Core/Android/CoreUtil.mk


LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Image/.


LOCAL_LDLIBS += -landroid


LOCAL_PATH := $(TOP_LOCAL_PATH)


LOCAL_MODULE    := Matting
LOCAL_SRC_FILES := Matting.cpp  Renderer.cpp  PngRecorder.cpp UjniTool.cpp


include $(BUILD_SHARED_LIBRARY)
