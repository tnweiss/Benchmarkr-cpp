//
// Created by tnwei on 3/4/2022.
//

#include "benchmarkr/init-context.h"

#include "benchmarkr-common/system.h"
#include "benchmarkr-common/defaults.h"
#include "benchmarkr-common/resolver/resolver.h"
#include "benchmarkr-common/resolver/cli_args.h"
#include "benchmarkr-common/global_test_context.h"
#include "benchmarkr-common/resolver/default_resolver.h"
#include "benchmarkr-common/resolver/environment_resolver.h"


static const char* BM_DIR_SHORT_EXTERNAL = "-b";
static const char* BM_DIR_EXTERNAL = "--bm-dir";
static const char* BM_DIR_INTERNAL = "BM_DIR";

static const char* LOG_LEVEL_SHORT_EXTERNAL = "-l";
static const char* LOG_LEVEL_EXTERNAL = "--log-level";
static const char* LOG_LEVEL_INTERNAL = "BM_LOG_LEVEL";

static const char* HELP_TEXT = R"(
Initialize the Global Context.

Usage
  benchmarkr init-context [FLAGS]

SHORT FLAG, FLAG, ENVIRONMENT VAR
  -b, --bm-dir        , BM_DIR             Benchmarkr Directory to create the global test context in
  -l, --log-level     , BM_LOG_LEVEL       Application log level

)";

static benchmarkr::VariableResolver resolver(int argc, char** argv) {
  benchmarkr::VariableResolver variable_resolver;

  std::unique_ptr<benchmarkr::DefaultResolver> default_resolver =
      std::make_unique<benchmarkr::DefaultResolver>();
  default_resolver->set_default(BM_DIR_INTERNAL, benchmarkr::DEFAULT_BM_DIR);
  default_resolver->set_default(LOG_LEVEL_INTERNAL, "disabled");

  std::unique_ptr<benchmarkr::EnvironmentResolver> environment_resolver =
      std::make_unique<benchmarkr::EnvironmentResolver>();

  std::unique_ptr<benchmarkr::CLIArgs> cli_args =
      std::make_unique<benchmarkr::CLIArgs>();
  cli_args->add_arg(benchmarkr::CLIArg(BM_DIR_SHORT_EXTERNAL, BM_DIR_EXTERNAL, BM_DIR_EXTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(LOG_LEVEL_SHORT_EXTERNAL, LOG_LEVEL_EXTERNAL, LOG_LEVEL_INTERNAL));
  cli_args->parse(argc - 1, argv + 1);

  variable_resolver.push_back(std::move(cli_args));
  variable_resolver.push_back(std::move(environment_resolver));
  variable_resolver.push_back(std::move(default_resolver));

  return variable_resolver;
}

const char *benchmarkr::InitContext::help() const {
  return HELP_TEXT;
}

void benchmarkr::InitContext::execute(int argc, char **argv) const {
  if (!is_root()) {
    std::cerr << "\n!!! Warning !!!\n  Some Context data may not get generated properly, run this command as root\n\n";
  }

  // get the variable resolver
  benchmarkr::VariableResolver variable_resolver = resolver(argc, argv);

  benchmarkr::set_log_level(variable_resolver.to_str(LOG_LEVEL_INTERNAL));

  // initialize the context and write to a file
  benchmarkr::GlobalTestContext(variable_resolver.to_str(BM_DIR_INTERNAL)).
    init().
    writeToFile();

  // notify success
  std::cout << "Done" << std::endl;
}
