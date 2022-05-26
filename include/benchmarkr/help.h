//
// Created by tyler on 10/20/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_COMMANDS_HELP_H_
#define BENCHMARKR_SRC_BENCHMARKR_COMMANDS_HELP_H_

#include "command.h"

namespace benchmarkr {

class Help: public Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] std::string help() const final;
};

}


#endif //BENCHMARKR_SRC_BENCHMARKR_COMMANDS_HELP_H_
