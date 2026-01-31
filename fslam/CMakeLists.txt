cmake_minimum_required(VERSION 4.2.0)

project(slam
    VERSION 1.0.0
    DESCRIPTION "All computer vision related functionality"
    LANGUAGES CXX
)

file(GLOB_RECURSE HEADERS
    include/*.hpp
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)




set(VISION_DEPS
    fmt 
    spdlog
    eigen
    yaml
    ${VISION_DEPS}
)




find_package(OpenCV REQUIRED core imgproc highgui opencv_calib3d opencv_features2d opencv_imgcodecs)
if(OpenCV_FOUND)
    message(STATUS "OpenCV found, adding to ct_vision dependencies")
    list(APPEND VISION_DEPS ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "OpenCV not found, but is required for ct_vision module")
endif()

add_module(slam
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDENCIES ${VISION_DEPS}
)

