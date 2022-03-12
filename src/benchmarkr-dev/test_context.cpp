//
// Created by tnwei on 1/26/2022.
//

#include "benchmarkr-dev/test_context.h"

#include "benchmarkr-common/util.h"
#include "benchmarkr-common/defaults.h"
#include "benchmarkr-common/global_test_context.h"


static const char* LANGUAGE = "cpp";

static const benchmarkr::GlobalTestContext& global_context(std::filesystem::path benchmarkr_dir) {
  static benchmarkr::GlobalTestContext global_ctx(std::move(benchmarkr_dir));

  // if not initialized with root data then initialize
  if (!global_ctx.hasInitialized()) {
    global_ctx.init();
  }

  return global_ctx;
}

std::vector<benchmarkr::func_t>& benchmarkr::registered_bm_tests() {
  static std::vector<benchmarkr::func_t> bm_tests;
  return bm_tests;
}

void benchmarkr::register_bm_test( benchmarkr::func_t f) {
  registered_bm_tests().push_back(f);
}

std::string& benchmarkr::get_software_version() {
  static std::string software_version;
  return software_version;
}

void benchmarkr::set_software_version(const char* new_val) {
  get_software_version().erase();
  get_software_version().append(new_val);
}

benchmarkr::TestContext::TestContext():
    TestContext(benchmarkr::DEFAULT_BM_DIR) {

}

nlohmann::json benchmarkr::TestContext::as_json() const {
  return {
      {"Language", _language},
      {"BenchmarkrVersion", _benchmarkr_version},
      {"SoftwareVersion", _software_version}
  };
}

std::map<std::string, std::string> benchmarkr::TestContext::full_context_printable() const {
  std::map<std::string, std::string> full_context = {
      {"Timestamp", pretty_time(_timestamp)},
      {"Language", _language},
      {"Benchmarkr Version", _benchmarkr_version},
      {"Software Version", _software_version},
      {"Registered Tests", std::to_string(_registered_tests)}
  };

  global_context(_benchmarkr_dir).construct_printable(full_context);

  return full_context;
}

benchmarkr::TestContext::TestContext(std::filesystem::path benchmarkr_dir):
  _benchmarkr_dir(std::move(benchmarkr_dir)),
  _language(LANGUAGE),
  _benchmarkr_version(BENCHMARKR_VERSION),
  _registered_tests(benchmarkr::registered_bm_tests().size()),
  _timestamp(benchmarkr::epoch_millis()),
  _software_version(benchmarkr::get_software_version()) {

}
