//
// Created by tyler on 10/19/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_COMMANDS_COMMAND_H_
#define BENCHMARKR_SRC_BENCHMARKR_COMMANDS_COMMAND_H_

namespace benchmarkr {

class Command {
 public:
  /**
   * Run the command
   */
  virtual void execute(int argc, char* argv[]) const = 0 ;

  /**
   * Get help text for the given command
   * @return
   */
  [[nodiscard]] virtual const char* help() const = 0;
};

/**
 * Used to set the logging level for the command
 * @param log_level
 */
void set_log_level(const std::string& log_level);

}


#endif //BENCHMARKR_SRC_BENCHMARKR_COMMANDS_COMMAND_H_
