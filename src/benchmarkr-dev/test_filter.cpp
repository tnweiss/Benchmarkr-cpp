//
// Created by tyler on 10/9/21.
//

#include "benchmarkr-dev/test_filter.h"


bool benchmarkr::TestFilter::passes(benchmarkr::Test *test) const {
  if (_filters.empty()) {
    return true;
  }

  return std::any_of(_filters.begin(), _filters.end(),
                     [test](auto n) { return n == test->name(); });
}

benchmarkr::TestFilter::TestFilter(std::string filters): _filters() {
  if (filters.empty()) return;

  unsigned long long pos;
  do {
    pos = filters.find(',');

    if (pos == std::string::npos) {
      _filters.push_back(filters);
    } else {
      _filters.push_back(filters.substr(0, pos));
      filters = filters.erase(0, pos + 1);
    }

  } while (!filters.empty() && pos != std::string::npos);
}