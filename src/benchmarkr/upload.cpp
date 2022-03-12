//
// Created by tnwei on 3/6/2022.
//

#include "benchmarkr/upload.h"

#include "benchmarkr-common/system.h"
#include "benchmarkr-common/defaults.h"
#include "benchmarkr-common/global_test_context.h"
#include "benchmarkr-common/resolver/resolver.h"
#include "benchmarkr-common/resolver/cli_args.h"
#include "benchmarkr-common/resolver/remote_config_resolver.h"
#include "benchmarkr-common/resolver/default_resolver.h"
#include "benchmarkr-common/resolver/environment_resolver.h"

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"
#include "elk/common/authentication.h"
#include "elk/elasticsearch/elasticsearch_client.h"
#include "elk/kibana/kibana_client.h"

static const char* ELASTIC_ORIGIN_SHORT_EXTERNAL = "-e";
static const char* ELASTIC_ORIGIN_EXTERNAL = "--elastic-origin";
static const char* ELASTIC_ORIGIN_INTERNAL = "BM_ELASTIC_ORIGIN";

static const char* AUTH_TYPE_SHORT_EXTERNAL = "-a";
static const char* AUTH_TYPE_EXTERNAL = "--auth-type";
static const char* AUTH_TYPE_INTERNAL = "BM_AUTH_TYPE";

static const char* USERNAME_SHORT_EXTERNAL = "-u";
static const char* USERNAME_EXTERNAL = "--username";
static const char* USERNAME_INTERNAL = "BM_USERNAME";

static const char* PASSWORD_SHORT_EXTERNAL = "-p";
static const char* PASSWORD_EXTERNAL = "--password";
static const char* PASSWORD_INTERNAL = "BM_PASSWORD";

static const char* BM_DIR_SHORT_EXTERNAL = "-b";
static const char* BM_DIR_EXTERNAL = "--bm-dir";
static const char* BM_DIR_INTERNAL = "BM_DIR";

static const char* INTERVAL_SHORT_EXTERNAL = "-i";
static const char* INTERVAL_EXTERNAL = "--interval";
static const char* INTERVAL_INTERNAL = "BM_INTERVAL";

static const char* LOG_LEVEL_SHORT_EXTERNAL = "-l";
static const char* LOG_LEVEL_EXTERNAL = "--log-level";
static const char* LOG_LEVEL_INTERNAL = "BM_LOG_LEVEL";

static const char* HELP_TEXT = R"(
Upload all results to the remote elastic server.

Usage
  benchmarkr upload [FLAGS]

SHORT FLAG, FLAG, ENVIRONMENT VAR
  -e, --elastic-origin, BM_ELASTIC_ORIGIN  Elasticsearch origin
  -a, --auth-type     , BM_AUTH_TYPE       Elasticsearch auth type
  -u, --username      , BM_USERNAME        ELK Username
  -p, --password      , BM_PASSWORD        ELK Password
  -b, --bm-dir        , BM_DIR             Benchmarkr Directory to create the global test context in
  -l, --log-level     , BM_LOG_LEVEL       Application log level

)";

