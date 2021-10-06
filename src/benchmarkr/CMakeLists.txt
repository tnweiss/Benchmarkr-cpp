############## Benchmarkr Library ##############
add_library(benchmarkr STATIC

        benchmarkr.cpp
        benchmarkr.h
        )

target_include_directories(benchmarkr PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
