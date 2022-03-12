//
// Created by Tyler on 10/4/2021.
//

#include <vector>

#include "test_context.h"

#ifndef SIPHON_SRC_TEST_BENCHMARKR_BENCHMARKR_H_
#define SIPHON_SRC_TEST_BENCHMARKR_BENCHMARKR_H_

/**
 * Macro, can only be used once, that sets the software version
 */
#define CONFIGURE_SOFTWARE_VERSION( v )                       \
  struct SoftwareVersion {                                    \
    SoftwareVersion() {                                      \
      benchmarkr::set_software_version( v );                             \
    };                                                        \
  };                                                          \
  SoftwareVersion sv; // NOLINT


/**
 * Macro that returns the appropriate macro name based on the number of Args
 */
#define VA_NARGS_IMPL(_1, _2, _3, N, ...) BENCHMARK##N
#define VA_NARGS(...)  VA_NARGS_IMPL(__VA_ARGS__, 3, 2, 1)

/**
 * The Benchmark Internal macro used to register a test
 */
#define UNPACK( ... ) { __VA_ARGS__ }

# define BENCHMARK_INTERNAL( test_name , data , hooks )                                                               \
class test_name : public benchmarkr::Test {                                                                           \
 public:                                                                                                              \
  [[nodiscard]] const char* name() const final {                                                                      \
    return #test_name;                                                                                                \
  };                                                                                                                  \
  void execute(std::map<std::string, void*>&) const final;                                                            \
  static Test* get_instance_##test_name() {                                                                           \
    std::map<std::string, nlohmann::json> d = UNPACK data;                                                            \
    std::map<std::string, benchmarkr::hook_f> h = UNPACK hooks;                                                       \
    return new test_name(d, h);                                                                                       \
  };                                                                                                                  \
  explicit test_name (const std::map<std::string, nlohmann::json>& d,                                                 \
    std::map<std::string, benchmarkr::hook_f> h): benchmarkr::Test(d, h) { };                                         \
  explicit test_name (): benchmarkr::Test({}) {                                                                       \
    benchmarkr::register_bm_test( test_name::get_instance_##test_name );                                              \
  };                                                                                                                  \
};                                                                                                                    \
test_name var_##test_name;                                                                                            \
void test_name::execute(std::map<std::string, void*>& test_data) const

#define BENCHMARK1( test_name ) BENCHMARK_INTERNAL( test_name, () , ())
#define BENCHMARK2( test_name , data ) BENCHMARK_INTERNAL( test_name , data , () )
#define BENCHMARK3( test_name , data , hooks ) BENCHMARK_INTERNAL( test_name , data , hooks )
/**
 * Main Benchmark macro used to register a test
 */
#define BENCHMARK(...) VA_NARGS(__VA_ARGS__)(__VA_ARGS__) // NOLINT


#endif // SIPHON_SRC_TEST_BENCHMARKR_BENCHMARKR_H_