static benchmarkr::VariableResolver resolver(int argc, char** argv) {
  benchmarkr::VariableResolver variable_resolver;

  std::unique_ptr<benchmarkr::DefaultResolver> default_resolver =
      std::make_unique<benchmarkr::DefaultResolver>();
  default_resolver->set_default(ELASTIC_ORIGIN_INTERNAL, "http://localhost:9200");
  default_resolver->set_default(BM_DIR_INTERNAL, benchmarkr::DEFAULT_BM_DIR);
  default_resolver->set_default(LOG_LEVEL_INTERNAL, "disabled");

  std::unique_ptr<benchmarkr::RemoteConfigResolver> remote_config_resolver =
      std::make_unique<benchmarkr::RemoteConfigResolver>();

  std::unique_ptr<benchmarkr::EnvironmentResolver> environment_resolver =
      std::make_unique<benchmarkr::EnvironmentResolver>();

  std::unique_ptr<benchmarkr::CLIArgs> cli_args =
      std::make_unique<benchmarkr::CLIArgs>();
  cli_args->add_arg(benchmarkr::CLIArg(ELASTIC_ORIGIN_SHORT_EXTERNAL, ELASTIC_ORIGIN_EXTERNAL, ELASTIC_ORIGIN_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(AUTH_TYPE_SHORT_EXTERNAL, AUTH_TYPE_EXTERNAL, AUTH_TYPE_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(USERNAME_SHORT_EXTERNAL, USERNAME_EXTERNAL, USERNAME_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(PASSWORD_SHORT_EXTERNAL, PASSWORD_EXTERNAL, PASSWORD_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(BM_DIR_SHORT_EXTERNAL, BM_DIR_EXTERNAL, BM_DIR_EXTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(INTERVAL_SHORT_EXTERNAL, INTERVAL_EXTERNAL, INTERVAL_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(LOG_LEVEL_SHORT_EXTERNAL, LOG_LEVEL_EXTERNAL, LOG_LEVEL_INTERNAL));
  cli_args->parse(argc - 1, argv + 1);

  variable_resolver.push_back(std::move(cli_args));
  variable_resolver.push_back(std::move(environment_resolver));
  variable_resolver.push_back(std::move(remote_config_resolver));
  variable_resolver.push_back(std::move(default_resolver));

  return variable_resolver;
}

/**
 *
 * @param resolver
 * @return
 */
static elk::ElkAuthentication authentication(benchmarkr::VariableResolver& resolver) {
  std::string auth_type = resolver.to_str(AUTH_TYPE_INTERNAL, "");

  if (auth_type == "basic") {
    // collect username and passwords from the variables
    std::string username = resolver.to_str(USERNAME_INTERNAL);
    std::string password = resolver.to_str(PASSWORD_INTERNAL);

    // log for debugging
    spdlog::debug("Initializing {0} auth with ({1}, {2})", auth_type, username, password);

    return {username.c_str(), password.c_str()};
  } else {
    spdlog::info("No auth handler for {0}", auth_type);

    return {};
  }
}

const char *benchmarkr::Upload::help() const {
  return HELP_TEXT;
}

void benchmarkr::Upload::execute(int argc, char **argv) const {
  // get the variable resolver
  benchmarkr::VariableResolver variable_resolver = resolver(argc, argv);

  benchmarkr::set_log_level(variable_resolver.to_str(LOG_LEVEL_INTERNAL));

  // get the authentication
  auto elk_auth = authentication(variable_resolver);

  // get the target elastic origin
  std::string elastic_origin = variable_resolver.to_str(ELASTIC_ORIGIN_INTERNAL);

  // initialize the client
  elk::ElasticsearchClient elasticsearch_client(elastic_origin.c_str(), elk_auth);

  // log
  std::cout << std::endl << std::endl << "Target Environment" << std::endl;
  std::cout << "  Target Elastic Server : " << elastic_origin << std::endl;

  // get the cluster details and log
  auto cluster_details = elasticsearch_client.get_cluster_details();
  std::cout << "  Target Cluster Name      : " << cluster_details.name() << std::endl;
  std::cout << "  Target Cluster Version   : " << cluster_details.version().number() << std::endl << std::endl << std::endl;

  // load global context
  benchmarkr::GlobalTestContext global_test_context(variable_resolver.to_str(BM_DIR_INTERNAL));
  nlohmann::json global_context = global_test_context.init().data();

  // list out all the results files in the results directory
  std::string benchmarkr_dir = (std::filesystem::path(variable_resolver.to_str(BM_DIR_INTERNAL)) /
      std::filesystem::path("results")).string();
  std::cout << "Uploading Results from " << benchmarkr_dir << std::endl;

  for (const auto & path: std::filesystem::directory_iterator(benchmarkr_dir)) {
    std::cout << "  Uploading " << path.path() << " ...";

    elk::BulkInsertBody request_body;
    std::vector<nlohmann::json> data_array = benchmarkr::parse_results(path.path().string(), global_context);
    for (auto & de: data_array) {
      request_body.create(elk::BulkInsertData(de));
    }

    elasticsearch_client.bulk_update_index("benchmarkr-0-0-0", request_body);

    std::remove(path.path().string().c_str());

    std::cout << " DONE" << std::endl;
  }
}


std::vector<nlohmann::json> benchmarkr::parse_results(const std::string& results_file_path, nlohmann::json& global_ctx) {
  // temporary variables to parse the text file
  std::string results_line;
  std::string results_text;
  std::ifstream results_file (results_file_path);

  // read each line and parse it into the full text
  while (std::getline(results_file, results_line)) {
    results_text.append(results_line);
  }

  // close the file
  results_file.close();

  // parse the text into a json
  nlohmann::json results_json = nlohmann::json::parse(results_text);

  // get the local context
  nlohmann::json local_context = results_json["LocalTestContext"];

  // Get the results and fold the local context and global context into the results
  std::vector<nlohmann::json> data = results_json["Results"].get<std::vector<nlohmann::json>>();

  for (auto & d: data) {
    // merge local context
    for (auto & ld: local_context.items()) {
      d[ld.key()] = ld.value();
    }

    // merge global context
    for (auto & gd: global_ctx.items()) {
      d[gd.key()] = gd.value();
    }
  }

  return data;
}
