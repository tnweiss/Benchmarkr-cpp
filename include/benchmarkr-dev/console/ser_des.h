//
// Created by tyler on 10/17/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_CONSOLE_SER_DES_H_
#define BENCHMARKR_SRC_BENCHMARKR_CONSOLE_SER_DES_H_

#include <memory>
#include <cstring>

#include "console.h"
#include "verbose_console.h"
#include "simple_console.h"
#include "silent_console.h"

namespace benchmarkr {

/**
 * Deserialize the args into a console type
 * @param args
 * @return
 */
std::unique_ptr<Console> deserialize(const std::string& console_type) {
  if (console_type == "Simple") {
    return std::make_unique<SimpleConsole>();
  } else if (console_type == "Silent") {
    return std::make_unique<SilentConsole>();
  } else if (console_type == "Verbose") {
    return std::make_unique<VerboseConsole>();
  } else {
    std::cout << "Unknown console type: " << console_type << std::endl;
    exit(1);
  }
}

}

#endif //BENCHMARKR_SRC_BENCHMARKR_CONSOLE_SER_DES_H_
