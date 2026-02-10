cmake_minimum_required(VERSION 4.2.0)

project(base
    VERSION 1.0.0
    DESCRIPTION "Core functionality: types, logging, error handling"
    LANGUAGES CXX
)

file(GLOB_RECURSE HEADERS
    include/*.hpp
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

set(BASE_DEPS )

if (NOT EMSCRIPTEN)
    list(APPEND BASE_DEPS fmt spdlog)
else()
    message(STATUS "Skipping fmt and spdlog for web build (using Emscripten's libc++)")
endif()


add_module(base
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDENCIES ${BASE_DEPS}
)

