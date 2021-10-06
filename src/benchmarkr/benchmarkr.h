//
// Created by Tyler on 10/4/2021.
//

#include <iostream>
#include <vector>

#ifndef SIPHON_SRC_TEST_BENCHMARKR_BENCHMARKR_H_
#define SIPHON_SRC_TEST_BENCHMARKR_BENCHMARKR_H_

/**
 * Test interface
 */
struct Test {
  /**
   * The lower bound for a test, significant improvements have been made
   * if this lower bound is met
   * @return the lower bound in ms
   */
  virtual long lower_bound_ms() = 0;

  /**
   * The max duration allowed for a test
   * @return the upper bound in ms
   */
  virtual long upper_bound_ms() = 0;

  /**
   * The test description
   * @return
   */
  virtual const char* description() = 0;

  /**
   * the test to execute
   */
  virtual void execute() = 0;
};

/**
 * Type definition for a function
 */
using func_t = Test*(*)();

/**
 * The list of registered benchmark tests
 * @return
 */
std::vector<func_t>& registered_bm_tests();

/**
 * Register a benchmark test
 */
void register_bm_test(func_t);

/**
 * Get the remote benchmark url to post results to
 * @return
 */
std::string& get_remote_benchmarkr();

/**
 * Set the remote url
 */
void set_remote_bemcharkr(const char*);

/**
 * Macro, can only be used once, that sets the remote url to upload results to
 */
#define CONFIGURE_REMOTE_URL( v )                             \
  struct RemoteBenchmarkr {                                   \
    RemoteBenchmarkr() {                                      \
      set_remote_benchmarkr( v );                             \
    };                                                        \
  };                                                          \
  RemoteBenchmarkr rm;

/**
 * Macro that returns the appropriate macro name based on the number of args
 */
#define VA_NARGS_IMPL(_1, _2, _3, _4, N, ...) BENCHMARK##N
#define VA_NARGS(...)  VA_NARGS_IMPL(__VA_ARGS__, 4, 3, 2, 1)

/**
 * The Benchmark Internal macro used to register a test
 */
#define BENCHMARK_INTERNAL(test, lb, ub, d)                                   \
  struct test : public Test {                                                 \
    public:                                                                   \
    long lower_bound_ms(){                                                    \
      return lb;                                                              \
    };                                                                        \
    long upper_bound_ms(){                                                    \
      return ub;                                                              \
    };                                                                        \
    const char* description() {                                               \
      return d;                                                               \
    };                                                                        \
    void execute();                                                           \
    static Test* get_instance_##test() {                                      \
      return new test(false);                                                 \
    };                                                                        \
    test (bool register_test) {                                               \
      if (register_test) {                                                    \
            register_bm_test( test::get_instance_##test );                    \
      }                                                                       \
    };                                                                        \
  };                                                                          \
  test var_##test(true);                                                      \
  void test::execute()


/**
 * MACROS that provide defaults for the internal call and suppresses linting
 * warnings
 */
#define BENCHMARK4( test, lower_bound, upper_bound, description ) \
  BENCHMARK_INTERNAL( test, lower_bound, upper_bound, description ) // NOLINT

#define BENCHMARK3( test, lower_bound, upper_bound ) \
  BENCHMARK_INTERNAL( test, lower_bound, upper_bound, "" ) // NOLINT

#define BENCHMARK2( test, description ) \
  BENCHMARK_INTERNAL( test, 0, 0, description ) // NOLINT

#define BENCHMARK1( test ) \
  BENCHMARK_INTERNAL( test, 0, 0, "" ) // NOLINT

/**
 * Main Benchmark macro used to register a test
 */
#define BENCHMARK(...) VA_NARGS(__VA_ARGS__)( __VA_ARGS__ )

#endif // SIPHON_SRC_TEST_BENCHMARKR_BENCHMARKR_H_
