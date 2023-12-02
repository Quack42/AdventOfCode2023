project(Day2
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day2
    main.cpp
)

target_link_libraries(day2
    PRIVATE
        AoC_Utils
)
