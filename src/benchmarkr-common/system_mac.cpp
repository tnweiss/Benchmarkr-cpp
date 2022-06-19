//
// Created by tnwei on 6/15/2022.
//

#ifdef __APPLE__
#include "benchmarkr-common/system.h"


std::string benchmarkr::hostname() {
  return "N/A";
}

int benchmarkr::get_window_width() {
  return MIN_CONSOLE_WIDTH;
}

unsigned int benchmarkr::cpu_frequency() {
  return 0;
}

unsigned long long benchmarkr::memory_installed() {
  return 0;
}

unsigned long long benchmarkr::memory_frequency() {
  return 0;
}

std::string benchmarkr::home_directory() {
  std::string home_dir = std::getenv("HOME");
  return home_dir;
}

/**
 * Solution pulled from ...
 *   https://stackoverflow.com/questions/3214297/how-can-my-c-c-application-determine-if-the-root-user-is-executing-the-command
 * @return
 */
bool benchmarkr::is_root() {
  return true;
}

/**
 * Solution copied from ...
 * https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
 * @param command
 * @return
 */
std::string benchmarkr::run(const char* command) {
  return "";
}
#endif