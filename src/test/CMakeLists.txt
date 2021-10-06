########## Benchmarkr Tests ##########

###### Unit Tests ######
add_executable(benchmarkr_unit_tests benchmarkr_unit_tests.cpp)
target_link_libraries(benchmarkr_unit_tests PUBLIC benchmarkr)

###### Integration Tests ######
add_executable(benchmark_integration_tests benchmark_integration_tests.cpp)
target_link_libraries(benchmark_integration_tests PUBLIC benchmarkr)
