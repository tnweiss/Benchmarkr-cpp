//
// Created by tyler on 2/28/22.
//

#include "catch.hpp"

#include "benchmarkr-common/system.h"

TEST_CASE("TestGetWindowWidth") {
  REQUIRE(benchmarkr::get_window_width() > 0);
}

TEST_CASE("TestGetOSName") {
  std::string os = benchmarkr::os_name();
  REQUIRE(!os.empty());
}

TEST_CASE("TestCPUThreads") {
  REQUIRE(benchmarkr::cpu_threads() > 0);
}

TEST_CASE("TestCPUFrequency") {
  REQUIRE(benchmarkr::cpu_frequency() > 0);
}

TEST_CASE("TestMemoryFrequency") {
  REQUIRE(benchmarkr::cpu_frequency() > 0);
}

TEST_CASE("TestIsRoot") {
  REQUIRE(!benchmarkr::is_root());
}

TEST_CASE("TestRequireRoot") {
  REQUIRE_THROWS_AS(benchmarkr::require_root("to test this"), std::exception);
}