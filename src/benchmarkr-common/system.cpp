//
// Created by Tyler on 10/7/2021.
//

#include "benchmarkr-common/system.h"

//////////////////////////// Platform Independent /////////////////////////////
#include <thread>
#include <iostream>

unsigned int benchmarkr::cpu_threads(){
  return std::thread::hardware_concurrency();
}

const char* benchmarkr::os_name()
{
#ifdef _WIN32
  return "Windows 32-bit";
#elif _WIN64
  return "Windows 64-bit";
#elif __APPLE__ || __MACH__
  return "Mac OSX";
#elif __linux__
  return "Linux";
#elif __FreeBSD__
  return "FreeBSD";
#elif __unix || __unix__
  return "Unix";
#else
  return "Other";
#endif
}

void benchmarkr::require_root(const std::string& action) {
  if (!is_root()) {
    std::cerr << "The current user must be root in order to " << action;
    throw std::exception();
  }
}
