project(Day11
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day11
    main.cpp
)

target_link_libraries(day11
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day11 PROPERTY CXX_STANDARD 20)
