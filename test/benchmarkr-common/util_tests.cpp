//
// Created by tyler on 2/28/22.
//

#include "catch.hpp"

#include "benchmarkr-common/util.h"

TEST_CASE("TestDemangle") {
  REQUIRE(benchmarkr::demangle(typeid(long).name()) == "long");
  REQUIRE(benchmarkr::demangle(typeid(int).name()) == "int");
  REQUIRE(benchmarkr::demangle(typeid(double).name()) == "double");
  REQUIRE(benchmarkr::demangle(typeid(float).name()) == "float");
}

TEST_CASE("TestToStrPrecision") {
  REQUIRE(benchmarkr::to_str_precision(4.1234, 1) == "4.1");
  REQUIRE(benchmarkr::to_str_precision(4.1234, 2) == "4.12");
  REQUIRE(benchmarkr::to_str_precision(4.1234, 3) == "4.123");
}

TEST_CASE("TestIdTime") {
  REQUIRE(benchmarkr::id_time(1646087321586) == "20220228142845860");
}

TEST_CASE("TestEpochMillis") {
  REQUIRE(benchmarkr::epoch_millis() > 0);
}

TEST_CASE("TestUUID") {
  REQUIRE(benchmarkr::uuid().size() == 36);
}

TEST_CASE("TestPrettyTime") {
  REQUIRE(benchmarkr::pretty_time(1646087321586) == "Mon Feb 28 14:28:41 2022");
}
