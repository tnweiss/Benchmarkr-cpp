#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

#include "elk/common/authentication.h"
#include "elk/elasticsearch/elasticsearch_client.h"

#include "benchmarkr-common/global_test_context.h"
#include "benchmarkr-common/resolver/resolver.h"

#include "benchmarkr/variable_resolver.h"
#include "benchmarkr/upload.h"

static const char* UPLOAD_DESCRIPTION = "Upload all results to the remote elastic server.";
static const char* UPLOAD_USAGE = "benchmarkr upload [Flags]";


static benchmarkr::CommandVariableResolver variable_resolver(int argc, char **argv) {
  return benchmarkr::CommandVariableResolverBuilder()
      .with_usage(UPLOAD_USAGE)
      .with_description(UPLOAD_DESCRIPTION)
      .with_auth_type()
      .with_username()
      .with_password()
      .with_benchmarkr_dir()
      .with_log_level()
      .build(argc, argv);
}

static elk::ElkAuthentication authentication(benchmarkr::CommandVariableResolver& resolver) {
  std::string auth_type = resolver.auth_type();

  if (auth_type == "basic") {
    // collect username and passwords from the variables
    std::string username = resolver.username();
    std::string password = resolver.password();

    // log for debugging
    spdlog::debug("Initializing {0} auth with ({1}, {2})", auth_type, username, password);

    return {username.c_str(), password.c_str()};
  } else {
    spdlog::info("No auth handler for {0}", auth_type);

    return {};
  }
}

std::string benchmarkr::Upload::help() const {
  return variable_resolver(0, nullptr).help();
}

void benchmarkr::Upload::execute(int argc, char **argv) const {
  // get the variable resolver
  benchmarkr::CommandVariableResolver resolver = variable_resolver(argc - 1, argv + 1);

  benchmarkr::set_log_level(resolver.log_level());

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
  std::cout << "  Target Cluster Version   : " << cluster_details.version().number() << std::endl << std::endl << std::endl;

  // load global context
  benchmarkr::GlobalTestContext global_test_context(resolver.benchmarkr_dir());
  nlohmann::json global_context = global_test_context.init().data();

  // list out all the results files in the results directory
  std::string benchmarkr_dir = (std::filesystem::path(resolver.benchmarkr_dir()) /
      std::filesystem::path("results")).string();
  std::cout << "Uploading Results from " << benchmarkr_dir << std::endl;

  for (const auto & path: std::filesystem::directory_iterator(benchmarkr_dir)) {
    std::cout << "  Uploading " << path.path() << " ...";

    try {
      elk::BulkInsertBody request_body;
      std::vector<nlohmann::json> data_array = benchmarkr::parse_results(path.path().string(), global_context);
      for (auto & de: data_array) {
        request_body.create(elk::BulkInsertData(de));
      }

      if (!data_array.empty()) {
        elasticsearch_client.bulk_update_index("benchmarkr-0-0-0", request_body);
      }

      std::remove(path.path().string().c_str());

      std::cout << " DONE" << std::endl;
    } catch (std::exception& ex) {
      std::cout << " ERROR " << ex.what() << std::endl;
    }
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
