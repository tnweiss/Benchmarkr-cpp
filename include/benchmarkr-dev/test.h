//
// Created by tyler on 10/8/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_TEST_H_
#define BENCHMARKR_SRC_BENCHMARKR_TEST_H_

#include "nlohmann/json.hpp"

#include "result.h"

// generic test property
#define prop_( k, v ) { k , v }

#define bm_SECONDS( x )      static_cast<unsigned long long>(x * 1000000)
#define bm_MILLISECONDS( x ) static_cast<unsigned long long>(x * 1000)
#define bm_MICROSECONDS( x ) static_cast<unsigned long long>(x)

#define bm_prop_LowerBoundKey "LowerBound"
#define prop_LowerBound_s( x )  {bm_prop_LowerBoundKey, bm_SECONDS( x)      }
#define prop_LowerBound_ms( x ) {bm_prop_LowerBoundKey, bm_MILLISECONDS( x) }
#define prop_LowerBound_us( x ) {bm_prop_LowerBoundKey, bm_MICROSECONDS( x) }

#define bm_prop_UpperBoundKey "UpperBound"
#define prop_UpperBound_s( x )  {bm_prop_UpperBoundKey, bm_SECONDS( x)      }
#define prop_UpperBound_ms( x ) {bm_prop_UpperBoundKey, bm_MILLISECONDS( x) }
#define prop_UpperBound_us( x ) {bm_prop_UpperBoundKey, bm_MICROSECONDS( x) }

#define bm_prop_DescriptionKey "Description"
static std::string bm_str( const char* v) {
  return {v};
}
#define prop_Description( x ) {bm_prop_DescriptionKey, bm_str(x) }

#define bm_hook_SetupKey "Setup"
#define hook_Setup( x ) { bm_hook_SetupKey , & (x) }

#define bm_hook_TeardownKey "Teardown"
#define hook_Teardown( x ) { bm_hook_TeardownKey , & (x) }

namespace benchmarkr{

/**
 * Hook function signature
 */
using hook_f = void(*)(std::map<std::string, void*>&);

/**
 * Test interface
 */
struct Test {
 public:
  /**
   * Generic Constructor
   */
  explicit Test(std::map<std::string, nlohmann::json> test_data);

  /**
   * Generic Constructor
   */
  Test(std::map<std::string, nlohmann::json> test_data, const std::map<std::string, hook_f>& hooks);

  /**
   * The test name
   * @return
   */
  [[nodiscard]] virtual const char* name() const = 0;

  /**
   * The lower bound for a test, significant improvements have been made
   * if this lower bound is met
   * @return the lower bound in ms
   */
  [[nodiscard]] unsigned long long lower_bound_us() const;

  /**
   * The max duration allowed for a test
   * @return the upper bound in ms
   */
  [[nodiscard]] unsigned long long upper_bound_us() const;

  /**
   * The test description
   * @return
   */
  [[nodiscard]] const std::string& description() const;

  /**
   * Run the test, calling the private execute method
   */
  [[nodiscard]] std::unique_ptr<Result> run() const;
 private:
  /**
   * the test to execute
   */
  virtual void execute(std::map<std::string, void*>& data) const = 0;

  // custom properties
  nlohmann::json _custom_properties;

  // test specific properties
  unsigned long long _lower_bound_us;
  unsigned long long _upper_bound_us;
  std::string _description;

  // lifecycle hooks
  hook_f _setup;
  hook_f _teardown;
};

/**
 * Type definition for a function
 */
using func_t = Test*(*)();

}

#endif //BENCHMARKR_SRC_BENCHMARKR_TEST_H_
