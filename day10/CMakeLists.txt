project(Day10
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day10
    main.cpp
)

target_link_libraries(day10
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day10 PROPERTY CXX_STANDARD 20)
