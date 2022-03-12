//
// Created by tnwei on 2/7/2022.
//

#include "benchmarkr-common/global_test_context.h"

#include <utility>

#include "benchmarkr-common/util.h"
#include "benchmarkr-common/system.h"
#include "benchmarkr-common/defaults.h"

static const char* OS_KEY = "OperatingSystem";
static const char* CPU_FREQUENCY_KEY = "CpuFrequency";
static const char* CPU_THREADS_KEY = "CpuThreads";
static const char* MEMORY_INSTALLED_KEY = "MemoryInstalled";
static const char* MEMORY_FREQUENCY_KEY = "MemoryFrequency";
static const char* HOSTNAME_KEY = "Hostname";


benchmarkr::GlobalTestContext::GlobalTestContext(): GlobalTestContext(benchmarkr::DEFAULT_BM_DIR) {

}

benchmarkr::GlobalTestContext::GlobalTestContext(std::filesystem::path benchmarkr_dir):
  _test_data(), _benchmarkr_dir(std::move(benchmarkr_dir)), _filename(benchmarkr::DEFAULT_BM_GLOBAL_CONTEXT_FILE) {

  const std::string full_path = (std::filesystem::path(_benchmarkr_dir) /
      std::filesystem::path(_filename)).string();

  if (std::filesystem::exists(full_path)) {
    // create stream object
    std::ifstream global_context_file;

    // open for reading
    global_context_file.open(full_path, std::ifstream::in);

    std::string line;
    std::string contents;

    while (std::getline(global_context_file, line)) {
      contents.append(line);
    }

    _test_data = nlohmann::json::parse(contents);
  }
}

void benchmarkr::GlobalTestContext::writeToFile() const {
  require_init();
  writeToFile(_benchmarkr_dir.string(), _filename);
}

void benchmarkr::GlobalTestContext::writeToFile(const std::string& benchmarkr_dir, const std::string& filepath) const {
  // root permissions are required
#ifndef WIN32
  benchmarkr::require_root("Root permissions are required to export GlobalTestContext");
#endif
  // create the benchmarkr dir
  std::filesystem::create_directories(benchmarkr_dir);

  // create the full path to the file
  const std::string full_path = (std::filesystem::path(_benchmarkr_dir) /
      std::filesystem::path(_filename)).string();

  // open the file
  std::ofstream test_context_file;
  test_context_file.open(full_path, std::ofstream::out);

  // serialize the test data
  test_context_file << _test_data.dump(2);

  // close the stream
  test_context_file.close();
}

benchmarkr::GlobalTestContext& benchmarkr::GlobalTestContext::init() {
  if (benchmarkr::is_root()) {
    // attributes that require root
    memory_frequency(benchmarkr::memory_frequency());
  } else {
    memory_frequency(0);
  }

  // values that can be attained through non root permissions
  os(benchmarkr::os_name());
  cpu_frequency(benchmarkr::cpu_frequency());
  cpu_threads(benchmarkr::cpu_threads());
  memory_installed(benchmarkr::memory_installed());
  hostname(benchmarkr::hostname());

  return *this;
}

void benchmarkr::GlobalTestContext::os(std::string os) {
  _test_data[OS_KEY] = os;
}

void benchmarkr::GlobalTestContext::cpu_frequency(unsigned short cpu_frequency) {
  _test_data[CPU_FREQUENCY_KEY] = cpu_frequency;
}

unsigned short benchmarkr::GlobalTestContext::cpu_frequency() const {
  require_init();
  return _test_data[CPU_FREQUENCY_KEY].get<unsigned short>();
}

std::string benchmarkr::GlobalTestContext::os() const {
  require_init();
  return _test_data[OS_KEY].get<std::string>();
}

void benchmarkr::GlobalTestContext::cpu_threads(unsigned short cpu_threads) {
  _test_data[CPU_THREADS_KEY] = cpu_threads;
}

unsigned short benchmarkr::GlobalTestContext::cpu_threads() const {
  require_init();
  return _test_data[CPU_THREADS_KEY].get<unsigned short>();
}

unsigned long long benchmarkr::GlobalTestContext::memory_installed() const {
  require_init();
  return _test_data[MEMORY_INSTALLED_KEY].get<unsigned long long>();
}

unsigned long long int benchmarkr::GlobalTestContext::memory_frequency() const {
  require_init();
  return _test_data[MEMORY_FREQUENCY_KEY].get<unsigned long long int>();
}

void benchmarkr::GlobalTestContext::memory_installed(unsigned long long int memory_installed) {
  _test_data[MEMORY_INSTALLED_KEY] = memory_installed;
}

void benchmarkr::GlobalTestContext::memory_frequency(unsigned long long int memory_frequency) {
  _test_data[MEMORY_FREQUENCY_KEY] = memory_frequency;
}

std::string benchmarkr::GlobalTestContext::hostname() const {
  require_init();
  return _test_data[HOSTNAME_KEY];
}

void benchmarkr::GlobalTestContext::hostname(std::string hostname) {
  _test_data[HOSTNAME_KEY] = hostname;
}

bool benchmarkr::GlobalTestContext::hasInitialized() const {
  return !_test_data.empty();
}

void benchmarkr::GlobalTestContext::construct_printable(std::map<std::string, std::string> &data) const {
  // nothing to do if the context has not been initialized
  if (!hasInitialized()) {
    return;
  }

  // add readable keys
  data["Operating System"] = os();
  data["Hostname"] = hostname();
  data["Memory Installed"] = std::to_string(memory_installed());
  data["Memory Frequency"] = std::to_string(memory_frequency());
  data["CPU Threads"] = std::to_string(cpu_threads());
  data["CPU Frequency"] = std::to_string(cpu_frequency());
}

void benchmarkr::GlobalTestContext::require_init() const {
  if (!hasInitialized()) {
    std::cerr << "Global Context was used before being initialized. Please run 'benchmarkr init-context` before testing";
    throw std::exception();
  }
}

nlohmann::json benchmarkr::GlobalTestContext::data() const {
  return _test_data;
}
