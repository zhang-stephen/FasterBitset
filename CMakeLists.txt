# FasterBitset building(arch64) building with C++20
# Copyright (c) Stephen Zhang 2024. All Right Reserved

cmake_minimum_required(VERSION 3.27)

# global settings
include(cmake/options.cmake)
include(cmake/global.cmake)

project(FasterBitset
    LANGUAGES CXX
)

# project settings
include(cmake/lib/compiler.cmake)
include(CTest)

# dependencies from vcpkg
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

# subdirectories
add_subdirectory(src)
add_subdirectory(test/ut)
