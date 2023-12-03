project(Day3
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day3
    main.cpp
)

target_link_libraries(day3
    PRIVATE
        AoC_Utils
)
