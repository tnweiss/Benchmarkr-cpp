//
// Created by tnwei on 3/8/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_WATCH_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_WATCH_H_


#include "command.h"

#include "nlohmann/json.hpp"

namespace benchmarkr {

/**
 * Continuously watch the benchmarkr dir for new test results and upload them the remote elastic server
 * @param file_path
 */
class UploadWatch: public Command {
 public:
  [[noreturn]] void execute(int argc, char* argv[]) const final;
  [[nodiscard]] const char* help() const final;
};

}

#endif //BENCHMARKR_INCLUDE_BENCHMARKR_UPLOAD_WATCH_H_
