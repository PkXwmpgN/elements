LOCAL_PATH := $(call my-dir)
PROJECT_ROOT:= $(call my-dir)/../../../../../../../..

BUILD_FIRE := true
BUILD_LIGHT := true
BUILD_STRANGE := true
BUILD_LIQUID := true
BUILD_AIR := true

# android
# ------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := elements
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)

ifeq ($(BUILD_FIRE), true)
	FILE_LIST += $(wildcard $(LOCAL_PATH)/fire/*.cpp)
endif

ifeq ($(BUILD_LIGHT), true)
	FILE_LIST += $(wildcard $(LOCAL_PATH)/light/*.cpp)
endif

ifeq ($(BUILD_STRANGE), true)
	FILE_LIST += $(wildcard $(LOCAL_PATH)/strange/*.cpp)
endif

ifeq ($(BUILD_AIR), true)
	FILE_LIST += $(wildcard $(LOCAL_PATH)/air/*.cpp)
endif

ifeq ($(BUILD_LIQUID), true)
	FILE_LIST += $(wildcard $(LOCAL_PATH)/liquid/*.cpp)
endif

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES := $(PROJECT_ROOT) $(LOCAL_PATH)
LOCAL_STATIC_LIBRARIES := experiments
LOCAL_LDLIBS := -landroid
include $(BUILD_SHARED_LIBRARY)

# experiments
# ------------------------------------
EXPERIMENT_PATH := $(PROJECT_ROOT)/experiments
include $(EXPERIMENT_PATH)/Android.mk
