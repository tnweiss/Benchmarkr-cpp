########## Benchmarkr Static Library ##########
add_library(benchmarkr-dev
    STATIC
        test.cpp
        result.cpp
        results.cpp
        benchmarkr.cpp
        test_filter.cpp
        test_context.cpp
        significant_results.cpp

        console/silent_console.cpp
        console/simple_console.cpp
        console/verbose_console.cpp
)

# create the alias
add_library(benchmarkr-dev::benchmarkr-dev ALIAS benchmarkr-dev)

# link to dependencies
target_link_libraries(benchmarkr-dev ${BM_DEV_LL} PRIVATE benchmarkr-common)

# add precompiled headers
target_precompile_headers(benchmarkr-dev PUBLIC benchmarkr_dev_pch.h)
