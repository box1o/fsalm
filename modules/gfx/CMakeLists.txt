cmake_minimum_required(VERSION 4.2.0)

project(gfx
    VERSION 1.0.0
    DESCRIPTION "Graphics module for rendering"
    LANGUAGES CXX
)

file(GLOB_RECURSE HEADERS
    include/*.hpp
    backends/*.hpp
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    backends/*.cpp
)

set(GFX_DEPS
    toolbox::base
    toolbox::math
    yaml-cpp::yaml-cpp

    glfw
    stb::stb
    shaderc_combined
    glslang
    SPIRV-Tools
    SPIRV-Tools-opt
    spirv-cross-cpp 
    spirv-cross-glsl 
    spirv-cross-hlsl 
    spirv-cross-msl 
    spirv-cross-reflect
    # tinygltf
)



find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
    message(STATUS "Vulkan found, adding to ct_vision dependencies")
    list(APPEND GFX_DEPS Vulkan::Vulkan)  
else()
    message(FATAL_ERROR "Vulkan not found, but is required for ct_vision module")
endif()

# find_package(Eigen3 REQUIRED)
# if(Eigen3_FOUND)
#     message(STATUS "Eigen3 found, adding to ct_vision dependencies")
#     list(APPEND GFX_DEPS Eigen3::Eigen)  
# else()
#     message(FATAL_ERROR "Eigen3 not found, but is required for ct_vision module")
# endif()

add_module(gfx
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDENCIES ${GFX_DEPS}
)

