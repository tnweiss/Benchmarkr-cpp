//
// Created by tyler on 2/28/22.
//

#include "catch.hpp"

#include "benchmarkr-common/system.h"

TEST_CASE("TestIsRoot") {
  REQUIRE(benchmarkr::is_root());
}

TEST_CASE("TestRequireRoot") {
  benchmarkr::require_root("Root Required to run this test");
}

TEST_CASE("TestMemoryFrequency") {
  REQUIRE(benchmarkr::memory_frequency() > 0);
}
