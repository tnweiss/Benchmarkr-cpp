//
// Created by tnwei on 1/21/2022.
//

#include "benchmarkr/init.h"

#include "benchmarkr-common/system.h"
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

static const char* KIBANA_ORIGIN_SHORT_EXTERNAL = "-k";
static const char* KIBANA_ORIGIN_EXTERNAL = "--kibana-origin";
static const char* KIBANA_ORIGIN_INTERNAL = "BM_KIBANA_ORIGIN";

static const char* AUTH_TYPE_SHORT_EXTERNAL = "-a";
static const char* AUTH_TYPE_EXTERNAL = "--auth-type";
static const char* AUTH_TYPE_INTERNAL = "BM_AUTH_TYPE";

static const char* USERNAME_SHORT_EXTERNAL = "-u";
static const char* USERNAME_EXTERNAL = "--username";
static const char* USERNAME_INTERNAL = "BM_USERNAME";

static const char* PASSWORD_SHORT_EXTERNAL = "-p";
static const char* PASSWORD_EXTERNAL = "--password";
static const char* PASSWORD_INTERNAL = "BM_PASSWORD";

static const char* LOG_LEVEL_SHORT_EXTERNAL = "-l";
static const char* LOG_LEVEL_EXTERNAL = "--log-level";
static const char* LOG_LEVEL_INTERNAL = "BM_LOG_LEVEL";

static const char* HELP_TEXT = R"(
Initialize the Elasticsearch indices and Kibana dashboards.

Usage
  benchmarkr init [FLAGS]

SHORT FLAG, FLAG, ENVIRONMENT VAR
  -e, --elastic-origin, BM_ELASTIC_ORIGIN  Elasticsearch origin
  -k, --kibana-origin , BM_KIBANA_ORIGIN   Kibana origin
  -a, --auth-type     , BM_AUTH_TYPE       Elasticsearch auth type
  -u, --username      , BM_USERNAME        ELK Username
  -p, --password      , BM_PASSWORD        ELK Password
  -l, --log-level     , BM_LOG_LEVEL       Application log level

)";

static const char* INDEX_PATTERN_REQUEST_BODY = R"(
{
  "attributes": {
    "title": "elk-cpp-test-index-*"
  }
}
)";


static benchmarkr::VariableResolver resolver(int argc, char** argv) {
  benchmarkr::VariableResolver variable_resolver;

  std::unique_ptr<benchmarkr::DefaultResolver> default_resolver =
      std::make_unique<benchmarkr::DefaultResolver>();
  default_resolver->set_default(ELASTIC_ORIGIN_INTERNAL, "http://localhost:9200");
  default_resolver->set_default(KIBANA_ORIGIN_INTERNAL, "http://localhost:5601");
  default_resolver->set_default(LOG_LEVEL_INTERNAL, "disabled");

  std::unique_ptr<benchmarkr::RemoteConfigResolver> remote_config_resolver =
      std::make_unique<benchmarkr::RemoteConfigResolver>();

  std::unique_ptr<benchmarkr::EnvironmentResolver> environment_resolver =
      std::make_unique<benchmarkr::EnvironmentResolver>();

  std::unique_ptr<benchmarkr::CLIArgs> cli_args =
      std::make_unique<benchmarkr::CLIArgs>();
  cli_args->add_arg(benchmarkr::CLIArg(ELASTIC_ORIGIN_SHORT_EXTERNAL, ELASTIC_ORIGIN_EXTERNAL, ELASTIC_ORIGIN_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(KIBANA_ORIGIN_SHORT_EXTERNAL, KIBANA_ORIGIN_EXTERNAL, KIBANA_ORIGIN_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(AUTH_TYPE_SHORT_EXTERNAL, AUTH_TYPE_EXTERNAL, AUTH_TYPE_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(USERNAME_SHORT_EXTERNAL, USERNAME_EXTERNAL, USERNAME_INTERNAL));
  cli_args->add_arg(benchmarkr::CLIArg(PASSWORD_SHORT_EXTERNAL, PASSWORD_EXTERNAL, PASSWORD_INTERNAL));
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

const char *benchmarkr::Init::help() const {
  return HELP_TEXT;
}

void benchmarkr::Init::execute(int argc, char **argv) const {
  // get the variable resolver
  benchmarkr::VariableResolver variable_resolver = resolver(argc, argv);

  benchmarkr::set_log_level(variable_resolver.to_str(LOG_LEVEL_INTERNAL));

  // get the index data from the remote file config
  nlohmann::json index_json = nlohmann::json::parse(variable_resolver.to_str("index.json"));

  // get the setup json for variable defaults
  nlohmann::json setup_json = nlohmann::json::parse(variable_resolver.to_str("setup.json"));

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
  std::cout << "  Target Cluster Version   : " << cluster_details.version().number() << std::endl;

  // get the kibana origin
  std::string kibana_origin = variable_resolver.to_str(KIBANA_ORIGIN_INTERNAL);

  // initialize the kibana client
  elk::KibanaClient kibana_client(kibana_origin.c_str(), elk_auth);

  // log elk
  std::cout << "  Targeting Kibana Server  : " << kibana_origin << std::endl << std::endl << std::endl;

  // create the benchmarkr index if it does not exist
  std::string index_name = "benchmarkr-0-0-0";
  std::cout << "Creating Index " << index_name << " ..." << std::endl;
  if (!elasticsearch_client.index_exists(index_name.c_str())) {
    elk::CreateIndexBody create_index_body(index_json);
    elasticsearch_client.create_index(index_name.c_str(), create_index_body);

    std::cout << "  Created Index" << std::endl << std::endl << std::endl;
  } else {
    std::cout << "  Index Exists" << std::endl << std::endl << std::endl;
  }

  // create the benchmarkr index pattern
  std::string index_pattern_name = "benchmarkr-idx-pattern";
  std::cout << "Creating Index Pattern " << index_pattern_name << " ..." << std::endl;
  if (!kibana_client.saved_object_exists(elk::SavedObjectType::INDEX_PATTERN, index_pattern_name.c_str())) {
    std::string index_pattern = variable_resolver.to_str("indexPattern.ndjson");
    kibana_client.import_object(index_pattern);

    std::cout << "  Created Index Pattern" << std::endl << std::endl;
  } else {
    std::cout << "  Index Pattern Exists" << std::endl << std::endl;
  }

  std::cout << "Creating Benchmarkr Dashboard " << std::endl;
  if (!kibana_client.saved_object_exists(elk::SavedObjectType::DASHBOARD, "benchmarkr-dashboard")) {
    std::string dashboard = variable_resolver.to_str("dashboard.ndjson");
    kibana_client.import_object(dashboard);

    std::cout << "  Created Dashboard" << std::endl << std::endl;
  } else {
    std::cout << "  Dashboard Exists" << std::endl << std::endl;
  }

  std::cout << std::endl << "Done" << std::endl;
}
