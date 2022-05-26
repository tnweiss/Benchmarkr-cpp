//
// Created by tyler on 2/5/22.
//

#include "benchmarkr-dev/results.h"

#include "benchmarkr-common/util.h"
#include "benchmarkr-common/defaults.h"


static const char* RESULTS_KEY = "Results";
static const char* ID_KEY = "ID";
static const char* TEST_CONTEXT_KEY = "LocalTestContext";


benchmarkr::Results::Results(unsigned long long num_results):
  Results(num_results, std::filesystem::path(benchmarkr::DEFAULT_BM_DIR)) {

}

void benchmarkr::Results::process(std::unique_ptr<benchmarkr::Result> result) {
  // add to significant failure or success collections
  if (result->significant_success()) {
    _significant_success.add(result.get());
    _significant_success_count ++;
  } else if (!result->success()) {
    _failures.add(result.get());
    _failure_count ++;
  } else {
    _success_count ++;
  }

  _results.push_back(std::move(result));
}

std::vector<std::unique_ptr<const benchmarkr::Result>>& benchmarkr::Results::results() {
  return _results;
}

std::vector<const benchmarkr::Result *> benchmarkr::Results::significant_success() {
  return _significant_success.as_vector();
}

std::vector<const benchmarkr::Result *> benchmarkr::Results::failures() {
  return _failures.as_vector();
}

void benchmarkr::Results::write_results(benchmarkr::TestContext &test_context) const {
  nlohmann::json output_json;

  // generate an ID for the file, should be in the format YYYYMMDDHHMMSSsss-<UUID>
  output_json[ID_KEY] = benchmarkr::uuid();

  // create the filename in the format <DATE>-<ID>.json
  std::string filename = benchmarkr::id_time(benchmarkr::epoch_millis()) + "." +
      output_json[ID_KEY].get<std::string>() + ".json";

  // create the path to the benchmarkr results directory
  std::filesystem::create_directories(_benchmarkr_results_dir);

  // create the full path to the output file
  std::filesystem::path filepath = _benchmarkr_results_dir / std::filesystem::path(filename);

  // set the test context as a global attribute
  output_json[TEST_CONTEXT_KEY] = test_context.as_json();

  // create the results array
  output_json[RESULTS_KEY] = nlohmann::json::array();

  // loop through each success
  for (const auto & _result : _results) {
    output_json[RESULTS_KEY].push_back(_result->as_json());
  }

  // notify the user
  std::cout << "Writing results to " << filepath << std::endl;

  // open the file
  std::ofstream results_file;
  results_file.open(filepath, std::ofstream::out);

  // serialize the test data
  results_file << output_json.dump(2);

  // close the stream
  results_file.close();
}

unsigned long long benchmarkr::Results::significant_success_count() const {
  return _significant_success_count;
}

unsigned long long benchmarkr::Results::success_count() const {
  return _success_count;
}

unsigned long long benchmarkr::Results::failure_count() const {
  return _failure_count;
}

benchmarkr::Results::Results(unsigned long long int num_results, const std::filesystem::path& benchmarkr_dir):
  _benchmarkr_results_dir(benchmarkr_dir / std::filesystem::path(benchmarkr::DEFAULT_BM_RESULTS_DIR)),
  _significant_success(10), _results(),
  _failures(10), _significant_success_count(0), _success_count(0), _failure_count(0) {

  results().reserve(num_results);
}

unsigned long long benchmarkr::Results::size() const {
  return _results.size();
}
