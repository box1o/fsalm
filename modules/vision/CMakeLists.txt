# cmake_minimum_required(VERSION 4.2.0)
#
# project(vision
#     VERSION 1.0.0
#     DESCRIPTION "All computer vision related functionality"
#     LANGUAGES CXX
# )
#
# file(GLOB_RECURSE HEADERS
#     include/*.hpp
# )
#
# file(GLOB_RECURSE SOURCES
#     src/*.cpp
# )
#
#
#
#
# set(VISION_DEPS
#     toolbox::base
#     toolbox::math
#     yaml-cpp::yaml-cpp
# )
#
#
# # find_package(Eigen3 REQUIRED)
# # if(Eigen3_FOUND)
# #     message(STATUS "Eigen3 found, adding to ct_vision dependencies")
# #     list(APPEND VISION_DEPS Eigen3::Eigen)  
# # else()
# #     message(FATAL_ERROR "Eigen3 not found, but is required for ct_vision module")
# # endif()
#
# find_package(OpenCV REQUIRED core imgproc highgui opencv_calib3d opencv_features2d opencv_imgcodecs)
# if(OpenCV_FOUND)
#     message(STATUS "OpenCV found, adding to ct_vision dependencies")
#     list(APPEND VISION_DEPS ${OpenCV_LIBS})
# else()
#     message(FATAL_ERROR "OpenCV not found, but is required for ct_vision module")
# endif()
#
# add_module(vision
#     SOURCES ${SOURCES}
#     HEADERS ${HEADERS}
#     DEPENDENCIES ${VISION_DEPS}
# )
#
