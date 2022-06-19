#include <chrono>
#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

#include "benchmarkr-common/resolver/resolver.h"

#include <benchmarkr/upload_watch.h>
#include "benchmarkr/upload.h"
#include <benchmarkr/variable_resolver.h>

static const char* UPLOAD_WATCH_DESCRIPTION = "Upload all results to the remote elastic server at a given interval.";
static const char* UPLOAD_WATCH_USAGE = "benchmarkr upload-watch [Flags]";


static benchmarkr::CommandVariableResolver variable_resolver(int argc, char **argv) {
  return benchmarkr::CommandVariableResolverBuilder()
      .with_usage(UPLOAD_WATCH_USAGE)
      .with_description(UPLOAD_WATCH_DESCRIPTION)
      .with_elastic_origin()
      .with_username()
      .with_password()
      .with_benchmarkr_dir()
      .with_interval()
      .with_log_level()
      .build(argc, argv);
}


std::string benchmarkr::UploadWatch::help() const {
  return variable_resolver(0, nullptr).help();
}


void benchmarkr::UploadWatch::execute(int argc, char **argv) const {
  // get the variable resolver
  benchmarkr::CommandVariableResolver resolver = variable_resolver(argc - 1, argv + 1);

  benchmarkr::set_log_level(resolver.log_level());

  benchmarkr::executeUploadWatch(resolver);
}

[[noreturn]] void benchmarkr::executeUploadWatch(const benchmarkr::CommandVariableResolver& resolver) {
  unsigned int interval = resolver.interval();

  unsigned int start = std::chrono::duration_cast< std::chrono::seconds >(
      std::chrono::system_clock::now().time_since_epoch()).count();

  while (true) {
    // execute the upload command
    benchmarkr::executeUpload(resolver);

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