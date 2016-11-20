cmake_minimum_required(VERSION 3.3)

project("elements")

# Third-party
set(EXTERNAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third-party)
set(EXTERNAL_INCLUDES)
set(EXTERNAL_LIBRARIES)

# assimp

set(EXTERNAL_PATH_ASSIMP ${EXTERNAL_PATH}/assimp)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_PATH_ASSIMP})
target_compile_options(assimp PRIVATE -fexceptions -frtti)

# build importers
target_compile_definitions(assimp PRIVATE
    ASSIMP_BUILD_NO_X_IMPORTER
    ASSIMP_BUILD_NO_3DS_IMPORTER
    ASSIMP_BUILD_NO_MD3_IMPORTER
    ASSIMP_BUILD_NO_MDL_IMPORTER
    ASSIMP_BUILD_NO_MD2_IMPORTER
    ASSIMP_BUILD_NO_PLY_IMPORTER
    ASSIMP_BUILD_NO_ASE_IMPORTER
    ASSIMP_BUILD_NO_HMP_IMPORTER
    ASSIMP_BUILD_NO_SMD_IMPORTER
    ASSIMP_BUILD_NO_MDC_IMPORTER
    ASSIMP_BUILD_NO_MD5_IMPORTER
    ASSIMP_BUILD_NO_STL_IMPORTER
    ASSIMP_BUILD_NO_LWO_IMPORTER
    ASSIMP_BUILD_NO_DXF_IMPORTER
    ASSIMP_BUILD_NO_NFF_IMPORTER
    ASSIMP_BUILD_NO_RAW_IMPORTER
    ASSIMP_BUILD_NO_OFF_IMPORTER
    ASSIMP_BUILD_NO_AC_IMPORTER
    ASSIMP_BUILD_NO_BVH_IMPORTER
    ASSIMP_BUILD_NO_IRRMESH_IMPORTER
    ASSIMP_BUILD_NO_IRR_IMPORTER
    ASSIMP_BUILD_NO_Q3D_IMPORTER
    ASSIMP_BUILD_NO_B3D_IMPORTER
    ASSIMP_BUILD_NO_TERRAGEN_IMPORTER
    ASSIMP_BUILD_NO_CSM_IMPORTER
    ASSIMP_BUILD_NO_3D_IMPORTER
    ASSIMP_BUILD_NO_LWS_IMPORTER
    ASSIMP_BUILD_NO_OGRE_IMPORTER
    ASSIMP_BUILD_NO_MS3D_IMPORTER
    ASSIMP_BUILD_NO_COB_IMPORTER
    ASSIMP_BUILD_NO_Q3BSP_IMPORTER
    ASSIMP_BUILD_NO_NDO_IMPORTER
    ASSIMP_BUILD_NO_IFC_IMPORTER
    ASSIMP_BUILD_NO_XGL_IMPORTER
    ASSIMP_BUILD_NO_FBX_IMPORTER
    ASSIMP_BUILD_NO_C4D_IMPORTER
    ASSIMP_BUILD_NO_ASSBIN_IMPORTER
    ASSIMP_BUILD_NO_BLEND_IMPORTER
    ASSIMP_BUILD_NO_GEO_IMPORTER
    ASSIMP_BUILD_NO_IRRMESH_IMPORTER)

# build processes
target_compile_definitions(assimp PRIVATE
    ASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS
    ASSIMP_BUILD_NO_DEBONE_PROCESS
    ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS
    ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS
    ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS
    ASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS
    ASSIMP_BUILD_NO_GENFACENORMALS_PROCESS
    ASSIMP_BUILD_NO_GENUVCOORDS_PROCESS
    ASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS
    ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS
    ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS
    ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS
    ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS
    ASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS
    ASSIMP_BUILD_NO_REMOVEVC_PROCESS
    ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS
    ASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS
    ASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS
    ASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS
    ASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS)

list(APPEND EXTERNAL_INCLUDES ${EXTERNAL_PATH_ASSIMP}/include)
list(APPEND EXTERNAL_LIBRARIES assimp)

#freetype

set(EXTERNAL_PATH_FREETYPE ${EXTERNAL_PATH}/freetype2)
add_subdirectory(${EXTERNAL_PATH_FREETYPE})
target_compile_definitions(freetype PRIVATE FT2_BUILD_LIBRARY)
list(APPEND EXTERNAL_INCLUDES ${EXTERNAL_PATH_FREETYPE}/include)
list(APPEND EXTERNAL_LIBRARIES freetype)

# glm

