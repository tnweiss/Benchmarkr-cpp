//
// Created by tnwei on 2/20/2022.
//

#include "benchmarkr-common/resolver/cli_args.h"

benchmarkr::CLIArg::CLIArg(std::string external_short_name,
                           std::string external_name,
                           std::string internal_name,
                           std::string default_val):
                           CLIArg(std::move(external_short_name), std::move(external_name),
                                  std::move(internal_name), false) {
  spdlog::debug("Initializing '{}' with default '{}'", internal_name, default_val);
  _value = std::move(default_val);
}

benchmarkr::CLIArg::CLIArg(std::string external_short_name,
                           std::string external_name,
                           std::string internal_name,
                           bool required) {
  spdlog::debug("Initializing '{}' with external name '{}' and short name '{}'", internal_name, external_name,
                external_short_name);
  _external_short_name = std::move(external_short_name);
  _external_name = std::move(external_name);
  _internal_name = std::move(internal_name);
  _required = required;
}

benchmarkr::CLIArg::CLIArg(std::string external_short_name, std::string external_name, std::string internal_name):
    CLIArg(std::move(external_short_name), std::move(external_name),
           std::move(internal_name), ""){

}

void benchmarkr::CLIArg::validate() {
  if (_required && _value.empty()) {
    spdlog::error("Missing required value for argument '{}'", _internal_name);
    throw CLIArgParseException("Missing required parameter '" + _external_name + "' , '" +
      _external_short_name + "'");
  }
}

std::string &benchmarkr::CLIArg::value() {
  return _value;
}

std::string &benchmarkr::CLIArg::key() {
  return _internal_name;
}

bool benchmarkr::CLIArg::set(int* i, const int argc, char *argv[]) {
  if (*i + 1 >= argc) {
    spdlog::debug("Reached end of argument list, returning");
    return false;
  }

  spdlog::trace("Comparing current flag '{}' to current arg selectors '{}' and '{}'",
                argv[*i], _external_short_name, _external_name);
  if (std::strcmp(argv[*i], _external_short_name.c_str()) == 0 || strcmp(argv[*i], _external_name.c_str()) == 0) {
    spdlog::debug("Setting CLI Argument '{}' to '{}'", _internal_name, argv[*i + 1]);
    _value = argv[*i + 1];

    *i += 2;

    return true;
  }

  return false;
}

benchmarkr::CLIArgParseException::CLIArgParseException(std::string what): _what(std::move(what)) {

}

const char *benchmarkr::CLIArgParseException::what() const noexcept {
  return _what.c_str();
}

benchmarkr::CLIArgs::CLIArgs(): _args() {

}

void benchmarkr::CLIArgs::add_arg(benchmarkr::CLIArg arg) {
  spdlog::debug("Adding CLI argument {}", arg.key());
  _args.insert_or_assign(arg.key(), arg);
}

void benchmarkr::CLIArgs::parse(int argc, char **argv) {
  // initialize the argument index to 1
  int argI = 1;

  // used to determine if the value has been found
  bool found;

  // loop through the arguments until our argument index equals the argument count
  while (argI < argc) {
    // current argument has not been found
    found = false;

    for (auto & arg: _args) {
      // try to resolve the argument
      spdlog::debug(found);
      spdlog::debug(arg.second.key());
      found = found || arg.second.set(&argI, argc, argv);
    }

    // if not found then throw an unknown argument exception
    if (!found) {
      spdlog::error("Unknown flag '" + std::string(argv[argI]) + "'");
      throw benchmarkr::CLIArgParseException("Unknown flag '" + std::string(argv[argI]) + "'");
    }
  }
}

std::string benchmarkr::CLIArgs::resolve(const std::string &key) {
  if (_args.find(key) == _args.end()) {
    return "";
  }

  spdlog::debug("Resolved CLI Argument '{}' as '{}'", key, _args.at(key).value());
  return _args.at(key).value();
}
