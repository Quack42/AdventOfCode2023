project(Day13
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day13
    main.cpp
)

target_link_libraries(day13
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day13 PROPERTY CXX_STANDARD 20)
