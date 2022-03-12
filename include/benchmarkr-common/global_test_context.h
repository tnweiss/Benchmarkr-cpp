//
// Created by tnwei on 2/7/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_GLOBAL_TEST_CONTEXT_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_GLOBAL_TEST_CONTEXT_H_

#include <string>

#include "nlohmann/json.hpp"

namespace benchmarkr {

/**
 * Context that hold Machine / OS specific properties
 */
class GlobalTestContext {
 public:
  /**
   * Initialize the global test context
   */
  GlobalTestContext();

  /**
  * Override the input / output filepath to the test context
  * @param filepath
  */
  explicit GlobalTestContext(std::filesystem::path benchmarkr_dir);

  /**
   * Write the context to the default file
   */
  void writeToFile() const;

  /**
   * Write the context to another file
   * @param filepath
   */
  void writeToFile(const std::string& benchmarkr_dir, const std::string& filepath) const;

  /**
   * Initialize the values
   */
  GlobalTestContext& init();

  /**
   * Returns true if the context has been initialized
   * @return
   */
  [[nodiscard]] bool hasInitialized() const;

  /**
   * Add context to the printable map
   * @param data
   */
  void construct_printable(std::map<std::string, std::string>& data) const;

  /**
   * Get the global context data
   * @return
   */
  nlohmann::json data() const;
  ///////////////////////////////////////////////// SETTERS / GETTERS /////////////////////////////////////////////////

  /**
   * Get the current operating system
   * @return
   */
  [[nodiscard]] std::string os() const;

  /**
   * Get the cpu frequency of the machine
   * @return
   */
  [[nodiscard]] unsigned short cpu_frequency() const;

  /**
   * Get the number of cpu threads
   * @return
   */
  [[nodiscard]] unsigned short cpu_threads() const;

  /**
   * Get the amount of memory installed
   * @return
   */
  [[nodiscard]] unsigned long long memory_installed() const;

  /**
   * Get the memory frequency
   * @return
   */
  [[nodiscard]] unsigned long long int memory_frequency() const;

  /**
   * Get the hostname
   * @return
   */
  [[nodiscard]] std::string hostname() const;
 protected:
  /**
   * Set the cpu frequency of the machine
   * @param cpu_frequency
   */
  void cpu_frequency(unsigned short cpu_frequency);

  /**
   * Set the current operating system
   * @param os
   */
  void os(std::string os);

  /**
   * Set the number of cpu threads
   * @param cpu_threads
   */
  void cpu_threads(unsigned short cpu_threads);

  /**
   * Set the memory installed
   * @param memory_installed
   */
  void memory_installed(unsigned long long memory_installed);

  /**
   * set the memory frequency
   * @param memory_frequency
   */
  void memory_frequency(unsigned long long int memory_frequency);

  /**
   * set the hostname
   * @param hostname
   */
  void hostname(std::string hostname);

  /**
   * Requires the global test context to be initialized before use
   */
  void require_init() const;
 private:
  const std::filesystem::path _benchmarkr_dir;
  const std::string _filename;

  nlohmann::json _test_data;
};

}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_GLOBAL_TEST_CONTEXT_H_
