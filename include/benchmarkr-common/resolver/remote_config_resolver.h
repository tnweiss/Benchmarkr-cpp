//
// Created by tyler on 2/28/22.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_RESOLVER_REMOTE_CONFIG_RESOLVER_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_RESOLVER_REMOTE_CONFIG_RESOLVER_H_

#include "resolver.h"

namespace benchmarkr {

/**
 * Resolve variables pulled from remote configuration
 */
class RemoteConfigResolver: public Resolver {
 public:
  std::string resolve(const std::string &) final;
};

}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_RESOLVER_REMOTE_CONFIG_RESOLVER_H_
