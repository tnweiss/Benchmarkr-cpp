//
// Created by tnwei on 3/8/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_WATCH_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_WATCH_H_

#include "nlohmann/json.hpp"

#include "command.h"
#include "variable_resolver.h"

namespace benchmarkr {

/**
 * Execute the upload watch
 * @param resolver
 */
[[noreturn]] void executeUploadWatch(const benchmarkr::CommandVariableResolver& resolver);

/**
 * Continuously watch the benchmarkr dir for new test results and upload them the remote elastic server
 * @param file_path
 */
class UploadWatch: public Command {
 public:
  void execute(int argc, char* argv[]) const final;
  [[nodiscard]] std::string help() const final;
};

}

#endif //BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_WATCH_H_
