//
// Created by tnwei on 2/19/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_CLI_ARGS_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_CLI_ARGS_H_

#include <map>
#include <string>

#include "resolver.h"

namespace benchmarkr {

/**
 * Custom exception used when an error occurs parsing arguments
 */
class CLIArgParseException: std::exception {
 public:
  /**
   *
   * @param what
   */
  explicit CLIArgParseException(std::string what);

  [[nodiscard]] const char * what () const noexcept override;
 private:
  std::string _what;
};

class CLIArg {
 public:
  /**
   * Initialize the argument
   * @param external_short_name the short command line alias e.g. '-t'
   * @param external_name the command line alias e.g. '--test'
   * @param internal_name the internal name of the variable e.g. 'TEST'
   */
  CLIArg(std::string external_short_name, std::string external_name, std::string internal_name);

  /**
   * Initialize the argument
   * @param external_short_name the short command line alias e.g. '-t'
   * @param external_name the command line alias e.g. '--test'
   * @param internal_name the internal name of the variable e.g. 'TEST'
   * @param required if true, an exception will get thrown if the value has not been populated
   */
  CLIArg(std::string external_short_name, std::string external_name, std::string internal_name, bool required);

  /**
   * Initialize the argument
   * @param external_short_name the short command line alias e.g. '-t'
   * @param external_name the command line alias e.g. '--test'
   * @param internal_name the internal name of the variable e.g. 'TEST'
   * @param default_val default value for the cli argument
   */
  CLIArg(std::string external_short_name, std::string external_name, std::string internal_name, std::string default_val);

  /**
   * Set the value if the flag for the variable is provided
   * @param argc arg count
   * @param argv arg value
   * @return true if the argument was found
   */
  bool set(int* i, int argc, char* argv[]);

  /**
   * get the key of the argument
   * @return
   */
  std::string& key();

  /**
   * Get the value of the argument
   * @return
   */
  std::string& value();

  /**
   * Validate the argument
   */
  void validate();
 private:
  std::string _external_short_name;
  std::string _external_name;
  std::string _internal_name;

  std::string _value;

  bool _required;
};

/**
 * Class used to parse command line arguments
 */
class CLIArgs : public benchmarkr::Resolver {
 public:
  /**
   * Default constructor
   */
  CLIArgs();

  /**
   * Add an argument
   * @param arg
   */
  void add_arg(CLIArg arg);

  /**
   * Parse arguments from a command line
   * @param argc
   * @param argv
   */
  void parse(int argc, char** argv);

  /**
   * Resolver interface method
   * @param key
   * @return
   */
  std::string resolve(const std::string& key) override;

 private:
  std::map<std::string, CLIArg> _args;
};
}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_COMMON_CLI_ARGS_H_
