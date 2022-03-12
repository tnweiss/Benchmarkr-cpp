//
// Created by tnwei on 1/26/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_TEST_CONTEXT_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_TEST_CONTEXT_H_

#include <vector>
#include <string>
#include <map>

#include "nlohmann/json.hpp"

#include "test.h"

#define BENCHMARKR_VERSION "0.0.1"

namespace benchmarkr {

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
 * Set the version of the software being tested
 */
void set_software_version(const char*);

/**
 * Get the version of the software being tested
 * @return
 */
std::string& get_software_version();

/**
 * Local test context details, reads global test context and merges it with local context information
 *   like number of tests, programming language, timestamp, etc
 */
class TestContext {
 public:
  /**
   * Default Constructor
   */
  TestContext();

  /**
   * Initialize & override the benchamrkr dir
   * @param benchmarkr_dir
   */
  TestContext(std::filesystem::path benchmarkr_dir);

  /**
   * Get the context as a map of keys to values as string
   * @return
   */
  [[nodiscard]] std::map<std::string, std::string> full_context_printable() const;

  /**
   * Get the properties as a json object
   * @return
   */
  [[nodiscard]] nlohmann::json as_json() const;

 private:
  const std::filesystem::path _benchmarkr_dir;

  // local test context properties
  // immutable properties
  const std::string _language;
  const std::string _benchmarkr_version;
  const unsigned long long _registered_tests;
  const std::string _software_version;
  const long long _timestamp;
};

}

#endif //BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_TEST_CONTEXT_H_
