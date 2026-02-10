# cmake_minimum_required(VERSION 4.2.0)
#
# project(gfx
#     VERSION 1.0.0
#     DESCRIPTION "Graphics module for rendering"
#     LANGUAGES CXX
# )
#
# file(GLOB_RECURSE HEADERS
#     include/*.hpp
#     backends/*.hpp
# )
#
# file(GLOB_RECURSE SOURCES
#     src/*.cpp
#     backends/*.cpp
# )
#
# set(GFX_DEPS
#     toolbox::base
#     toolbox::math
#     yaml-cpp::yaml-cpp
#
#     glfw
#     # stb::stb
#     # shaderc_combined
#     # glslang
#     # SPIRV-Tools
#     # SPIRV-Tools-opt
#     # spirv-cross-cpp 
#     # spirv-cross-glsl 
#     # spirv-cross-hlsl 
#     # spirv-cross-msl 
#     # spirv-cross-reflect
#     # # tinygltf
# )
#
# if (NOT EMSCRITEN)
#     list(APPEND GFX_DEPS webgpu_dawn)  
# else()
#     message(FATAL_ERROR "Added the down lib for now emscriten build")
# endif()
#
#
#
#
# # find_package(Vulkan REQUIRED)
# # if(Vulkan_FOUND)
# #     message(STATUS "Vulkan found, adding to ct_vision dependencies")
# #     list(APPEND GFX_DEPS Vulkan::Vulkan)  
# # else()
# #     message(FATAL_ERROR "Vulkan not found, but is required for ct_vision module")
# # endif()
# #
# # find_package(Eigen3 REQUIRED)
# # if(Eigen3_FOUND)
# #     message(STATUS "Eigen3 found, adding to ct_vision dependencies")
# #     list(APPEND GFX_DEPS Eigen3::Eigen)  
# # else()
# #     message(FATAL_ERROR "Eigen3 not found, but is required for ct_vision module")
# # endif()
#
# add_module(gfx
#     SOURCES ${SOURCES}
#     HEADERS ${HEADERS}
#     DEPENDENCIES ${GFX_DEPS}
# )
#



cmake_minimum_required(VERSION 4.2.0)

project(gfx
    VERSION 1.0.0
    DESCRIPTION "Graphics module for rendering"
    LANGUAGES CXX
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/*.hpp
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/*.cpp
)

set(GFX_DEPS
    toolbox::base
    toolbox::math
    yaml-cpp::yaml-cpp
)

# Native-only deps
if(NOT EMSCRIPTEN)
    list(APPEND GFX_DEPS
        glfw
        webgpu_dawn
    )
else()
    message(STATUS "gfx: EMSCRIPTEN build -> using emdawnwebgpu port (no glfw, no webgpu_dawn)")
    # target_compile_definitions(gfx INTERFACE TOOLBOX_GFX_WEB)
endif()

add_module(gfx
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDENCIES ${GFX_DEPS}
)
