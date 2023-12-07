project(Day7
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day7
    main.cpp
)

target_link_libraries(day7
    PRIVATE
        AoC_Utils
)

# C++20
set_property(TARGET day7 PROPERTY CXX_STANDARD 20)
