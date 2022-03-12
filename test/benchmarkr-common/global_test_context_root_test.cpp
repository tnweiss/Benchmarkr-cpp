//
// Created by tyler on 2/28/22.
//

#include "catch.hpp"

#include "benchmarkr-common/system.h"
#include "benchmarkr-common/global_test_context.h"

TEST_CASE("TestGlobalContext") {
  // load global context, and initialize, then write to file
  benchmarkr::GlobalTestContext global_context;
  global_context.init();
  global_context.writeToFile();

  // load the context from the file
  const benchmarkr::GlobalTestContext global_context_2;

  REQUIRE(benchmarkr::memory_installed() == global_context.memory_installed());
  REQUIRE(global_context.memory_installed() == global_context_2.memory_installed());

  REQUIRE(global_context.memory_frequency() != 0);
  REQUIRE(benchmarkr::memory_frequency() == global_context.memory_frequency());
  REQUIRE(global_context.memory_frequency() == global_context_2.memory_frequency());

  REQUIRE(benchmarkr::cpu_frequency() == global_context.cpu_frequency());
  REQUIRE(global_context.cpu_frequency() == global_context_2.cpu_frequency());

  REQUIRE(benchmarkr::cpu_threads() == global_context.cpu_threads());
  REQUIRE(global_context.cpu_threads() == global_context_2.cpu_threads());

  REQUIRE(benchmarkr::os_name() == global_context.os());
  REQUIRE(global_context.os() == global_context_2.os());

  REQUIRE(benchmarkr::hostname() == global_context.hostname());
  REQUIRE(global_context.hostname() == global_context_2.hostname());
}
