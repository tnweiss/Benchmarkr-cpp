//
// Created by tnwei on 2/7/2022.
//

//////////////////////////// Linux /////////////////////////////

#ifdef unix

#include "benchmarkr-common/system.h"

#include "unistd.h"
#include "sys/ioctl.h"
#include "sys/sysinfo.h"

static const char* MEMORY_FREQ_COMMAND = R"(dmidecode | grep "Configured Memory Speed: " | grep "MT/s")";
static const char* MEMORY_FREQ_REGEX = "Configured Memory Speed: ([0-9]+) MT/s";

std::string benchmarkr::hostname() {
  // open /etc/hostname
  std::ifstream host_file;
  host_file.open("/etc/hostname", std::ifstream::in);

  // read the 1 line
  std::string line;
  getline(host_file, line);

  // close the file handle
  host_file.close();

  // return the data
  return line;
}

int benchmarkr::get_window_width() {
  winsize w{};
  ioctl(STDERR_FILENO, TIOCGWINSZ, &w);
  if (w.ws_col > MIN_CONSOLE_WIDTH) {
    return w.ws_col;
  }
  return MIN_CONSOLE_WIDTH;
}

unsigned int benchmarkr::cpu_frequency() {
  std::ifstream proc ("/sys/devices/system/cpu/cpu0/cpufreq/base_frequency");
  std::string freq_str;
  getline(proc, freq_str);
  proc.close();
  return std::stoi(freq_str) / 1000;
}

unsigned long long benchmarkr::memory_installed() {
  struct sysinfo s{};
  sysinfo(&s);
  return s.totalram / 1000;
}

unsigned long long benchmarkr::memory_frequency() {
  require_root("Root privileges are required to calculate memory frequency");

  // run the command to get the memory frequency
  std::string result = run(MEMORY_FREQ_COMMAND);

  // regex search objects
  const std::regex _regex(MEMORY_FREQ_REGEX);
  std::smatch match;

  // holds totals for average
  unsigned long long memory_frequency_total = 0;
  int total = 0;

  // while we continue to find memory matches
  while(std::regex_search(result, match, _regex) ) {
    // add the total
    memory_frequency_total += std::stoi(match[1]);
    total += 1;

    // move the result before
    result = match.suffix();
  }

  if (total == 0) {
    spdlog::error("No memory frequency data available");
    exit(1);
  }

  return memory_frequency_total / total;
}

std::string benchmarkr::home_directory() {
  // TODO get caller id
  std::string home_dir = std::getenv("HOME");

  if (home_dir.empty()) {
    spdlog::error("Unable to find home directory env var {HOME}");
  }

  return home_dir;
}

/**
 * Solution pulled from ...
 *   https://stackoverflow.com/questions/3214297/how-can-my-c-c-application-determine-if-the-root-user-is-executing-the-command
 * @return
 */
bool benchmarkr::is_root() {
  spdlog::error(getuid());
  return getuid() == 0;
//  auto me = getuid();
//  auto myprivs = geteuid();
//  return me != myprivs;
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

  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

#endif
