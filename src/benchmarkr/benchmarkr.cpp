//
// Created by Tyler on 10/4/2021.
//

#include "benchmarkr.h"


std::vector<func_t>& registered_bm_tests() {
  static std::vector<func_t> bm_tests;
  return bm_tests;
}

void register_bm_test( func_t f) {
    registered_bm_tests().push_back(f);
}

std::string& get_remote_benchmarkr() {
  static std::string remote_benchmarkr;
  return remote_benchmarkr;
}

void set_remote_bemcharkr(const char* new_val) {
  get_remote_benchmarkr().erase();
  get_remote_benchmarkr().append(new_val);
}

#ifndef BENCHMARKR_NO_MAIN
int main() {
  std::vector<std::unique_ptr<Test>> tests;
  tests.reserve(10);

  for (func_t& f: registered_bm_tests()) {
    tests.emplace_back(f());
  }

  for (std::unique_ptr<Test>& test: tests) {
    test->execute();
  }
}
#endif
