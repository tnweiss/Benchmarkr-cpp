//
// Created by tnwei on 3/4/2022.
//

#include "catch.hpp"

#include "benchmarkr-dev/test_filter.h"


/**
 * Test class
 */
class TFTest : public benchmarkr::Test {
 public:
  [[nodiscard]] const char* name() const final {
    return _name.c_str();
  };
  void execute(std::map<std::string, void*>& test_data) const override {};
  explicit TFTest (std::string name): benchmarkr::Test({}), _name(std::move(name)) {};
 private:
  const std::string _name;
};



TEST_CASE("TestTestFilter") {
  const char* TEST_FILTER = "test_a,test_b";

  TFTest test_a("test_a");
  TFTest test_b("test_b");
  TFTest test_c("test_c");

  benchmarkr::TestFilter test_filter(TEST_FILTER);

  REQUIRE(test_filter.passes(&test_a));
  REQUIRE(test_filter.passes(&test_b));
  REQUIRE(!test_filter.passes(&test_c));
}

