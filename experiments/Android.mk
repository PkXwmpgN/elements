LOCAL_PATH := $(call my-dir)

# experiments
# ------------------------------------

include $(CLEAR_VARS)

LOCAL_MODULE := experiments
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror

ifeq ($(BUILD_FIRE), true)
	LOCAL_SRC_FILES += fire/renderer.cpp \
		fire/pass_raymarching.cpp \
		fire/pass_fire.cpp
endif

ifeq ($(BUILD_LIGHT), true)
	LOCAL_SRC_FILES += light/pass_particles.cpp \
		light/pass_occluding.cpp \
		light/pass_velocities.cpp \
		light/pass_positions.cpp \
		light/renderer.cpp
endif

ifeq ($(BUILD_STRANGE), true)
	LOCAL_SRC_FILES += strange/attractors.cpp \
		strange/renderer.cpp \
		strange/particles_product.cpp \
		strange/pass_positions.cpp \
		strange/pass_particles.cpp
endif

ifeq ($(BUILD_AIR), true)
	LOCAL_SRC_FILES += air/pass_velocities.cpp \
		air/pass_positions.cpp \
		air/pass_particles.cpp \
		air/renderer.cpp
endif

ifeq ($(BUILD_LIQUID), true)
	LOCAL_SRC_FILES += liquid/renderer.cpp \
		liquid/pass_particles.cpp \
		liquid/pass_metaballs.cpp \
		liquid/pass_liquid.cpp
endif

LOCAL_C_INCLUDES += $(LOCAL_PATH)/..
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_STATIC_LIBRARIES := elements_engine
include $(BUILD_STATIC_LIBRARY)

# elements engine
# ------------------------------------

include $(LOCAL_PATH)/../elements/Android.mk
