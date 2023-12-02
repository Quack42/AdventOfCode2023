project(AoC_Utils
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_library(AoC_Utils
    src/StringUtils.cpp
)

target_include_directories(AoC_Utils
    PUBLIC
        include
)

