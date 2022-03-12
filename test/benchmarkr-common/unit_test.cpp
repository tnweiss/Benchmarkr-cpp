//
// Created by tyler on 2/4/22.
//

#include "unit_test.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TestArgs::TestArgs(std::vector<std::string> arguments):
    _argv(new char*[arguments.size() + 1]),
    _argc(static_cast<int>(arguments.size()) + 1)
{
  // reserve first two for dummy data
  _argv[0] = new char[1]();
  _argv[0][0] = '\0';

  int i = 1;
  for (std::string& argument: arguments) {
    _argv[i] = new char[argument.size() + 1];
    strcpy(_argv[i], argument.data());
    i ++;
  }
}

TestArgs::~TestArgs() {
  for (int i=0; i<_argc; i++) {
    delete _argv[i];
  }

  delete[] _argv;
}

char **TestArgs::argv() const {
  return _argv;
}

int TestArgs::argc() const {
  return _argc;
}
