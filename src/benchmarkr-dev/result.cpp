//
// Created by tnwei on 2/7/2022.
//

#include "benchmarkr-dev/result.h"

#include "benchmarkr-common/util.h"


static const char* TIMESTAMP_KEY = "Timestamp";
static const char* TEST_ID_KEY = "TestID";
static const char* TEST_NAME_KEY = "TestName";
static const char* CUSTOM_PROPERTIES_KEY = "CustomProperties";
static const char* LOWER_BOUND_KEY = "LowerBound";
static const char* UPPER_BOUND_KEY = "UpperBound";
static const char* DELTA_KEY = "PerformanceDelta";
static const char* DURATION_KEY = "Duration";
static const char* SUCCESS_KEY = "Success";
static const char* SIGNIFICANT_SUCCESS_KEY = "SignificantSuccess";


benchmarkr::Result::Result(std::string name,
                           unsigned long long int lower_bound,
                           unsigned long long int upper_bound,
                           std::string description,
                           nlohmann::json custom_properties): _data(), _outcome() {
  _data[TEST_ID_KEY] = uuid();
  _data[TEST_NAME_KEY] = name;
  _data[TIMESTAMP_KEY] = epoch_millis();
  _data[CUSTOM_PROPERTIES_KEY] = std::move(custom_properties);
  _data[LOWER_BOUND_KEY] = lower_bound;
  _data[UPPER_BOUND_KEY] = upper_bound;

  _data[DURATION_KEY] = 0;
  _data[DELTA_KEY] = 0.0;
  _data[SUCCESS_KEY] = false;
  _data[SIGNIFICANT_SUCCESS_KEY] = false;

  _description = std::move(description);
}

void benchmarkr::Result::set_delta(double delta) {
  _data[DELTA_KEY] = delta;
}

void benchmarkr::Result::set_duration(unsigned long long int duration) {
  _data[DURATION_KEY] = duration;

  if (upper_bound() == 0 || duration < upper_bound()) {
    set_success(true);

    if (lower_bound() > 0 && duration < lower_bound()) {
      set_significant_success(true);
    }
  } else {
    set_success(false);
  }
}

void benchmarkr::Result::set_outcome(std::string outcome) {
  _outcome = std::move(outcome);
}

std::string benchmarkr::Result::name() const {
  return _data[TEST_NAME_KEY].get<std::string>();
}

unsigned long long benchmarkr::Result::lower_bound() const {
  return _data[LOWER_BOUND_KEY].get<unsigned long long>();
}

unsigned long long benchmarkr::Result::upper_bound() const {
  return _data[UPPER_BOUND_KEY].get<unsigned long long>();
}

double benchmarkr::Result::delta() const {
  return _data[DELTA_KEY].get<double>();
}

const std::string &benchmarkr::Result::outcome() const {
  return _outcome;
}

const std::string &benchmarkr::Result::description() const {
  return _description;
}

bool benchmarkr::Result::success() const {
  return _data[SUCCESS_KEY].get<bool>();
}

bool benchmarkr::Result::significant_success() const {
  return _data[SIGNIFICANT_SUCCESS_KEY].get<bool>();
}

unsigned long long benchmarkr::Result::duration_us() const {
  return _data[DURATION_KEY].get<unsigned long long>();
}

const nlohmann::json &benchmarkr::Result::as_json() const {
  return _data;
}

void benchmarkr::Result::set_success(bool success) {
  _data[SUCCESS_KEY] = success;
}

void benchmarkr::Result::set_significant_success(bool success) {
  _data[SIGNIFICANT_SUCCESS_KEY] = success;
}
