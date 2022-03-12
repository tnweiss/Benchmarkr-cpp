//
// Created by Tyler on 10/6/2021.
//

#include "benchmarkr-common/util.h"

std::string benchmarkr::pretty_time(long long epoch_millis) {
  std::time_t time(epoch_millis / 1000);
  std::string pretty_time {std::asctime(std::localtime(&time))};

  // remove the trailing newline
  pretty_time.pop_back();

  return pretty_time;
}


std::string benchmarkr::to_str_precision( double value, int precision) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(precision) << value;
  return stream.str();
}

std::string benchmarkr::id_time(long long epoch_millis) {
  // get the timestamp in seconds
  long long epoch_seconds = epoch_millis / 1000;

  // parse the millis
  long long epoch_millis_remainder = epoch_millis % 1000;
  std::string epoch_millis_remainder_str = std::to_string(epoch_millis_remainder);

  std::time_t time(epoch_seconds);

  // create the buffer
  char buffer [18];

  // zero pad millis
  buffer[14] = '0';
  buffer[15] = '0';
  buffer[16] = '0';

  // add null terminator
  buffer[17] = '\0';

  // add the datetime to the buffer
  std::strftime(buffer, 16, "%Y%m%d%H%M%S", std::localtime(&time));

  // copy the millis to the buffer
  memcpy(&buffer[16 - epoch_millis_remainder_str.length()], epoch_millis_remainder_str.c_str(),
         epoch_millis_remainder_str.length());

  return {buffer};
}

long long benchmarkr::epoch_millis() {
  const auto now = std::chrono::system_clock::now();
  const auto epoch = now.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
}
