####### Project Variables ##########
project(benchmarkr)
cmake_minimum_required (VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )


####### OS Vars ##########
include(CMake/Linux.cmake)
include(CMake/Windows.cmake)


####### Dependencies ##########
link_directories(dependencies/lib)
include_directories(dependencies/include)


########## Include the src subdirectory ##########
add_subdirectory(src)