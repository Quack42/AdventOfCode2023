project(Day9
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day9
    main.cpp
)

target_link_libraries(day9
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day9 PROPERTY CXX_STANDARD 20)
