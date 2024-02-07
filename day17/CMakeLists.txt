project(Day17
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day17
    main.cpp
)

target_link_libraries(day17
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day17 PROPERTY CXX_STANDARD 20)
