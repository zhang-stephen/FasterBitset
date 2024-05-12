
cmake_minimum_required(VERSION 3.27)

include(cmake/options.cmake)
include(cmake/global.cmake)

project(FasterBitset
    LANGUAGES CXX
)

include(cmake/lib/compiler.cmake)
include(CTest)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

add_subdirectory(test/ut)
