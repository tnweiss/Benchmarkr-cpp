//
// Created by tnwei on 2/7/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_RESULT_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_RESULT_H_

#include "nlohmann/json.hpp"

namespace benchmarkr {

/**
 * Test Result Container
 */
class Result {
 public:
  /**
   * Instantiate the result with immutable properties
   * @param name the name of the test
   * @param lower_bound the lower bound of the test
   * @param upper_bound the upper bound of the test
   * @param description the description of the test
   * @param custom_properties the custom properties
   */
  Result(std::string name, unsigned long long lower_bound, unsigned long long upper_bound,
         std::string description, nlohmann::json custom_properties);

  /**
   * Set the performance delta of the test, relative to the upper bound limit.
   * 0 = no change
   * > 0 = percentage above upper limi
   * < 0 = percentage below lower limit
   * @param delta
   */
  void set_delta(double delta);

  /**
   * Set the duration of the test
   * @param duration
   */
  void set_duration(unsigned long long duration);

  /**
   * Set the outcome of the test
   * @param outcome
   */
  void set_outcome(std::string outcome);

  /**
   * set the status of the test
   * @param success
   */
  void set_success(bool success);

  /**
   * set the status of the test
   * @param success
   */
  void set_significant_success(bool success);

  /**
   * Get the name of the test
   * @return
   */
  [[nodiscard]] std::string name() const;

  /**
   * Lower bound of the test
   * @return
   */
  [[nodiscard]] unsigned long long lower_bound() const;

  /**
   * upper bound of the test
   * @return
   */
  [[nodiscard]] unsigned long long upper_bound() const;

  /**
   * performance delta of the test
   * @return
   */
  [[nodiscard]] double delta() const;

  /**
   * Get the reason for failure or for success
   * @return
   */
  [[nodiscard]] const std::string& outcome() const;

  /**
   * Get the description of the test
   * @return
   */
  [[nodiscard]] const std::string& description() const;

  /**
  * Get the result of the test
  * @return
  */
  [[nodiscard]] bool success() const;

  /**
   * True if the test beat its lower bound
   * @return
   */
  [[nodiscard]] bool significant_success() const;

  /**
   * Get the actual test duration
   * @return
   */
  [[nodiscard]] unsigned long long duration_us() const;

  /**
   * Get the data as a json
   * @return
   */
  [[nodiscard]] const nlohmann::json& as_json() const;

 private:
  // json representation of the results
  nlohmann::json _data;

  // supplemental properties
  std::string _outcome;
  std::string _description;
};

}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_RESULT_H_
