//
// Created by tnwei on 5/24/2022.
//
#include <iostream>
#include "benchmarkr/print_version.h"
#include "benchmarkr/version.h"

void benchmarkr::PrintVersion::execute(int argc, char **argv) const {
  std::cout << BENCHMARKR_VERSION << std::endl;
}

const char *benchmarkr::PrintVersion::help() const {
  return "Print the client version";
}

