//
// Created by tyler on 2/23/22.
//

#include "benchmarkr-common/resolver/default_resolver.h"

benchmarkr::DefaultResolver::DefaultResolver(): _defaults() {

}

std::string benchmarkr::DefaultResolver::resolve(const std::string &key) {
  if (_defaults.find(key) == _defaults.end()) {
    return "";
  }

  spdlog::debug("Resolved Default Variable '{}' as '{}'", key, _defaults.at(key));
  return _defaults.at(key);
}

void benchmarkr::DefaultResolver::set_default(const std::string& key, const std::string& value) {
  spdlog::debug("Setting Default Variable '{}' to '{}'", key, value);
  _defaults.insert_or_assign(key, value);
}
