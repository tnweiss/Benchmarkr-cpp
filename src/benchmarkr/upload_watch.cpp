//
// Created by tnwei on 3/8/2022.
//

#include <benchmarkr/upload_watch.h>

#include "benchmarkr/upload.h"
#include "benchmarkr-common/resolver/resolver.h"
#include "benchmarkr-common/resolver/cli_args.h"
#include "benchmarkr-common/resolver/default_resolver.h"
#include "benchmarkr-common/resolver/environment_resolver.h"

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"
#include <chrono>
#include "elk/kibana/kibana_client.h"


static const char* INTERVAL_SHORT_EXTERNAL = "-i";
static const char* INTERVAL_EXTERNAL = "--interval";
static const char* INTERVAL_INTERNAL = "BM_INTERVAL";

static const char* LOG_LEVEL_SHORT_EXTERNAL = "-l";
static const char* LOG_LEVEL_EXTERNAL = "--log-level";
static const char* LOG_LEVEL_INTERNAL = "BM_LOG_LEVEL";

static const char* HELP_TEXT = R"(
Upload all results to the remote elastic server at a given interval.

Usage
  benchmarkr upload-watch [FLAGS]

SHORT FLAG, FLAG, ENVIRONMENT VAR
  -e, --elastic-origin, BM_ELASTIC_ORIGIN  Elasticsearch origin
  -a, --auth-type     , BM_AUTH_TYPE       Elasticsearch auth type
  -u, --username      , BM_USERNAME        ELK Username
  -p, --password      , BM_PASSWORD        ELK Password
  -b, --bm-dir        , BM_DIR             Benchmarkr Directory to create the global test context in
  -i, --interval      , BM_INTERVAL        Time between checking the results folder
  -l, --log-level     , BM_LOG_LEVEL       Application log level

)";


const char *benchmarkr::UploadWatch::help() const {
  return HELP_TEXT;
}

static benchmarkr::VariableResolver resolver(int argc, char** argv) {
  benchmarkr::VariableResolver variable_resolver;

  std::unique_ptr<benchmarkr::DefaultResolver> default_resolver =
      std::make_unique<benchmarkr::DefaultResolver>();
  default_resolver->set_default(INTERVAL_EXTERNAL, "60");
  default_resolver->set_default(LOG_LEVEL_INTERNAL, "disabled");

  std::unique_ptr<benchmarkr::EnvironmentResolver> environment_resolver =
      std::make_unique<benchmarkr::EnvironmentResolver>();

  std::unique_ptr<benchmarkr::CLIArgs> cli_args =
      std::make_unique<benchmarkr::CLIArgs>();
  cli_args->add_arg(benchmarkr::CLIArg(INTERVAL_SHORT_EXTERNAL, INTERVAL_EXTERNAL, INTERVAL_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(LOG_LEVEL_SHORT_EXTERNAL, LOG_LEVEL_EXTERNAL, LOG_LEVEL_INTERNAL));
  cli_args->parse(argc - 1, argv + 1);

  variable_resolver.push_back(std::move(cli_args));
  variable_resolver.push_back(std::move(environment_resolver));
  variable_resolver.push_back(std::move(default_resolver));

  return variable_resolver;
}

[[noreturn]] void benchmarkr::UploadWatch::execute(int argc, char **argv) const {
  benchmarkr::VariableResolver variable_resolver = resolver(argc, argv);
  benchmarkr::Upload upload;

  benchmarkr::set_log_level(variable_resolver.to_str(LOG_LEVEL_INTERNAL));

  unsigned int interval = variable_resolver.to_int(INTERVAL_INTERNAL);

  unsigned int start = std::chrono::duration_cast< std::chrono::seconds >(
      std::chrono::system_clock::now().time_since_epoch()).count();

  while (true) {
    // run the upload command
    upload.execute(argc, argv);

    // get the end
    unsigned int end = std::chrono::duration_cast< std::chrono::seconds >(
        std::chrono::system_clock::now().time_since_epoch()).count();

    // notify user of sleeping

    std::cout << "-----------------------------------" << std::endl << "Sleeping " << interval - (end - start) <<
      " seconds ..." << std::endl << "-----------------------------------" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds (interval - (end - start)));

    start = std::chrono::duration_cast< std::chrono::seconds >(
        std::chrono::system_clock::now().time_since_epoch()).count();
  }
}