//
// Created by tyler on 10/9/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_TEST_FILTER_H_
#define BENCHMARKR_SRC_BENCHMARKR_TEST_FILTER_H_

#include "test.h"

namespace benchmarkr {

class TestFilter {
 public:
  /**
 * Instantiate the filter with the provided args
 * @param args
 */
  explicit TestFilter(std::string filters);

  /**
   *
   * @param test
   * @return
   */
  bool passes(Test* test) const;
 private:
  std::vector<std::string> _filters;
};

}


#endif //BENCHMARKR_SRC_BENCHMARKR_TEST_FILTER_H_
