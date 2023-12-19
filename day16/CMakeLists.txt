project(Day16
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day16
    main.cpp
)

target_link_libraries(day16
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day16 PROPERTY CXX_STANDARD 20)
