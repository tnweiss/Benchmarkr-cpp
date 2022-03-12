//
// Created by tyler on 10/18/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_CONSOLE_SILENT_CONSOLE_H_
#define BENCHMARKR_SRC_BENCHMARKR_CONSOLE_SILENT_CONSOLE_H_

#include "console.h"

namespace benchmarkr {

class SilentConsole: public Console {
 public:
  void print_headers(const std::map<std::string, std::string> &) final;
  void print_test(const std::string &, unsigned long, unsigned long) final;
  void print_test_result(unsigned long, bool) final;
  void print_failures(std::vector<const benchmarkr::Result *> &) final;
  void print_significant_success(std::vector<const benchmarkr::Result *> &) final;
  void print_summary(unsigned int, unsigned int, unsigned int, unsigned int) final;
};

}


#endif //BENCHMARKR_SRC_BENCHMARKR_CONSOLE_SILENT_CONSOLE_H_
