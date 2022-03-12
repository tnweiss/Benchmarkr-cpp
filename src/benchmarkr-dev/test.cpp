//
// Created by tyler on 10/8/21.
//

#include "benchmarkr-dev//test.h"

#include <utility>

#include "benchmarkr-common/util.h"


// Elastic properties
static const char* BM_LOWER_BOUND_KEY = bm_prop_LowerBoundKey;
static const char* BM_UPPER_BOUND_KEY = bm_prop_UpperBoundKey;

// Supplemental properties
static const char* BM_DESCRIPTION_KEY = bm_prop_DescriptionKey;

// Defaults
static unsigned long long BM_LOWER_BOUND_DEFAULT = 0;
static unsigned long long BM_UPPER_BOUND_DEFAULT = 0;

/**
 * provides a safe interface to get keys from the internal map
 * @tparam T
 * @param key
 * @return
 */
template<typename T>
static T get (nlohmann::json& data, const char* key) {
  try {
    return data.at(key).get<T>();
  } catch (std::bad_cast& bc) {
    std::string err;
    err.append("Bad Cast Exception occurred when fetching \"");
    err.append(key);
    err.append("\"\n    Expected type: ");
    err.append(benchmarkr::demangle(typeid(T).name()));
    err.append("\n    Actual type:   ");
    err.append(benchmarkr::demangle(typeid(data[key].type()).name()));
    err.append("\n");

    std::cerr << err << std::endl;
    throw bc;
  }
}

benchmarkr::Test::Test(std::map<std::string, nlohmann::json> test_data): _custom_properties() {
  // set defaults
  _description = "";
  _lower_bound_us = BM_LOWER_BOUND_DEFAULT;
  _upper_bound_us = BM_UPPER_BOUND_DEFAULT;
  _setup = nullptr;
  _teardown = nullptr;

  // overrides
  if (test_data.count(BM_DESCRIPTION_KEY)) {
    _description = test_data.at(BM_DESCRIPTION_KEY).get<std::string>();
    test_data.erase(BM_DESCRIPTION_KEY);
  }

  if (test_data.count(BM_LOWER_BOUND_KEY)) {
    _lower_bound_us = test_data.at(BM_LOWER_BOUND_KEY).get<unsigned long long>();
    test_data.erase(BM_LOWER_BOUND_KEY);
  }

  if (test_data.count(BM_UPPER_BOUND_KEY)) {
    _upper_bound_us = test_data.at(BM_UPPER_BOUND_KEY).get<unsigned long long>();
    test_data.erase(BM_UPPER_BOUND_KEY);
  }

  // anything left is a custom property
  for (auto& d: test_data) {
    _custom_properties[d.first] = d.second;
  }
}

unsigned long long benchmarkr::Test::lower_bound_us() const {
  return _lower_bound_us;
}

unsigned long long benchmarkr::Test::upper_bound_us() const {
  return _upper_bound_us;
}

const std::string &benchmarkr::Test::description() const {
  return _description;
}

std::unique_ptr<benchmarkr::Result> benchmarkr::Test::run() const {
  // instantiate the result
  auto result = std::make_unique<Result>(name(), _lower_bound_us,
                                         _upper_bound_us, _description,
                                         _custom_properties);
  // test data
  std::map<std::string, void*> data{};

  try {
    // call setup
    if (_setup != nullptr) {
      _setup(data);
    }
    // mark the start
    auto start = std::chrono::steady_clock::now();
    // run the test
    execute(data);
    // mark the end
    auto end = std::chrono::steady_clock::now();
    // tear down
    if (_teardown != nullptr) {
      _teardown(data);
    }
    // set the duration of the test
    unsigned long long duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end - start).count();
    result->set_duration(duration);

    // determine the performance delta
    double percent_improvement = 0.0;
    if (upper_bound_us() != 0 && duration > upper_bound_us()) {
      // compute percent over the upper bound
      percent_improvement = 0 -
          ((((static_cast<double>(duration) - static_cast<double>(upper_bound_us())) /
              static_cast<double>(upper_bound_us())) * 100.0));

      // construct outcome message
      std::string msg;
      msg.append("Test Duration exceeded UpperBound.  ");
      msg.append(std::to_string(upper_bound_us()));
      msg.append(" < ");
      msg.append(std::to_string(duration));
      result->set_outcome(msg);

    } else if (lower_bound_us() != 0 && duration < lower_bound_us()) {
      // compute percent under lower bound
      percent_improvement =
          ((((static_cast<double>(lower_bound_us()) - static_cast<double>(duration)) /
              static_cast<double>(lower_bound_us())) * 100.0));

      // construct outcome message
      std::string text;
      text.append(to_str_precision(percent_improvement, 3));
      text.append("%");
      result->set_outcome(text);
    }

    result->set_delta(percent_improvement);
  } catch (std::exception& ex) {
    // set the outcome to the error message and mark the test failed
    result->set_outcome(ex.what());
    result->set_success(false);
  }

  // try tearing down the test resources
  try {
    if (_teardown != nullptr) {
      _teardown(data);
    }
  } catch (std::exception& ex) {
    std::cerr << std::endl << std::endl << "An exception occurred while tearing down " << name() << std::endl << std::endl;
  }

  // return the test result
  return result;
}

benchmarkr::Test::Test(std::map<std::string, nlohmann::json> test_data, const std::map<std::string, hook_f>& hooks): Test(std::move(test_data)) {
  // parse out the lifecycle hooks
  if (hooks.find(bm_hook_SetupKey) != hooks.end()) {
    _setup = hooks.at(bm_hook_SetupKey);
  }

  if (hooks.find(bm_hook_TeardownKey) != hooks.end()) {
    _setup = hooks.at(bm_hook_TeardownKey);
  }
}
