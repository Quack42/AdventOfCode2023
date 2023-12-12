project(Day12
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day12
    main.cpp
)

target_link_libraries(day12
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day12 PROPERTY CXX_STANDARD 20)
