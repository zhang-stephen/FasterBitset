# Global Environment configuration, must be included befor the first `project()` command
# Copyright (c) Stephen Zhang 2024. All rights reserved.

# Toolchain - vcpkg default
set(CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE
    STRING "vcpkg toolchain file")


# export compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# default building type is Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" STRING)
endif ()

# c/c++ standard and features
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if (CMAKE_HOST_APPLE)
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@16/bin/clang++")
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@16/bin/clang")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif ()
