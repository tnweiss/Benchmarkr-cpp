//
// Created by tyler on 2/28/22.
//

#ifdef WIN32

#include "benchmarkr-common/util.h"

std::string benchmarkr::demangle( const char* mangled_name ) {
    return {mangled_name};
}

#include "combaseapi.h"

/**
 * Solutions pulled from
 * https://stackoverflow.com/questions/18555306/convert-guid-structure-to-lpcstr
 * and
 * https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
 * @return
 */
std::string benchmarkr::uuid() {
  // create buffer
  char szGuid[40] = {0};

  // create the GUID
  GUID guid;
  CoCreateGuid(&guid);

  // format the GUID
  sprintf(szGuid, "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
          guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2],
          guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

  return {szGuid};
}

void benchmarkr::env_set(const std::string& key, const std::string& val) {
  _putenv_s(key.c_str(), val.c_str());
}

#endif