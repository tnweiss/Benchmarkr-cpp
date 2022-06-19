#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

#include "elk/elasticsearch/elasticsearch_client.h"
#include "elk/kibana/kibana_client.h"

#include "benchmarkr/init.h"
#include "benchmarkr/version.h"
#include "benchmarkr/variable_resolver.h"

#include "benchmarkr-common/resolver/resolver.h"


static const char* INIT_DESCRIPTION = "Initialize the elasticsearch indices and kibana dashboards.";
static const char* INIT_USAGE = "benchmarkr init [Flags]";


std::string benchmarkr::create_index_name() {
  return fmt::format("benchmarkr-{}-{}-{}", BENCHMARKR_VERSION_MAJOR, BENCHMARKR_VERSION_MINOR,
              BENCHMARKR_VERSION_PATCH);
}

std::string benchmarkr::create_index_pattern_name() {
  return "benchmarkr-idx-pattern";
}

std::string benchmarkr::create_dashboard_name() {
  return "benchmarkr-dashboard";
}

static benchmarkr::CommandVariableResolver variable_resolver(int argc, char **argv) {
  return benchmarkr::CommandVariableResolverBuilder()
      .with_usage(INIT_USAGE)
      .with_description(INIT_DESCRIPTION)
      .with_log_level()
      .with_username()
      .with_password()
      .with_kibana_origin()
      .with_elastic_origin()
      .build(argc, argv);
}

std::string benchmarkr::Init::help() const {
  return variable_resolver(0, nullptr).help();
}

void benchmarkr::Init::execute(int argc, char **argv) const {
  benchmarkr::CommandVariableResolver resolver = variable_resolver(argc - 1, argv + 1);

  benchmarkr::set_log_level(resolver.log_level());

  benchmarkr::executeInit(resolver);
}

void benchmarkr::executeInit(const benchmarkr::CommandVariableResolver& resolver) {
  // get the index data from the remote file config
  nlohmann::json index_json = nlohmann::json::parse(resolver.index_json());

  // get the setup json for variable defaults
  nlohmann::json setup_json = nlohmann::json::parse(resolver.setup_json());

  // get the authentication
  auto elk_auth = authentication(resolver);

  // get the target elastic origin
  std::string elastic_origin = resolver.elastic_origin();

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
  std::string kibana_origin = resolver.kibana_origin();

  // initialize the kibana client
  elk::KibanaClient kibana_client(kibana_origin.c_str(), elk_auth);

  // log elk
  std::cout << "  Targeting Kibana Server  : " << kibana_origin << std::endl << std::endl << std::endl;

  // create the benchmarkr index if it does not exist
  std::string index_name = create_index_name();
  std::cout << "Creating Index " << index_name << " ..." << std::endl;
  if (!elasticsearch_client.index_exists(index_name.c_str())) {
    elk::CreateIndexBody create_index_body(index_json);
    elasticsearch_client.create_index(index_name.c_str(), create_index_body);

    std::cout << "  Created Index" << std::endl << std::endl << std::endl;
  } else {
    std::cout << "  Index Exists" << std::endl << std::endl << std::endl;
  }

  // create the benchmarkr index pattern
  std::string index_pattern_name = create_index_pattern_name();
  std::cout << "Creating Index Pattern " << index_pattern_name << " ..." << std::endl;
  if (!kibana_client.saved_object_exists(elk::SavedObjectType::INDEX_PATTERN, index_pattern_name.c_str())) {
    std::string index_pattern = resolver.index_pattern_ndjson();
    kibana_client.import_object(index_pattern);

    std::cout << "  Created Index Pattern" << std::endl << std::endl;
  } else {
    std::cout << "  Index Pattern Exists" << std::endl << std::endl;
  }

  std::string dashboard_name = create_dashboard_name();
  std::cout << "Creating Benchmarkr Dashboard " << dashboard_name << std::endl;
  if (!kibana_client.saved_object_exists(elk::SavedObjectType::DASHBOARD, dashboard_name.c_str())) {
    std::string dashboard = resolver.dashboard_ndjson();
    kibana_client.import_object(dashboard);

    std::cout << "  Created Dashboard" << std::endl << std::endl;
  } else {
    std::cout << "  Dashboard Exists" << std::endl << std::endl;
  }

  std::cout << std::endl << "Done" << std::endl;
}
