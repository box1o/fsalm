cmake_minimum_required(VERSION 4.2.0)

project(fslam VERSION 0.0.1 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(namespace fs CACHE STRING "Namespace for the project ")

include(COptions)
include(Dependencies)
include(Module)

message(STATUS "  Version:              ${PROJECT_VERSION}")
message(STATUS "  Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler:             ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  C++ standard:         C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Install prefix:       ${CMAKE_INSTALL_PREFIX}")

add_subdirectory(3rdparty)

add_subdirectory(fslam)
add_subdirectory(studio)

