//
// Created by tnwei on 3/4/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_INIT_CONTEXT_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_INIT_CONTEXT_H_

#include "command.h"

namespace benchmarkr {

class InitContext: public Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] const char* help() const final;
};

}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_INIT_CONTEXT_H_
