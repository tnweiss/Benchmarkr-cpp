//
// Created by tnwei on 2/7/2022.
//

//////////////////////////// Windows /////////////////////////////

#if  defined(WIN64) || defined(WIN32)
#include "benchmarkr-common/system.h"

#include <Windows.h>

/**
 * Solution pulled from
 *   https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
 * @return
 */
int benchmarkr::get_window_width() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

  int window_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

  if (window_width < MIN_CONSOLE_WIDTH) {
    return MIN_CONSOLE_WIDTH;
  }
  return window_width;
}

/**
 * Solution pulled from
 *   https://www.codeproject.com/Articles/7340/Get-the-Processor-Speed-in-two-simple-ways
 * @return
 */
unsigned int benchmarkr::cpu_frequency() {
  DWORD BufSize = _MAX_PATH;
  DWORD dwMHz = _MAX_PATH;
  HKEY hKey;

  RegOpenKeyEx(HKEY_LOCAL_MACHINE,
               R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)",
               0,
               KEY_READ,
               &hKey);
  RegQueryValueEx(hKey, "~MHz", nullptr, nullptr, (LPBYTE) &dwMHz, &BufSize);
  return dwMHz;
}

unsigned long long benchmarkr::memory_installed() {
  unsigned long long total_memory_kilobytes = 0;
  GetPhysicallyInstalledSystemMemory(&total_memory_kilobytes);
  return total_memory_kilobytes;
}

unsigned long long benchmarkr::memory_frequency() {
  return 0;
}

std::string benchmarkr::hostname() {
  return {std::getenv("COMPUTERNAME")};
}

std::string benchmarkr::home_directory() {
  std::string home_dir = std::string(std::getenv("HOMEDRIVE")) + std::getenv("HOMEPATH");

  if (home_dir.empty()) {
    spdlog::error("Unable to find home directory env var HOMEDRIVE");
  }

  return home_dir;
}

/**
 * Solution copied from ...
 * https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
 * @param command
 * @return
 */
std::string benchmarkr::run(const char* command) {
  std::array<char, 128> buffer{};
  std::string result;

  std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command, "r"), _pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

/**
 * Solution pulled from ...
 *   https://stackoverflow.com/questions/3214297/how-can-my-c-c-application-determine-if-the-root-user-is-executing-the-command
 * @return
 */
bool benchmarkr::is_root() {
  // TODO fixme
  return false;
}

#endif
