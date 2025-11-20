LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libvulkan.xeno_wrapper
LOCAL_SRC_FILES := $(shell ls src/*.c)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS := -ldl -lpthread -lz
include $(BUILD_SHARED_LIBRARY)
