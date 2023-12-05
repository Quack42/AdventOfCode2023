project(Day5
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day5
    main.cpp
)

target_link_libraries(day5
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day5 PROPERTY CXX_STANDARD 20)
