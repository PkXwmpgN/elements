LOCAL_PATH := $(call my-dir)

# libpng
# ------------------------------------
include $(CLEAR_VARS)

LIBPNG_PATH := ../third-party/libpng
LOCAL_MODULE := png
LOCAL_CFLAGS := -O3 -s -DNDEBUG -ffast-math
FILE_LIST := $(wildcard $(LOCAL_PATH)/$(LIBPNG_PATH)/*.c)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
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

# assimp
# ------------------------------------

include $(CLEAR_VARS)

ASSIMP_PATH := ../third-party/assimp-3.2
LOCAL_MODULE := assimp
LOCAL_CPP_FEATURES := exceptions rtti

ASSIMP_SRC_DIR := code

FILE_LIST := $(wildcard $(LOCAL_PATH)/$(ASSIMP_PATH)/$(ASSIMP_SRC_DIR)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/$(ASSIMP_PATH)/contrib/openddlparser/code/*.cpp)
#FILE_LIST += $(wildcard $(LOCAL_PATH)/$(ASSIMP_PATH)/contrib/unzip/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/$(ASSIMP_PATH)/contrib/poly2tri/poly2tri/*/*.cc)
#FILE_LIST += $(LOCAL_PATH)/$(ASSIMP_PATH)/contrib/clipper/clipper.cpp
FILE_LIST += $(LOCAL_PATH)/$(ASSIMP_PATH)/contrib/ConvertUTF/ConvertUTF.c
FILE_LIST += $(LOCAL_PATH)/$(ASSIMP_PATH)/contrib/irrXML/irrXML.cpp

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

DONT_BUILD_PROCESS = \
    -DASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS \
    -DASSIMP_BUILD_NO_DEBONE_PROCESS \
    -DASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS \
    -DASSIMP_BUILD_NO_FINDINSTANCES_PROCESS \
    -DASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS \
    -DASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS \
    -DASSIMP_BUILD_NO_GENFACENORMALS_PROCESS \
    -DASSIMP_BUILD_NO_GENUVCOORDS_PROCESS \
    -DASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS \
    -DASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS \
    -DASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS \
    -DASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS \
    -DASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS \
    -DASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS \
    -DASSIMP_BUILD_NO_REMOVEVC_PROCESS \
    -DASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS \
    -DASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS \
    -DASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS \
    -DASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS \
    -DASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS

DONT_BUILD_IMPORTERS = \
    -DASSIMP_BUILD_NO_X_IMPORTER \
    -DASSIMP_BUILD_NO_3DS_IMPORTER \
    -DASSIMP_BUILD_NO_MD3_IMPORTER \
    -DASSIMP_BUILD_NO_MDL_IMPORTER \
    -DASSIMP_BUILD_NO_MD2_IMPORTER \
    -DASSIMP_BUILD_NO_PLY_IMPORTER \
    -DASSIMP_BUILD_NO_ASE_IMPORTER \
    -DASSIMP_BUILD_NO_HMP_IMPORTER \
    -DASSIMP_BUILD_NO_SMD_IMPORTER \
    -DASSIMP_BUILD_NO_MDC_IMPORTER \
    -DASSIMP_BUILD_NO_MD5_IMPORTER \
    -DASSIMP_BUILD_NO_STL_IMPORTER \
    -DASSIMP_BUILD_NO_LWO_IMPORTER \
    -DASSIMP_BUILD_NO_DXF_IMPORTER \
    -DASSIMP_BUILD_NO_NFF_IMPORTER \
    -DASSIMP_BUILD_NO_RAW_IMPORTER \
    -DASSIMP_BUILD_NO_OFF_IMPORTER \
    -DASSIMP_BUILD_NO_AC_IMPORTER \
    -DASSIMP_BUILD_NO_BVH_IMPORTER \
    -DASSIMP_BUILD_NO_IRRMESH_IMPORTER \
    -DASSIMP_BUILD_NO_IRR_IMPORTER \
    -DASSIMP_BUILD_NO_Q3D_IMPORTER \
    -DASSIMP_BUILD_NO_B3D_IMPORTER \
    -DASSIMP_BUILD_NO_TERRAGEN_IMPORTER \
    -DASSIMP_BUILD_NO_CSM_IMPORTER \
    -DASSIMP_BUILD_NO_3D_IMPORTER \
    -DASSIMP_BUILD_NO_LWS_IMPORTER \
    -DASSIMP_BUILD_NO_OGRE_IMPORTER \
    -DASSIMP_BUILD_NO_MS3D_IMPORTER \
    -DASSIMP_BUILD_NO_COB_IMPORTER \
    -DASSIMP_BUILD_NO_Q3BSP_IMPORTER \
    -DASSIMP_BUILD_NO_NDO_IMPORTER \
    -DASSIMP_BUILD_NO_IFC_IMPORTER \
    -DASSIMP_BUILD_NO_XGL_IMPORTER \
    -DASSIMP_BUILD_NO_FBX_IMPORTER \
    -DASSIMP_BUILD_NO_C4D_IMPORTER \
    -DASSIMP_BUILD_NO_ASSBIN_IMPORTER \
    -DASSIMP_BUILD_NO_BLEND_IMPORTER \
    -DASSIMP_BUILD_NO_GEO_IMPORTER \
    -DASSIMP_BUILD_NO_IRRMESH_IMPORTER \
#   -DASSIMP_BUILD_NO_OBJ_IMPORTER \


ASSIMP_FLAGS_3_0 := \
    -DASSIMP_BUILD_DLL_EXPORT \
    -DASSIMP_BUILD_NO_OWN_ZLIB \
    -DASSIMP_BUILD_BOOST_WORKAROUND \
    -DASSIMP_EXPORTS -fPIC \
    -fvisibility=hidden -Wall

ASSIMP_FLAGS_3_1 := $(ASSIMP_FLAGS_3_0) -DASSIMP_BUILD_BLENDER_DEBUG

LOCAL_CFLAGS += $(ASSIMP_FLAGS_3_1) \
    -DASSIMP_BUILD_NO_EXPORT \
    -DOPENDDL_NO_USE_CPP11 \
    $(DONT_BUILD_IMPORTERS)  \
    $(DONT_BUILD_PROCESS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(ASSIMP_PATH) \
	$(LOCAL_PATH)/$(ASSIMP_PATH)/include \
    $(LOCAL_PATH)/$(ASSIMP_PATH)/$(ASSIMP_SRC_DIR)/BoostWorkaround \
    $(LOCAL_PATH)/$(ASSIMP_PATH)/contrib/openddlparser/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(ASSIMP_PATH)/include \
    $(LOCAL_PATH)/$(ASSIMP_PATH)/$(ASSIMP_SRC_DIR)/BoostWorkaround

LOCAL_EXPORT_LDLIBS := -lz

include $(BUILD_STATIC_LIBRARY)

# elements
# ------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := elements_engine
LOCAL_CPPFLAGS  := -std=c++14 -O3 -s -DNDEBUG -ffast-math -Wall -Wextra -Werror
FILE_LIST := $(wildcard $(LOCAL_PATH)/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/assets/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/io/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/math/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/metrics/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/preferences/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/rendering/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/scene/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/simulation/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/synchronization/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/timing/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/ui/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/utils/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/design/*/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/../third-party/glm
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_EXPORT_LDLIBS := -latomic -lGLESv2 -llog -L$(SYSROOT)/usr/lib
LOCAL_STATIC_LIBRARIES := png freetype pugixml assimp

include $(BUILD_STATIC_LIBRARY)
