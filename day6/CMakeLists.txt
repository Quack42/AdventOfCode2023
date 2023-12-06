project(Day6
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day6
    main.cpp
)

target_link_libraries(day6
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day6 PROPERTY CXX_STANDARD 20)
