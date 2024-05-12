# UT building configurations for cmake project
# Copyright (c) Stephen Zhang 2024. All Right Reserved


add_library(ut_entry STATIC UtMain.cc)

target_link_libraries(ut_entry
    PUBLIC
        GTest::gtest
        GTest::gmock
        compiler
        bitset
)
