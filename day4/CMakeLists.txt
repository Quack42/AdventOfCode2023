project(Day4
    LANGUAGES
        CXX
)
cmake_minimum_required(VERSION 3.22)

add_executable(day4
    main.cpp
)

target_link_libraries(day4
    PRIVATE
        AoC_Utils
)
