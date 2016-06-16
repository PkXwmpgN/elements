LOCAL_PATH := $(call my-dir)
PROJECT_ROOT:= $(LOCAL_PATH)/..

# experiments
# ------------------------------------

include $(CLEAR_VARS)

EXPERIMENTS_PATH := $(PROJECT_ROOT)/experiments
LOCAL_MODULE := experiments
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror

ifeq ($(BUILD_FIRE), true)
	LOCAL_SRC_FILES += $(EXPERIMENTS_PATH)/fire/renderer.cpp \
		$(EXPERIMENTS_PATH)/fire/pass_raymarching.cpp \
		$(EXPERIMENTS_PATH)/fire/pass_fire.cpp
endif

ifeq ($(BUILD_LIGHT), true)
	LOCAL_SRC_FILES += $(EXPERIMENTS_PATH)/light/pass_particles.cpp \
		$(EXPERIMENTS_PATH)/light/pass_occluding.cpp \
		$(EXPERIMENTS_PATH)/light/pass_velocities.cpp \
		$(EXPERIMENTS_PATH)/light/pass_positions.cpp \
		$(EXPERIMENTS_PATH)/light/renderer.cpp
endif

ifeq ($(BUILD_STRANGE), true)
	LOCAL_SRC_FILES += $(EXPERIMENTS_PATH)/strange/attractors.cpp \
		$(EXPERIMENTS_PATH)/strange/renderer.cpp \
		$(EXPERIMENTS_PATH)/strange/particles_product.cpp \
		$(EXPERIMENTS_PATH)/strange/pass_positions.cpp \
		$(EXPERIMENTS_PATH)/strange/pass_particles.cpp
endif

ifeq ($(BUILD_AIR), true)
	LOCAL_SRC_FILES += $(EXPERIMENTS_PATH)/air/pass_velocities.cpp \
		$(EXPERIMENTS_PATH)/air/pass_positions.cpp \
		$(EXPERIMENTS_PATH)/air/pass_particles.cpp \
		$(EXPERIMENTS_PATH)/air/renderer.cpp
endif

ifeq ($(BUILD_LIQUID), true)
	LOCAL_SRC_FILES += $(EXPERIMENTS_PATH)/liquid/renderer.cpp \
		$(EXPERIMENTS_PATH)/liquid/pass_particles.cpp \
		$(EXPERIMENTS_PATH)/liquid/pass_metaballs.cpp \
		$(EXPERIMENTS_PATH)/liquid/pass_liquid.cpp
endif

LOCAL_C_INCLUDES += $(PROJECT_ROOT)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_STATIC_LIBRARIES := elements_engine
include $(BUILD_STATIC_LIBRARY)

# elements engine
# ------------------------------------
ELEMENTS_PATH := $(PROJECT_ROOT)/elements
include $(ELEMENTS_PATH)/Android.mk
