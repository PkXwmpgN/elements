LOCAL_PATH := $(call my-dir)

# experiments
# ------------------------------------

include $(CLEAR_VARS)

LOCAL_MODULE := experiments
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror

ifeq ($(BUILD_FIRE), true)
	EXPERIMETNS_FILES += $(wildcard $(LOCAL_PATH)/fire/*.cpp)
endif

ifeq ($(BUILD_LIGHT), true)
	EXPERIMETNS_FILES += $(wildcard $(LOCAL_PATH)/light/*.cpp)
endif

ifeq ($(BUILD_STRANGE), true)
	EXPERIMETNS_FILES += $(wildcard $(LOCAL_PATH)/strange/*.cpp)
endif

ifeq ($(BUILD_AIR), true)
	EXPERIMETNS_FILES += $(wildcard $(LOCAL_PATH)/air/*.cpp)
endif

ifeq ($(BUILD_LIQUID), true)
	EXPERIMETNS_FILES += $(wildcard $(LOCAL_PATH)/liquid/*.cpp)
endif

LOCAL_SRC_FILES := $(EXPERIMETNS_FILES:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/..
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_STATIC_LIBRARIES := elements_engine
include $(BUILD_STATIC_LIBRARY)

# elements engine
# ------------------------------------

include $(LOCAL_PATH)/../elements/Android.mk
