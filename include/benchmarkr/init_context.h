//
// Created by tnwei on 3/4/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_INIT_CONTEXT_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_INIT_CONTEXT_H_

#include "command.h"

namespace benchmarkr {

/**
 * Execute the initialize context command
 */
void executeInitContext(const CommandVariableResolver&);

class InitContext: public Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] std::string help() const final;
};

}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_INIT_CONTEXT_H_
