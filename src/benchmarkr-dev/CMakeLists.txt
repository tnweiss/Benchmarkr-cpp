########## Benchmarkr Static Library ##########
add_library(benchmarkrDev
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
add_library(benchmarkrDev::benchmarkrDev ALIAS benchmarkrDev)

# link to dependencies
target_link_libraries(benchmarkrDev ${BM_DEV_LL} PRIVATE benchmarkrCommon)

# add precompiled headers
target_precompile_headers(benchmarkrDev PUBLIC benchmarkr_dev_pch.h)
