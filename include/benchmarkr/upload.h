//
// Created by tnwei on 3/6/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_H_

#include "nlohmann/json.hpp"

#include "variable_resolver.h"
#include "command.h"

namespace benchmarkr {

/**
 * Upload the results to the remote elastic server
 * @param file_path
 */
std::vector<nlohmann::json> parse_results(const std::string& results_file_path, nlohmann::json& global_ctx);

/**
 * Execute the command
 * @param resolver
 */
void executeUpload(const benchmarkr::CommandVariableResolver& resolver);

class Upload: public Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] std::string help() const final;
};

}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_H_
