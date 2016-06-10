LOCAL_PATH := $(call my-dir)
PROJECT_ROOT:= $(LOCAL_PATH)/..

# libpng
# ------------------------------------
include $(CLEAR_VARS)

LIBPNG_PATH := $(PROJECT_ROOT)/third-party/libpng
LOCAL_MODULE := png
LOCAL_CFLAGS := -O3 -s -DNDEBUG -ffast-math
LOCAL_SRC_FILES := $(wildcard $(LIBPNG_PATH)/*.c)
LOCAL_EXPORT_C_INCLUDES := $(LIBPNG_PATH)
LOCAL_EXPORT_LDLIBS := -lz
include $(BUILD_STATIC_LIBRARY)

# pugixml
# ------------------------------------
include $(CLEAR_VARS)

PUGIXML_PATH := $(PROJECT_ROOT)/third-party/pugixml
LOCAL_MODULE := pugixml
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -DPUGIXML_NO_EXCEPTIONS
LOCAL_SRC_FILES := $(PUGIXML_PATH)/src/pugixml.cpp
LOCAL_EXPORT_C_INCLUDES := $(PUGIXML_PATH)/src
include $(BUILD_STATIC_LIBRARY)

#freetype
#-------------------------------------
include $(CLEAR_VARS)

FREETYPE_PATH := $(PROJECT_ROOT)/third-party/freetype-2.6.2
LOCAL_MODULE := freetype
LOCAL_CFLAGS := -O3 -s -DNDEBUG -ffast-math -DFT2_BUILD_LIBRARY
LOCAL_SRC_FILES := $(FREETYPE_PATH)/src/autofit/autofit.c \
    $(FREETYPE_PATH)/src/base/ftbase.c \
    $(FREETYPE_PATH)/src/base/ftbbox.c \
    $(FREETYPE_PATH)/src/base/ftbdf.c \
    $(FREETYPE_PATH)/src/base/ftbitmap.c \
    $(FREETYPE_PATH)/src/base/ftcid.c \
    $(FREETYPE_PATH)/src/base/ftfntfmt.c \
    $(FREETYPE_PATH)/src/base/ftfstype.c \
    $(FREETYPE_PATH)/src/base/ftgasp.c \
    $(FREETYPE_PATH)/src/base/ftglyph.c \
    $(FREETYPE_PATH)/src/base/ftgxval.c \
    $(FREETYPE_PATH)/src/base/ftinit.c \
    $(FREETYPE_PATH)/src/base/ftlcdfil.c \
    $(FREETYPE_PATH)/src/base/ftmm.c \
    $(FREETYPE_PATH)/src/base/ftotval.c \
    $(FREETYPE_PATH)/src/base/ftpatent.c \
    $(FREETYPE_PATH)/src/base/ftpfr.c \
    $(FREETYPE_PATH)/src/base/ftstroke.c \
    $(FREETYPE_PATH)/src/base/ftsynth.c \
    $(FREETYPE_PATH)/src/base/ftsystem.c \
    $(FREETYPE_PATH)/src/base/fttype1.c \
    $(FREETYPE_PATH)/src/base/ftwinfnt.c \
    $(FREETYPE_PATH)/src/bdf/bdf.c \
    $(FREETYPE_PATH)/src/bzip2/ftbzip2.c \
    $(FREETYPE_PATH)/src/cache/ftcache.c \
    $(FREETYPE_PATH)/src/cff/cff.c \
    $(FREETYPE_PATH)/src/cid/type1cid.c \
    $(FREETYPE_PATH)/src/gzip/ftgzip.c \
    $(FREETYPE_PATH)/src/lzw/ftlzw.c \
    $(FREETYPE_PATH)/src/pcf/pcf.c \
    $(FREETYPE_PATH)/src/pfr/pfr.c \
    $(FREETYPE_PATH)/src/psaux/psaux.c \
    $(FREETYPE_PATH)/src/pshinter/pshinter.c \
    $(FREETYPE_PATH)/src/psnames/psnames.c \
    $(FREETYPE_PATH)/src/raster/raster.c \
    $(FREETYPE_PATH)/src/sfnt/sfnt.c \
    $(FREETYPE_PATH)/src/smooth/smooth.c \
    $(FREETYPE_PATH)/src/truetype/truetype.c \
    $(FREETYPE_PATH)/src/type1/type1.c \
    $(FREETYPE_PATH)/src/type42/type42.c \
    $(FREETYPE_PATH)/src/winfonts/winfnt.c
LOCAL_C_INCLUDES := $(FREETYPE_PATH)/include \
    $(FREETYPE_PATH)/src
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_STATIC_LIBRARY)

# elements
# ------------------------------------
include $(CLEAR_VARS)

ELEMENTS_PATH := $(PROJECT_ROOT)/elements
LOCAL_MODULE := elements_engine
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror
LOCAL_SRC_FILES := $(ELEMENTS_PATH)/rendering/core/shader.cpp \
    $(ELEMENTS_PATH)/rendering/core/program.cpp \
    $(ELEMENTS_PATH)/rendering/computation/compute_product_index.cpp \
    $(ELEMENTS_PATH)/rendering/computation/compute_target.cpp \
    $(ELEMENTS_PATH)/rendering/primitives/square.cpp \
    $(ELEMENTS_PATH)/rendering/effects/light_scattered.cpp \
    $(ELEMENTS_PATH)/rendering/effects/blur.cpp \
    $(ELEMENTS_PATH)/rendering/effects/gradient.cpp \
    $(ELEMENTS_PATH)/rendering/effects/tone.cpp \
    $(ELEMENTS_PATH)/rendering/effects/blend.cpp \
    $(ELEMENTS_PATH)/rendering/effects/clear.cpp \
    $(ELEMENTS_PATH)/rendering/passes/pass_target_placement.cpp \
    $(ELEMENTS_PATH)/rendering/passes/pass_target.cpp \
    $(ELEMENTS_PATH)/rendering/passes/pass_target_input.cpp \
    $(ELEMENTS_PATH)/rendering/passes/pass_target_storage.cpp \
    $(ELEMENTS_PATH)/rendering/passes/pass_composition.cpp \
    $(ELEMENTS_PATH)/rendering/utils/program_loader.cpp \
    $(ELEMENTS_PATH)/ui/freetype/manager.cpp \
    $(ELEMENTS_PATH)/ui/freetype/line.cpp \
    $(ELEMENTS_PATH)/ui/system.cpp \
    $(ELEMENTS_PATH)/ui/control.cpp \
    $(ELEMENTS_PATH)/ui/controls/button.cpp \
    $(ELEMENTS_PATH)/ui/controls/panel.cpp \
    $(ELEMENTS_PATH)/ui/controls/label.cpp \
    $(ELEMENTS_PATH)/ui/controls/slider.cpp \
    $(ELEMENTS_PATH)/assets/assets_storage.cpp \
    $(ELEMENTS_PATH)/assets/asset_texture.cpp \
    $(ELEMENTS_PATH)/assets/asset_blob.cpp \
    $(ELEMENTS_PATH)/assets/asset_xml.cpp \
    $(ELEMENTS_PATH)/assets/asset_operations_stream.cpp \
    $(ELEMENTS_PATH)/preferences/preferences.cpp \
    $(ELEMENTS_PATH)/metrics/metrics.cpp \
    $(ELEMENTS_PATH)/timing/framerate.cpp \
    $(ELEMENTS_PATH)/simulation/liquid/system.cpp \
    $(ELEMENTS_PATH)/simulation/liquid/volume.cpp \
    $(ELEMENTS_PATH)/simulation/liquid/config.cpp \
    $(ELEMENTS_PATH)/simulation/air/system.cpp \
    $(ELEMENTS_PATH)/simulation/air/volume.cpp

LOCAL_C_INCLUDES := $(PROJECT_ROOT) \
    $(PROJECT_ROOT)/elements \
    $(PROJECT_ROOT)/third-party/glm
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_EXPORT_LDLIBS := -latomic -lGLESv2 -llog -L$(SYSROOT)/usr/lib
LOCAL_STATIC_LIBRARIES := png freetype pugixml

include $(BUILD_STATIC_LIBRARY)
