LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(GRE_TOP)/native-code/SharedAndroid.mk

LOCAL_CFLAGS += -DXTR3D_IMAGEACQUISITION_EXPORTS
LOCAL_CPPFLAGS += -DXTR3D_IMAGEACQUISITION_EXPORTS

LOCAL_SRC_FILES :=  ImageAcquisitionCamera.cpp CameraImpl.cpp ImageAcquisitionFileReader.cpp ImageAcquisitionBase.cpp

LOCAL_C_INCLUDES := $(NATIVE_CODE_ROOT)/../../../../../Utils/Xtr3DOS \
					$(NATIVE_CODE_ROOT)/../../../../../Utils/Xtr3dLogger \
					$(NATIVE_CODE_ROOT)/../../../../../Utils/Util \
					$(NATIVE_CODE_ROOT)/../../../../../Utils/ImageLib \
					$(NATIVE_CODE_ROOT)/../../../../../Utils/Xtr3dInfra \
					$(NATIVE_CODE_ROOT)/../../../../../Platform/Windows/XTR3D_SDK_Metadata \
					$(NATIVE_CODE_ROOT)/../../../../../Platform/linux/Utils/OSUtils \
					$(NATIVE_CODE_ROOT)/../../../../../Algorithms/Utils/AlgUtils \
					$(NATIVE_CODE_ROOT)/../../../../../ThirdParty/boost \
					$(NATIVE_CODE_ROOT)/../../../../../Algorithms/Utils/OsUtils \
					$(NATIVE_CODE_ROOT)/../../../../../Platform/android/SDK/AviReader \
					$(NATIVE_CODE_ROOT)/../../../../../Platform/android/SDK/AviReader/include \
					$(NATIVE_CODE_ROOT)/../../../../../Algorithms/Utils/OSUtils \
					$(NATIVE_CODE_ROOT)/../../../../../Platform/Common/SDK/XTR3D_ImageAcquisition
					
LOCAL_C_INCLUDES += $(NATIVE_CODE_ROOT)/../../../../../Platform/android/Utils/Xtr3dInfra

LOCAL_MODULE := libXtrIA
LOCAL_SHARED_LIBRARIES := libdl libAlgUtils libOSUtils ffmpegutils libXtr3dInfra

#$(info "Building Static Library")
include $(BUILD_SHARED_LIBRARY)

