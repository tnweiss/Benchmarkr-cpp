//
// Created by tnwei on 5/25/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_TEST_CONNECTION_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_TEST_CONNECTION_H_

#include "command.h"

namespace benchmarkr {

const char* help_text();

class TestConnection: public Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] std::string help() const final;
};

}

#endif //BENCHMARKR_INCLUDE_BENCHMARKR_TEST_CONNECTION_H_
