//
// Created by tyler on 2/5/22.
//

#include "catch.hpp"

#include "benchmarkr-dev/test_context.h"


TEST_CASE("TestTestContext") {
  benchmarkr::set_software_version("1.2.3");

  benchmarkr::TestContext test_context;

  // map used to pretty print values to the console
  std::map<std::string, std::string> tctx_sm = test_context.full_context_printable();

  // json used for elastic
  nlohmann::json tctx_js = test_context.as_json();

  REQUIRE(!tctx_sm.at("Timestamp").empty());
  REQUIRE(!tctx_js.at("Timestamp").empty());
}


TEST_CASE("TestSoftwareVersion") {
  const char* SW_VER = "0.0.1";
  benchmarkr::set_software_version(SW_VER);
  REQUIRE(benchmarkr::get_software_version() == SW_VER);
}
