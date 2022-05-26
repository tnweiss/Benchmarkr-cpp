//
// Created by tnwei on 3/6/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_H_

#include "benchmarkr-common/resolver/resolver.h"
#include "command.h"

#include "nlohmann/json.hpp"

namespace benchmarkr {

/**
 * Upload the results to the remote elastic server
 * @param file_path
 */
std::vector<nlohmann::json> parse_results(const std::string& results_file_path, nlohmann::json& global_ctx);

class Upload: public Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] std::string help() const final;
};

}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_H_
