####### Project Variables ##########
project(benchmarkr)
cmake_minimum_required (VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set(BENCHMARKR_VERSION 0.0.1)
set(SPDLOG_INSTALL true)

####### OS Vars ##########
include(CMake/Linux.cmake)
include(CMake/Windows.cmake)
include(CMake/Setup.cmake)

####### Dependencies ##########
include(CMake/dependencies.cmake)

########## Include Directories ##########
include_directories(include)
include_directories(dependencies/include)

########## Include the src subdirectory ##########
add_subdirectory(src)
add_subdirectory(test)

########## Libraries ##########
install(TARGETS benchmarkr
        DESTINATION bin)
install(TARGETS benchmarkr-dev cpr::cpr spdlog::spdlog
        DESTINATION lib)
install(DIRECTORY include/benchmarkr-dev
        DESTINATION ${INC_INSTALL_DIR})