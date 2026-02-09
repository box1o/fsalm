cmake_minimum_required(VERSION 4.2.0)

project(math
    VERSION 1.0.0
    DESCRIPTION "Mathematics and linear algebra module for toolbox"
    LANGUAGES CXX
)

file(GLOB_RECURSE HEADERS
    include/*.hpp
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)


add_module(math
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
)

