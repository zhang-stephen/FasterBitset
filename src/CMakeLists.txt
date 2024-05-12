# implementations of bitset

add_library(bitset INTERFACE)

target_include_directories(bitset
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(bitset
    INTERFACE
        compiler
)
