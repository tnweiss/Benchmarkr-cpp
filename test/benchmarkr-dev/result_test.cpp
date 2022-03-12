//
// Created by tnwei on 3/3/2022.
//

#include "catch.hpp"

#include "benchmarkr-dev/result.h"
#include "benchmarkr-dev/significant_results.h"


TEST_CASE("TestResult") {
  benchmarkr::Result result("Test", 1, 3, "Test Description", {});
  result.set_duration(2);
  result.set_delta(0);
  result.set_outcome("Everything worked");
  result.set_success(true);

  REQUIRE(result.lower_bound() == 1);
  REQUIRE(result.upper_bound() == 3);
  REQUIRE(result.success());
  REQUIRE(result.description() == "Test Description");
  REQUIRE(result.name() == "Test");
  REQUIRE(result.duration_us() == 2);
  REQUIRE(result.delta() == 0);
  REQUIRE(result.outcome() == "Everything worked");
}

TEST_CASE("TestSignificantResults") {
  benchmarkr::SignificantResults results(3);

  std::unique_ptr<benchmarkr::Result> result1 = std::make_unique<benchmarkr::Result>(
      benchmarkr::Result("Result1", 1, 3, "Result 1 Description", {}));
  std::unique_ptr<benchmarkr::Result> result2 = std::make_unique<benchmarkr::Result>(
      benchmarkr::Result("Result2", 4, 6, "Result 2 Description", {}));
  std::unique_ptr<benchmarkr::Result> result3 = std::make_unique<benchmarkr::Result>(
      benchmarkr::Result("Result3", 7, 9, "Result 3 Description", {}));
  std::unique_ptr<benchmarkr::Result> result4 = std::make_unique<benchmarkr::Result>(
      benchmarkr::Result("Result4", 10, 12, "Result 4 Description", {}));

  result1->set_delta(-2.11);
  result2->set_delta(0);
  result3->set_delta(1.111);
  result4->set_delta(1.99);

  results.add(result1.get());
  results.add(result2.get());
  results.add(result3.get());
  results.add(result4.get());

  std::vector<const benchmarkr::Result*> results_sorted = results.as_vector();

  REQUIRE(results_sorted.size() == 3);
  REQUIRE(results_sorted.at(0)->delta() == -2.11);

  for (int i=1; i<results_sorted.size(); i++) {
    REQUIRE(std::abs(results_sorted.at(i)->delta()) < std::abs(results_sorted.at(i-1)->delta()));
  }
}