set(EXTERNAL_PATH_GLM ${EXTERNAL_PATH}/glm)
list(APPEND EXTERNAL_INCLUDES ${EXTERNAL_PATH_GLM})

# libpng

set(EXTERNAL_PATH_PNG ${EXTERNAL_PATH}/libpng)
configure_file(${EXTERNAL_PATH_PNG}/scripts/pnglibconf.h.prebuilt
               ${EXTERNAL_PATH_PNG}/pnglibconf.h)
file(GLOB LIBPNG_SOURCE_FILES ${EXTERNAL_PATH_PNG}/*.h ${EXTERNAL_PATH_PNG}/*.c)
list(REMOVE_ITEM LIBPNG_SOURCE_FILES ${EXTERNAL_PATH_PNG}/pngtest.c)
add_library(png STATIC ${LIBPNG_SOURCE_FILES})
list(APPEND EXTERNAL_INCLUDES ${EXTERNAL_PATH_PNG})
list(APPEND EXTERNAL_LIBRARIES png)

# pugixml

set(EXTERNAL_PATH_PUGIXML ${EXTERNAL_PATH}/pugixml)
add_subdirectory(${EXTERNAL_PATH_PUGIXML})
target_compile_definitions(pugixml PRIVATE PUGIXML_NO_EXCEPTIONS)
list(APPEND EXTERNAL_INCLUDES ${EXTERNAL_PATH_PUGIXML}/src)
list(APPEND EXTERNAL_LIBRARIES pugixml)

# snape

set(EXTERNAL_PATH_SNAPE ${EXTERNAL_PATH}/snape)
list(APPEND EXTERNAL_INCLUDES ${EXTERNAL_PATH_SNAPE}/include)

# elements

set(ELEMENTS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/elements)

set(ELEMENTS_SOURCE_ASSETS_PATH ${ELEMENTS_PATH}/assets)
set(ELEMENTS_SOURCE_ASSETS
    ${ELEMENTS_SOURCE_ASSETS_PATH}/asset_blob.cpp
    ${ELEMENTS_SOURCE_ASSETS_PATH}/asset_blob.h
    ${ELEMENTS_SOURCE_ASSETS_PATH}/asset_model.cpp
    ${ELEMENTS_SOURCE_ASSETS_PATH}/asset_model.h
    ${ELEMENTS_SOURCE_ASSETS_PATH}/asset_texture.cpp
    ${ELEMENTS_SOURCE_ASSETS_PATH}/asset_texture.h
    ${ELEMENTS_SOURCE_ASSETS_PATH}/asset_xml.cpp
    ${ELEMENTS_SOURCE_ASSETS_PATH}/asset_xml.h
    ${ELEMENTS_SOURCE_ASSETS_PATH}/assets_storage.cpp
    ${ELEMENTS_SOURCE_ASSETS_PATH}/assets_storage.h
    ${ELEMENTS_SOURCE_ASSETS_PATH}/assets.h)

set(ELEMENTS_SOURCE_COLLISION_PATH ${ELEMENTS_PATH}/collision)
set(ELEMENTS_SOURCE_COLLISION
    ${ELEMENTS_SOURCE_COLLISION_PATH}/aabb.cpp
    ${ELEMENTS_SOURCE_COLLISION_PATH}/aabb.h
    ${ELEMENTS_SOURCE_COLLISION_PATH}/collide.cpp
    ${ELEMENTS_SOURCE_COLLISION_PATH}/collide.h
    ${ELEMENTS_SOURCE_COLLISION_PATH}/contact.h)

set(ELEMENTS_SOURCE_IO_PATH ${ELEMENTS_PATH}/io)
set(ELEMENTS_SOURCE_IO
    ${ELEMENTS_SOURCE_IO_PATH}/utils/file_guard.h
    ${ELEMENTS_SOURCE_IO_PATH}/file.h
    ${ELEMENTS_SOURCE_IO_PATH}/path.cpp
    ${ELEMENTS_SOURCE_IO_PATH}/path.h
    ${ELEMENTS_SOURCE_IO_PATH}/system.h)

set(ELEMENTS_SOURCE_MATH_PATH ${ELEMENTS_PATH}/math)
set(ELEMENTS_SOURCE_MATH
    ${ELEMENTS_SOURCE_MATH_PATH}/common.h
    ${ELEMENTS_SOURCE_MATH_PATH}/constants.h
    ${ELEMENTS_SOURCE_MATH_PATH}/geometric.h
    ${ELEMENTS_SOURCE_MATH_PATH}/matrix.h
    ${ELEMENTS_SOURCE_MATH_PATH}/quaternion.h
    ${ELEMENTS_SOURCE_MATH_PATH}/transform.h
    ${ELEMENTS_SOURCE_MATH_PATH}/trigonometry.h
    ${ELEMENTS_SOURCE_MATH_PATH}/types.h)

set(ELEMENTS_SOURCE_METRICS_PATH ${ELEMENTS_PATH}/metrics)
set(ELEMENTS_SOURCE_METRICS
    ${ELEMENTS_SOURCE_METRICS_PATH}/metrics.cpp
    ${ELEMENTS_SOURCE_METRICS_PATH}/metrics.h)

set(ELEMENTS_SOURCE_PREFERENCES_PATH ${ELEMENTS_PATH}/preferences)
set(ELEMENTS_SOURCE_PREFERENCES
    ${ELEMENTS_SOURCE_PREFERENCES_PATH}/preferences.cpp
    ${ELEMENTS_SOURCE_PREFERENCES_PATH}/preferences.h)

set(ELEMENTS_SOURCE_RENDERING_PATH ${ELEMENTS_PATH}/rendering)
set(ELEMENTS_SOURCE_RENDERING
    ${ELEMENTS_SOURCE_RENDERING_PATH}/computation/compute_product_index.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/computation/compute_product_index.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/computation/compute_target.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/computation/compute_target.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/buffer.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/opengl.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/program.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/program.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/shader.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/shader.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/target_attachment.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/target_buffered.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/target_buffered.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/target_maker.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/target_maker.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/target.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/target.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/texture_maker.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/texture_policy.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/texture.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/core/texture.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/blend.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/blend.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/blur.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/blur.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/clear.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/clear.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/forwards.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/gradient.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/gradient.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/light_scattered.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/light_scattered.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/tone.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/effects/tone.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/models/model_warehouse.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/models/model_warehouse.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/models/model.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/models/model.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/models/process_load_model.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/models/process_load_model.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_base.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_composition.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_composition.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_slot.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_target_input.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_target_input.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_target_placement.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_target_placement.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_target_storage.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_target_storage.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_target.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/passes/pass_target.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/primitives/icosphere.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/primitives/icosphere.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/primitives/square.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/primitives/square.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/state/state_macro.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/state/state_traits.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/state/state.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/forward_process.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/forward_process.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/forward.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/forward.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_geometry_pass.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_geometry_pass.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_geometry_process.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_geometry_process.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_lighting_pass.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_lighting_pass.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_lighting_process.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_lighting_process.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_reconstruct_pass.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_reconstruct_pass.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_reconstruct_process.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp_reconstruct_process.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/techniques/lpp.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/utils/program_data.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/utils/program_data.h
    ${ELEMENTS_SOURCE_RENDERING_PATH}/utils/program_loader.cpp
    ${ELEMENTS_SOURCE_RENDERING_PATH}/utils/program_loader.h)

set(ELEMENTS_SOURCE_SCENE_PATH ${ELEMENTS_PATH}/scene)
set(ELEMENTS_SOURCE_SCENE
    ${ELEMENTS_SOURCE_SCENE_PATH}/camera/camera.cpp
    ${ELEMENTS_SOURCE_SCENE_PATH}/camera/camera.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/entity/entity.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/entity/geometry.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/entity/materials.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/entity/mesh.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/entity/vertices.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/graph/node.cpp
    ${ELEMENTS_SOURCE_SCENE_PATH}/graph/node.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/light/light.cpp
    ${ELEMENTS_SOURCE_SCENE_PATH}/light/light.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/modifier/modifier.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/modifier/positioning.cpp
    ${ELEMENTS_SOURCE_SCENE_PATH}/modifier/positioning.h
    ${ELEMENTS_SOURCE_SCENE_PATH}/scene.cpp
    ${ELEMENTS_SOURCE_SCENE_PATH}/scene.h)

set(ELEMENTS_SOURCE_SIMULATION_PATH ${ELEMENTS_PATH}/simulation)
set(ELEMENTS_SOURCE_SIMULATION
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/air/system.cpp
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/air/system.h
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/air/volume.cpp
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/air/volume.h
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/liquid/config.h
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/liquid/config.cpp
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/liquid/obstacle.h
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/liquid/particle.h
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/liquid/system.cpp
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/liquid/system.h
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/liquid/volume.cpp
    ${ELEMENTS_SOURCE_SIMULATION_PATH}/liquid/volume.h)

set(ELEMENTS_SOURCE_SYNCHRONIZATION_PATH ${ELEMENTS_PATH}/synchronization)
set(ELEMENTS_SOURCE_SYNCHRONIZATION
    ${ELEMENTS_SOURCE_SYNCHRONIZATION_PATH}/future.h
    ${ELEMENTS_SOURCE_SYNCHRONIZATION_PATH}/signal.h
    ${ELEMENTS_SOURCE_SYNCHRONIZATION_PATH}/task.h)

set(ELEMENTS_SOURCE_TIMING_PATH ${ELEMENTS_PATH}/timing)
set(ELEMENTS_SOURCE_TIMING
    ${ELEMENTS_SOURCE_TIMING_PATH}/framerate.cpp
    ${ELEMENTS_SOURCE_TIMING_PATH}/framerate.h
    ${ELEMENTS_SOURCE_TIMING_PATH}/timer.cpp
    ${ELEMENTS_SOURCE_TIMING_PATH}/timer.h
    ${ELEMENTS_SOURCE_TIMING_PATH}/update.cpp
    ${ELEMENTS_SOURCE_TIMING_PATH}/update.h)

set(ELEMENTS_SOURCE_UI_PATH ${ELEMENTS_PATH}/ui)
set(ELEMENTS_SOURCE_UI
    ${ELEMENTS_SOURCE_UI_PATH}/controls/button.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/controls/button.h
    ${ELEMENTS_SOURCE_UI_PATH}/controls/check.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/controls/check.h
    ${ELEMENTS_SOURCE_UI_PATH}/controls/label.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/controls/label.h
    ${ELEMENTS_SOURCE_UI_PATH}/controls/panel.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/controls/panel.h
    ${ELEMENTS_SOURCE_UI_PATH}/controls/slider.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/controls/slider.h
    ${ELEMENTS_SOURCE_UI_PATH}/freetype/forwards.h
    ${ELEMENTS_SOURCE_UI_PATH}/freetype/line.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/freetype/line.h
    ${ELEMENTS_SOURCE_UI_PATH}/freetype/manager.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/freetype/manager.h
    ${ELEMENTS_SOURCE_UI_PATH}/control.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/control.h
    ${ELEMENTS_SOURCE_UI_PATH}/system.cpp
    ${ELEMENTS_SOURCE_UI_PATH}/system.h
    ${ELEMENTS_SOURCE_UI_PATH}/types.h)

set(ELEMENTS_SOURCE_UTILS_PATH ${ELEMENTS_PATH}/utils)
set(ELEMENTS_SOURCE_UTILS
    ${ELEMENTS_SOURCE_UTILS_PATH}/std/dependency_index.h
    ${ELEMENTS_SOURCE_UTILS_PATH}/std/enum.h
    ${ELEMENTS_SOURCE_UTILS_PATH}/std/iterator_range.h
    ${ELEMENTS_SOURCE_UTILS_PATH}/std/optional.h
    ${ELEMENTS_SOURCE_UTILS_PATH}/std/pointer.h
    ${ELEMENTS_SOURCE_UTILS_PATH}/std/product.h
    ${ELEMENTS_SOURCE_UTILS_PATH}/std/values.h)

add_library(elements STATIC
    ${ELEMENTS_SOURCE_ASSETS}
    ${ELEMENTS_SOURCE_COLLISION}
    ${ELEMENTS_SOURCE_IO}
    ${ELEMENTS_SOURCE_MATH}
    ${ELEMENTS_SOURCE_METRICS}
    ${ELEMENTS_SOURCE_PREFERENCES}
    ${ELEMENTS_SOURCE_RENDERING}
    ${ELEMENTS_SOURCE_SCENE}
    ${ELEMENTS_SOURCE_SIMULATION}
    ${ELEMENTS_SOURCE_SYNCHRONIZATION}
    ${ELEMENTS_SOURCE_TIMING}
    ${ELEMENTS_SOURCE_UI}
    ${ELEMENTS_SOURCE_UTILS})

target_include_directories(elements PRIVATE ${ELEMENTS_PATH} ${EXTERNAL_INCLUDES})
target_include_directories(elements PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ELEMENTS_PATH}
    ${EXTERNAL_PATH_SNAPE}/include
    ${EXTERNAL_PATH_GLM})

target_compile_options(elements PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)
target_compile_options(elements PUBLIC $<$<CONFIG:DEBUG>:-Wall -Wextra -Werror>)
target_compile_options(elements PUBLIC $<$<CONFIG:RELEASE>:-O3 -ffast-math -Wall -Wextra -Werror>)

#experiments
set(EXPERIMENTS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/experiments)

if(EXPERIMENTS_LIQUID)
    set(SOURCE_FILES
        ${EXPERIMENTS_PATH}/liquid/pass_liquid.cpp
        ${EXPERIMENTS_PATH}/liquid/pass_liquid.h
        ${EXPERIMENTS_PATH}/liquid/pass_metaballs.cpp
        ${EXPERIMENTS_PATH}/liquid/pass_metaballs.h
        ${EXPERIMENTS_PATH}/liquid/pass_particles.cpp
        ${EXPERIMENTS_PATH}/liquid/pass_particles.h
        ${EXPERIMENTS_PATH}/liquid/renderer.cpp
        ${EXPERIMENTS_PATH}/liquid/renderer.h)
    list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
endif()

if(EXPERIMENTS_FIRE)
    set(SOURCE_FILES
        ${EXPERIMENTS_PATH}/fire/pass_fire.cpp
        ${EXPERIMENTS_PATH}/fire/pass_fire.h
        ${EXPERIMENTS_PATH}/fire/pass_raymarching.cpp
        ${EXPERIMENTS_PATH}/fire/pass_raymarching.h
        ${EXPERIMENTS_PATH}/fire/renderer.cpp
        ${EXPERIMENTS_PATH}/fire/renderer.h)
    list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
endif()

if(EXPERIMENTS_LIGHT)
    set(SOURCE_FILES
        ${EXPERIMENTS_PATH}/light/pass_occluding.cpp
        ${EXPERIMENTS_PATH}/light/pass_occluding.h
        ${EXPERIMENTS_PATH}/light/pass_particles.cpp
        ${EXPERIMENTS_PATH}/light/pass_particles.h
        ${EXPERIMENTS_PATH}/light/pass_positions.cpp
        ${EXPERIMENTS_PATH}/light/pass_positions.h
        ${EXPERIMENTS_PATH}/light/pass_velocities.cpp
        ${EXPERIMENTS_PATH}/light/pass_velocities.h
        ${EXPERIMENTS_PATH}/light/renderer.cpp
        ${EXPERIMENTS_PATH}/light/renderer.h)
    list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
endif()

if(EXPERIMENTS_AIR)
    set(SOURCE_FILES
        ${EXPERIMENTS_PATH}/air/pass_particles.cpp
        ${EXPERIMENTS_PATH}/air/pass_particles.h
        ${EXPERIMENTS_PATH}/air/pass_positions.cpp
        ${EXPERIMENTS_PATH}/air/pass_positions.h
        ${EXPERIMENTS_PATH}/air/pass_velocities.cpp
        ${EXPERIMENTS_PATH}/air/pass_velocities.h
        ${EXPERIMENTS_PATH}/air/renderer.cpp
        ${EXPERIMENTS_PATH}/air/renderer.h)
    list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
endif()

if(EXPERIMENTS_STRANGE)
    set(SOURCE_FILES
        ${EXPERIMENTS_PATH}/strange/attractors.cpp
        ${EXPERIMENTS_PATH}/strange/attractors.h
        ${EXPERIMENTS_PATH}/strange/particles_product.cpp
        ${EXPERIMENTS_PATH}/strange/particles_product.h
        ${EXPERIMENTS_PATH}/strange/pass_particles.cpp
        ${EXPERIMENTS_PATH}/strange/pass_particles.h
        ${EXPERIMENTS_PATH}/strange/pass_positions.cpp
        ${EXPERIMENTS_PATH}/strange/pass_positions.h
        ${EXPERIMENTS_PATH}/strange/renderer.cpp
        ${EXPERIMENTS_PATH}/strange/renderer.h)
    list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
endif()

if(EXPERIMENTS_CHARACTER)
    set(SOURCE_FILES
        ${EXPERIMENTS_PATH}/character/renderer.cpp
        ${EXPERIMENTS_PATH}/character/renderer.h)
    list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
endif()

if(EXPERIMENTS_DEMO)
    set(SOURCE_FILES
        ${EXPERIMENTS_PATH}/demo/renderer.cpp
        ${EXPERIMENTS_PATH}/demo/renderer.h)
    list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
endif()

if(EXPERIMENTS_SOURCE_FILES)
    if(PLATFORM_ANDROID)
        set(JNI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/platform/android/elements/libs/Elements/src/main/jni)
        set(SOURCE_FILES
            ${JNI_PATH}/asset_fs.cpp
            ${JNI_PATH}/asset_fs.h
            ${JNI_PATH}/jni_common.cpp
            ${JNI_PATH}/jni_environment.h
            ${JNI_PATH}/logging.h
            ${JNI_PATH}/metrics.cpp
            ${JNI_PATH}/metrics.h
            ${JNI_PATH}/preferences.cpp
            ${JNI_PATH}/preferences.h
            ${JNI_PATH}/renderer_factory.h)
        list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})

        if(EXPERIMENTS_LIQUID)
            set(SOURCE_FILES
                ${JNI_PATH}/liquid/liquid_config.cpp
                ${JNI_PATH}/liquid/liquid_config.h
                ${JNI_PATH}/liquid/liquid_jni.cpp
                ${JNI_PATH}/liquid/liquid_renderer.cpp
                ${JNI_PATH}/liquid/liquid_renderer.h
                ${JNI_PATH}/liquid/liquid_ui.cpp
                ${JNI_PATH}/liquid/liquid_ui.h)
            list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
        endif()

        if(EXPERIMENTS_FIRE)
            set(SOURCE_FILES
                ${JNI_PATH}/fire/fire_config.cpp
                ${JNI_PATH}/fire/fire_config.h
                ${JNI_PATH}/fire/fire_jni.cpp
                ${JNI_PATH}/fire/fire_renderer.cpp
                ${JNI_PATH}/fire/fire_renderer.h
                ${JNI_PATH}/fire/fire_ui.cpp
                ${JNI_PATH}/fire/fire_ui.h)
            list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
        endif()

        if(EXPERIMENTS_LIGHT)
            set(SOURCE_FILES
                ${JNI_PATH}/light/light_jni.cpp
                ${JNI_PATH}/light/light_renderer.h
                ${JNI_PATH}/light/light_renderer.cpp)
            list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
        endif()

        if(EXPERIMENTS_STRANGE)
            set(SOURCE_FILES
                ${JNI_PATH}/strange/strange_jni.cpp
                ${JNI_PATH}/strange/strange_renderer.h
                ${JNI_PATH}/strange/strange_renderer.cpp)
            list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
        endif()

        if(EXPERIMENTS_AIR)
            set(SOURCE_FILES
                ${JNI_PATH}/air/air_jni.cpp
                ${JNI_PATH}/air/air_renderer.h
                ${JNI_PATH}/air/air_renderer.cpp)
            list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
        endif()

        if(EXPERIMENTS_CHARACTER)
            set(SOURCE_FILES
                ${JNI_PATH}/character/character_jni.cpp
                ${JNI_PATH}/character/character_renderer.h
                ${JNI_PATH}/character/character_renderer.cpp)
            list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
        endif()

        if(EXPERIMENTS_DEMO)
            set(SOURCE_FILES
                ${JNI_PATH}/demo/demo_jni.cpp
                ${JNI_PATH}/demo/demo_renderer.h
                ${JNI_PATH}/demo/demo_renderer.cpp)
            list(APPEND EXPERIMENTS_SOURCE_FILES ${SOURCE_FILES})
        endif()

        find_library(NDK_LIBRARY_ANDROID NAMES android)
        if(NOT NDK_LIBRARY_ANDROID)
            message(FATAL_ERROR "Can not find NDK Android library")
        endif()

        find_library(NDK_LIBRARY_LOG NAMES log)
        if(NOT NDK_LIBRARY_LOG)
            message(FATAL_ERROR "Can not find NDK Log library")
        endif()

        find_library(NDK_LIBRARY_GLSE2 NAMES GLESv2)
        if(NOT NDK_LIBRARY_GLSE2)
            message(FATAL_ERROR "Can not find NDK GLESv2 library")
        endif()

        add_library(experiments SHARED ${EXPERIMENTS_SOURCE_FILES})
        target_link_libraries(experiments ${NDK_LIBRARY_ANDROID} elements atomic
            ${EXTERNAL_LIBRARIES} ${NDK_LIBRARY_GLSE2} ${NDK_LIBRARY_LOG})
        target_include_directories(experiments PRIVATE ${JNI_PATH})
        target_compile_options(experiments PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)
        target_compile_options(experiments PUBLIC $<$<CONFIG:DEBUG>:-Wall -Wextra -Werror>)
        target_compile_options(experiments PUBLIC $<$<CONFIG:RELEASE>:-O3 -ffast-math -Wall -Wextra -Werror>)

    endif()
endif()
