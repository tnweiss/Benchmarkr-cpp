//
// Created by tyler on 2/28/22.
//

#if defined(__linux__) || defined(__APPLE__)

#include "benchmarkr-common/util.h"

#include "uuid/uuid.h"
#include <cxxabi.h>

static const char* HOME_DIR_KEY = "HOME";

std::string benchmarkr::demangle( const char* mangled_name ) {
  std::size_t len = 0 ;
  int status = 0 ;
  std::unique_ptr< char, decltype(&std::free) > ptr(
      __cxxabiv1::__cxa_demangle( mangled_name, nullptr, &len, &status ), &std::free ) ;
  return ptr.get() ;
}

std::string benchmarkr::uuid() {
  // instantiate uuid var
  uuid_t uuid;

  // generate random uuid
  uuid_generate(uuid);

  // uuid buffer
  char uuid_out[40];

  // convert hex to string
  uuid_unparse(uuid, uuid_out);

  return {uuid_out};
}

void benchmarkr::env_set(const std::string& key, const std::string& val) {
  setenv(key.c_str(), val.c_str(), 1);
}
#endif
