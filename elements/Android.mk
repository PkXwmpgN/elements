LOCAL_PATH := $(call my-dir)

# libpng
# ------------------------------------
include $(CLEAR_VARS)

LIBPNG_PATH := ../third-party/libpng
LOCAL_MODULE := png
LOCAL_CFLAGS := -O3 -s -DNDEBUG -ffast-math
LOCAL_SRC_FILES := $(LIBPNG_PATH)/png.c \
	$(LIBPNG_PATH)/pngerror.c \
	$(LIBPNG_PATH)/pngget.c \
	$(LIBPNG_PATH)/pngmem.c \
	$(LIBPNG_PATH)/pngpread.c \
	$(LIBPNG_PATH)/pngread.c \
	$(LIBPNG_PATH)/pngrio.c \
	$(LIBPNG_PATH)/pngrtran.c \
	$(LIBPNG_PATH)/pngrutil.c \
	$(LIBPNG_PATH)/pngset.c \
	$(LIBPNG_PATH)/pngtest.c \
	$(LIBPNG_PATH)/pngtrans.c \
	$(LIBPNG_PATH)/pngwio.c \
	$(LIBPNG_PATH)/pngwrite.c \
	$(LIBPNG_PATH)/pngwtran.c \
	$(LIBPNG_PATH)/pngwutil.c
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(LIBPNG_PATH)
LOCAL_EXPORT_LDLIBS := -lz
include $(BUILD_STATIC_LIBRARY)

# pugixml
# ------------------------------------
include $(CLEAR_VARS)

PUGIXML_PATH := ../third-party/pugixml
LOCAL_MODULE := pugixml
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -DPUGIXML_NO_EXCEPTIONS
LOCAL_SRC_FILES := $(PUGIXML_PATH)/src/pugixml.cpp
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(PUGIXML_PATH)/src
include $(BUILD_STATIC_LIBRARY)

#freetype
#-------------------------------------
include $(CLEAR_VARS)

FREETYPE_PATH := ../third-party/freetype-2.6.2
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
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(FREETYPE_PATH)/include \
    $(LOCAL_PATH)/$(FREETYPE_PATH)/src
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_STATIC_LIBRARY)

# elements
# ------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := elements_engine
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror
LOCAL_SRC_FILES := rendering/core/shader.cpp \
    rendering/core/program.cpp \
    rendering/computation/compute_product_index.cpp \
    rendering/computation/compute_target.cpp \
    rendering/primitives/square.cpp \
    rendering/effects/light_scattered.cpp \
    rendering/effects/blur.cpp \
    rendering/effects/gradient.cpp \
    rendering/effects/tone.cpp \
    rendering/effects/blend.cpp \
    rendering/effects/clear.cpp \
    rendering/passes/pass_target_placement.cpp \
    rendering/passes/pass_target.cpp \
    rendering/passes/pass_target_input.cpp \
    rendering/passes/pass_target_storage.cpp \
    rendering/passes/pass_composition.cpp \
    rendering/utils/program_loader.cpp \
    ui/freetype/manager.cpp \
    ui/freetype/line.cpp \
    ui/system.cpp \
    ui/control.cpp \
    ui/controls/button.cpp \
    ui/controls/panel.cpp \
    ui/controls/label.cpp \
    ui/controls/slider.cpp \
    assets/assets_storage.cpp \
    assets/asset_texture.cpp \
    assets/asset_blob.cpp \
    assets/asset_xml.cpp \
    assets/asset_operations_stream.cpp \
    preferences/preferences.cpp \
    metrics/metrics.cpp \
    timing/framerate.cpp \
    timing/update.cpp \
    timing/timer.cpp \
    simulation/liquid/system.cpp \
    simulation/liquid/volume.cpp \
    simulation/liquid/config.cpp \
    simulation/air/system.cpp \
    simulation/air/volume.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/../third-party/glm
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_EXPORT_LDLIBS := -latomic -lGLESv2 -llog -L$(SYSROOT)/usr/lib
LOCAL_STATIC_LIBRARIES := png freetype pugixml

include $(BUILD_STATIC_LIBRARY)
