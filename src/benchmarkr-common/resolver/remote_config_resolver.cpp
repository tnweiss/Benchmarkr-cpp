//
// Created by tyler on 2/28/22.
//

#include "benchmarkr-common/resolver/remote_config_resolver.h"

#include "benchmarkr-common/resolver/remote_config.h"

std::string benchmarkr::RemoteConfigResolver::resolve(const std::string & key) {
  std::map<std::string, std::string>& data = get_remote_files();

  if (data.find(key) == data.end()) {
    return "";
  }

  return data.at(key);
}
