//
// Created by tyler on 11/1/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_RESOLVERS_ENVRESOLVER_H_
#define BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_RESOLVERS_ENVRESOLVER_H_

#include "resolver.h"

namespace benchmarkr {

/**
 * Resolve variables stored as an environment variable
 */
class EnvironmentResolver : public Resolver {
 public:
  std::string resolve(const std::string &) final;
};

}
#endif //BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_RESOLVERS_ENVRESOLVER_H_
