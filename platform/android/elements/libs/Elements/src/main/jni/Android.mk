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
LOCAL_SRC_FILES += \
	jni_common.cpp \
	asset_reader.cpp \
	preferences.cpp \
	metrics.cpp

ifeq ($(BUILD_FIRE), true)
	LOCAL_SRC_FILES += \
		fire/fire_renderer.cpp \
		fire/fire_config.cpp \
		fire/fire_ui.cpp \
		fire/fire_jni.cpp
endif

ifeq ($(BUILD_LIGHT), true)
	LOCAL_SRC_FILES += \
		light/light_jni.cpp \
		light/light_renderer.cpp
endif

ifeq ($(BUILD_STRANGE), true)
	LOCAL_SRC_FILES += \
		strange/strange_jni.cpp \
		strange/strange_renderer.cpp
endif

ifeq ($(BUILD_AIR), true)
	LOCAL_SRC_FILES += \
		air/air_jni.cpp \
		air/air_renderer.cpp
endif

ifeq ($(BUILD_LIQUID), true)
	LOCAL_SRC_FILES += \
		liquid/liquid_jni.cpp \
		liquid/liquid_config.cpp \
		liquid/liquid_renderer.cpp \
		liquid/liquid_ui.cpp
endif

LOCAL_C_INCLUDES := $(PROJECT_ROOT)
LOCAL_STATIC_LIBRARIES := experiments elements_engine png freetype pugixml
LOCAL_LDLIBS := -landroid
include $(BUILD_SHARED_LIBRARY)

# experiments
# ------------------------------------
EXPERIMENT_PATH := $(PROJECT_ROOT)/experiments
include $(EXPERIMENT_PATH)/Android.mk
