//
// Created by tnwei on 5/24/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_PRINT_VERSION_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_PRINT_VERSION_H_

#include "command.h"

namespace benchmarkr {

class PrintVersion: public benchmarkr::Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] const char* help() const final;
};

}

#endif //BENCHMARKR_INCLUDE_BENCHMARKR_PRINT_VERSION_H_
