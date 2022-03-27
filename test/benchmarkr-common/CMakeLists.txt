###### Unit Tests ######
add_executable(benchmarkr_common_unit_tests

        unit_test.cpp

        util_tests.cpp
        system_test.cpp
        resolver_tests.cpp
)
add_executable(benchmarkr_common_root_unit_tests

        unit_test.cpp

        system_root_test.cpp
        global_test_context_root_test.cpp
)

# precompile benchmarkr-common headers
target_precompile_headers(benchmarkr_common_unit_tests PRIVATE benchmarkr_common_tests_pch.h)
target_precompile_headers(benchmarkr_common_root_unit_tests PRIVATE benchmarkr_common_tests_pch.h)

# link with the benchmarkr_dev library
target_link_libraries(benchmarkr_common_unit_tests PUBLIC benchmarkrCommon)
target_link_libraries(benchmarkr_common_root_unit_tests PUBLIC benchmarkrCommon)
