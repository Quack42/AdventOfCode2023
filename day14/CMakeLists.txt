project(Day14
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day14
    main.cpp
)

target_link_libraries(day14
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day14 PROPERTY CXX_STANDARD 20)
