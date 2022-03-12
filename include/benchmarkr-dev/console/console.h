//
// Created by tyler on 10/17/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_CONSOLE_CONSOLE_H_
#define BENCHMARKR_SRC_BENCHMARKR_CONSOLE_CONSOLE_H_

#include <string>
#include <map>
#include <vector>

#include "../result.h"

namespace benchmarkr {

class Console {
 public:
  /**
   * Print the initial benchmark test header with current machine stats + column headers
   */
  virtual void print_headers(const std::map<std::string, std::string> &) = 0;

  /**
   * prints the test name, upper bound, and lower bound for the current test
   */
  virtual void print_test(const std::string &, unsigned long, unsigned long) = 0;

  /**
   * prints the duration in green if it passed and in red if it did not.
   *   If duration is 0 it is assumed an exception occurred
   */
  virtual void print_test_result(unsigned long, bool) = 0;

  /**
   * print more detailed descriptions of failures
   */
  virtual void print_failures(std::vector<const benchmarkr::Result *> &) = 0;

  /**
   * print significant success
   */
  virtual void print_significant_success(std::vector<const benchmarkr::Result *> &) = 0;

  /**
   * print the test summary
   */
  virtual void print_summary(unsigned int, unsigned int, unsigned int, unsigned int) = 0;
};

}
#endif //BENCHMARKR_SRC_BENCHMARKR_CONSOLE_CONSOLE_H_
