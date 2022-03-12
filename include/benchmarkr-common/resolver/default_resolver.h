//
// Created by tyler on 2/23/22.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_RESOLVER_DEFAULT_RESOLVER_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_RESOLVER_DEFAULT_RESOLVER_H_

#include <map>

#include "resolver.h"

namespace benchmarkr {

class DefaultResolver: public Resolver {
 public:
  std::string resolve(const std::string &) override;

  /**
   * Default constructor
   */
  DefaultResolver();

  /**
   * Set the new default
   * @param key
   * @param value
   */
  void set_default(const std::string& key, const std::string& value);
 private:
  std::map<std::string, std::string> _defaults;
};
}

#endif //BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_RESOLVER_DEFAULT_RESOLVER_H_
