LOCAL_PATH := $(call my-dir)
PROJECT_ROOT:= $(LOCAL_PATH)/../..

# experiments
# ------------------------------------
include $(CLEAR_VARS)

EXPERIMENTS_PATH := $(PROJECT_ROOT)/experiments
LOCAL_MODULE := experiments
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror
LOCAL_SRC_FILES := pass_particles.cpp \
				   pass_occluding.cpp \
				   pass_velocities.cpp \
				   pass_positions.cpp \
				   renderer.cpp
LOCAL_C_INCLUDES := $(PROJECT_ROOT) \
                    $(EXPERIMENTS_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_STATIC_LIBRARIES := elements_engine
include $(BUILD_STATIC_LIBRARY)

# elements engine
# ------------------------------------
ELEMENTS_PATH := $(PROJECT_ROOT)/elements
include $(ELEMENTS_PATH)/Android.mk
