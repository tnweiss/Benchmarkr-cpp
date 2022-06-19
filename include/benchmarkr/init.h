//
// Created by tnwei on 1/21/2022.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_
#define BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_

#include "command.h"
#include "variable_resolver.h"

namespace benchmarkr {

/**
 * Create the Elasticsearch index name
 * @return
 */
std::string create_index_name();

/**
 * Create the Elasticsearch index pattern
 * @return
 */
std::string create_index_pattern_name();

/**
 * Create the dashboard name
 * @return
 */
std::string create_dashboard_name();

/**
 * Execute the init command
 * @param resolver
 */
void executeInit(const CommandVariableResolver& resolver);

class Init: public benchmarkr::Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] std::string help() const final;
};

}


#endif //BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_H_
