//
// Created by tnwei on 3/8/2022.
//

#include "benchmarkr/command.h"

void benchmarkr::set_log_level(const std::string& log_level) {
  if (log_level == "disabled") {
    spdlog::set_level(spdlog::level::off);
  } else if (log_level == "debug") {
    spdlog::set_level(spdlog::level::debug);
  } else if (log_level == "info") {
    spdlog::set_level(spdlog::level::info);
  } else {
    std::cerr << std::endl <<"Unknown log level " << log_level << std::endl;
    exit(1);
  }
}