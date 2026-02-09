cmake_minimum_required(VERSION 4.2.0)

project(toolbox VERSION 0.0.1 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(namespace toolbox CACHE STRING "Namespace for the project ")

include(COptions)
include(Dependencies)
include(Module)

message(STATUS "  Version:              ${PROJECT_VERSION}")
message(STATUS "  Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler:             ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  C++ standard:         C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Install prefix:       ${CMAKE_INSTALL_PREFIX}")

add_subdirectory(3rdparty)

set(MODULES_DIR "${CMAKE_SOURCE_DIR}/modules")
file(GLOB CHILD_FOLDERS RELATIVE "${MODULES_DIR}" "${MODULES_DIR}/*")

foreach(FOLDER ${CHILD_FOLDERS})
    if(IS_DIRECTORY "${MODULES_DIR}/${FOLDER}")
        message(STATUS "Module ../${FOLDER}")
        add_subdirectory("${MODULES_DIR}/${FOLDER}")
    endif()
endforeach()



add_subdirectory(studio)
