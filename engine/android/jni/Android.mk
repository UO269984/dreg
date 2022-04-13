LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include ../../globalVars.make
include ../srcs.make

LOCAL_ARM_MODE := arm
LOCAL_MODULE := $(LIBRARY_NAME)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(INCLUDE)
LOCAL_SRC_FILES := $(SRCS)

include $(BUILD_SHARED_LIBRARY)