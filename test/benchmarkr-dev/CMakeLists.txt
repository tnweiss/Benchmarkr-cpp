###### Unit Tests ######
add_executable(benchmarkr_dev_unit_tests

        unit_test.cpp
        test_test.cpp
        result_test.cpp
        test_context_test.cpp
)
add_executable(benchmarkr_dev_integration_tests

        integration_test.cpp
)

# precompile benchmarkr-common headers
target_precompile_headers(benchmarkr_dev_unit_tests PRIVATE benchmarkr_dev_tests_pch.h)
target_precompile_headers(benchmarkr_dev_integration_tests PRIVATE benchmarkr_dev_integration_test_pch.h)

# link with the benchmarkr_dev library
target_link_libraries(benchmarkr_dev_unit_tests PRIVATE benchmarkrDev)
target_link_libraries(benchmarkr_dev_integration_tests PRIVATE benchmarkrDev)
