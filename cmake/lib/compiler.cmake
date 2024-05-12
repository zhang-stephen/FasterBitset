# compiler configurations
# Copyright (c) Stephen Zhang 2024. All Right Reserved

add_library(compiler INTERFACE)

target_include_directories(compiler
    INTERFACE
        ${CMAKE_BINARY_DIR}
)

target_compile_options(compiler
    INTERFACE
        -Wall
        -Werror
        -Wshadow
        -Wformat=2
        -Wunused
)

target_compile_definitions(compiler
    INTERFACE
         $<IF:$<CONFIG:Debug>,DEBUG,NDEBUG>
)
