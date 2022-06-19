#include "benchmarkr/test_connection.h"

#include <elk/common/authentication.h>
#include <elk/elasticsearch/elasticsearch_client.h>
#include <elk/kibana/models/saved_object_type.h>
#include <elk/kibana/kibana_client.h>
#include "benchmarkr/variable_resolver.h"

#include "benchmarkr/init.h"

static const char* TEST_CONNECTION_DESCRIPTION = "Test connection to the remote elastic and report index status.";
static const char* TEST_CONNECTION_USAGE = "benchmarkr test-connection [Flags]";


static benchmarkr::CommandVariableResolver variable_resolver(int argc, char **argv) {
  return benchmarkr::CommandVariableResolverBuilder()
      .with_usage(TEST_CONNECTION_USAGE)
      .with_description(TEST_CONNECTION_DESCRIPTION)
      .with_elastic_origin()
      .with_kibana_origin()
      .with_username()
      .with_password()
      .with_log_level()
      .build(argc, argv);
}

std::string benchmarkr::TestConnection::help() const {
  return variable_resolver(0, nullptr).help();
}

void benchmarkr::TestConnection::execute(int argc, char **argv) const {
  // get the variable resolver
  benchmarkr::CommandVariableResolver resolver = variable_resolver(argc - 1, argv + 1);

  benchmarkr::set_log_level(resolver.log_level());

  benchmarkr::executeTestConnection(resolver);
}

void benchmarkr::executeTestConnection(const CommandVariableResolver& resolver) {
  // get the authentication
  auto elk_auth = authentication(resolver);

  // initialize the clients
  std::string elastic_origin = resolver.elastic_origin();
  elk::ElasticsearchClient elasticsearch_client(elastic_origin.c_str(), elk_auth);
  std::string kibana_origin = resolver.kibana_origin();
  elk::KibanaClient kibana_client(kibana_origin.c_str(), elk_auth);

  // get the cluster details and log
  auto cluster_details = elasticsearch_client.get_cluster_details();

  // search for the index
  std::string index_name = benchmarkr::create_index_name();
  std::string indexExists = "Error ";
  try {
    indexExists = elasticsearch_client.index_exists(index_name.c_str()) ? "Available": "Unavailable";
  } catch (std::exception& ex) {
    indexExists += ex.what();
  }

  // search for the index pattern
  std::string index_pattern_name = benchmarkr::create_index_pattern_name();
  std::string indexPatternExists = "Error ";
  try {
    indexPatternExists = kibana_client.saved_object_exists(
        elk::SavedObjectType::INDEX_PATTERN, index_pattern_name.c_str()) ? "Available": "Unavailable";
  } catch (std::exception& ex) {
    indexPatternExists += ex.what();
  }

  // search for the dashboard
  std::string dashboard_name = benchmarkr::create_dashboard_name();
  std::string dashboardExists = "Error ";
  try {
    dashboardExists = kibana_client.saved_object_exists(
        elk::SavedObjectType::DASHBOARD, dashboard_name.c_str()) ? "Available": "Unavailable";
  } catch (std::exception& ex) {
    dashboardExists += ex.what();
  }

  // print the results
  std::cout << "Cluster Name         : " << cluster_details.cluster_name()     << std::endl
            << "Cluster Version      : " << cluster_details.version().number() << std::endl
            << "Index Status         : " << indexExists                        << std::endl
            << "Index Pattern Status : " << indexPatternExists                 << std::endl
            << "Dashboard Status     : " << dashboardExists                    << std::endl;
}