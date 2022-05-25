####### Project ##########
cmake_minimum_required (VERSION 3.5)
project(benchmarkr VERSION 0.0.3)
set(BENCHMARKR_COMMON_LIBRARY benchmarkrCommon)
set(BENCHMARKR_DEV_LIBRARY benchmarkrDev)
set(BENCHMARKR_EXECUTABLE benchmarkr)

####### CMAKE VARIABLES ##########
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

####### OS Vars ##########
include(cmake/linux.cmake)
include(cmake/windows.cmake)

####### Remote Configuration ##########
include(cmake/remote_config.cmake)

####### Dependencies ##########
include(cmake/dependencies.cmake)

########## Include Directories ##########
include_directories(include)
include_directories(dependencies/include)

####### Create Version Header ##########
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/benchmarkr-dev/version.h
        @ONLY
)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/benchmarkr-common/version.h
        @ONLY
)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/benchmarkr/version.h
        @ONLY
)

########## Include the src subdirectory ##########
add_subdirectory(src)

# If the project is not a submodule, add the test directory
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    add_subdirectory(test)
endif()

########## Script to Install ##########
include(cmake/install.cmake)
