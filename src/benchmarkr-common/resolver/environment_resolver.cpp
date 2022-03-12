//
// Created by tyler on 11/1/21.
//

#include "benchmarkr-common/resolver/environment_resolver.h"

std::string benchmarkr::EnvironmentResolver::resolve(const std::string &key) {
  char* value = std::getenv(key.c_str());

  if (value == nullptr) {
    return "";
  }

  spdlog::debug("Resolved Environment Variable '{}' as '{}'", key, value);
  return value;
}
