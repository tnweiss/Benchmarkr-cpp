//
// Created by tnwei on 1/21/2022.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_
#define BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_

#include "command.h"

namespace benchmarkr {

class Init: public benchmarkr::Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] const char* help() const final;
};

}


#endif //BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_
