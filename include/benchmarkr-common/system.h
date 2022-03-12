//
// Created by Tyler on 10/7/2021.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_SYSTEM_H_
#define BENCHMARKR_SRC_BENCHMARKR_SYSTEM_H_

#include <string>

namespace benchmarkr {

// minimum console window width
#define MIN_CONSOLE_WIDTH 100

/**
 * Get the width of the current console
 * @return
 */
int get_window_width();

/**
 * Pretty print the operating system name
 * @return
 */
const char* os_name();

/**
 * Get the number of threads available
 * @return
 */
unsigned int cpu_threads();

/**
 * Get the base cpu frequency
 * @return
 */
unsigned int cpu_frequency();

/**
 * Get the total amount of memory installed (in kilobytes)
 * @return
 */
unsigned long long memory_installed();

/**
 * Get the average frequency of all installed memory
 * @return
 */
unsigned long long memory_frequency();

/**
 * Get the hostname of the current device
 * @return
 */
std::string hostname();

/**
 * Returns true if the current user is root
 * @return
 */
bool is_root();

/**
 * Requires that the current user is root
 */
void require_root(const std::string& action);

/**
 * Get the current users home directory
 * @return
 */
std::string home_directory();

/**
 * Given a command, run it and capture the output
 * @param command
 * @return
 */
std::string run(const char * command);
}


#endif // BENCHMARKR_SRC_BENCHMARKR_SYSTEM_H_
