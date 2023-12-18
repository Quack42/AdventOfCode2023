project(Day15
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day15
    main.cpp
)

target_link_libraries(day15
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day15 PROPERTY CXX_STANDARD 20)
