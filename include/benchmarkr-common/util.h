////
//// Created by Tyler on 10/6/2021.
////
//
#ifndef BENCHMARKR_SRC_BENCHMARKR_UTIL_H_
#define BENCHMARKR_SRC_BENCHMARKR_UTIL_H_

#include <string>

namespace benchmarkr {
  /**
   * Used in exception handling to print the human readable type names
   *   pulled from https://www.cplusplus.com/forum/beginner/175177/
   * @return
   */
  std::string demangle( const char* );

  /**
   * convert double to a string with a given precision
   * @return <VAL>.<PRECISION>%
   */
  std::string to_str_precision( double, int);

  /**
  * turns current unix timestamp into an ID string YYYYMMDDHHMMSSsss
  * @return
  */
  std::string id_time(long long epoch_millis);

  /**
  * Get the time since epoch in milliseconds
  * @return
  */
  long long epoch_millis();

  /**
  * Generate a new UUID
  * @return
  */
  std::string uuid();

  /**
  * Convert milliseconds since epoch to a readable format
  * @return
  */
  std::string pretty_time(long long epoch_millis);

  /**
   * set the environment value
   * @param key
   * @param val
   */
  void env_set(const std::string& key, const std::string& val);
}

#endif // BENCHMARKR_SRC_BENCHMARKR_UTIL_H_

