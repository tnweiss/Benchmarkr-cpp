//
// Created by tnwei on 1/21/2022.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_
#define BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_

#include "command.h"

namespace benchmarkr {

std::string create_index_name();
std::string create_index_pattern_name();
std::string create_dashboard_name();

class Init: public benchmarkr::Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] std::string help() const final;
};

}


#endif //BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_
