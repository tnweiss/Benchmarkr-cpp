//
// Created by tyler on 2/5/22.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_RESULTS_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_RESULTS_H_

#include <vector>

#include "test.h"
#include "significant_results.h"
#include "test_context.h"

namespace benchmarkr {

class Results {
 public:
  /**
   * Default constructor
   */
  explicit Results(unsigned long long num_results);

  /**
    * Default constructor
    */
  explicit Results(unsigned long long num_results, const std::filesystem::path&  benchmarkr_dir);

  /**
   * Categorize the test
   * @param result
   */
  void process(std::unique_ptr<Result> result);

  /**
   * Get all tests that passed
   * @return
   */
  [[nodiscard]] std::vector<std::unique_ptr<const Result>>& results();

  /**
   * Get all tests that passed with significant improvement
   * @return
   */
  [[nodiscard]] std::vector<const Result*> significant_success();

  /**
   * Get all tests that failed
   * @return
   */
  [[nodiscard]] std::vector<const Result*> failures();

  /**
   * Write the results out to a file
   * @param test_context
   */
  void write_results(TestContext& test_context) const;

  /**
   * significant success count
   * @return
   */
  [[nodiscard]] unsigned long long significant_success_count() const;

  /**
   * Success count
   * @return
   */
  [[nodiscard]] unsigned long long success_count() const;

  /**
   * Failure count
   * @return
   */
  [[nodiscard]] unsigned long long failure_count() const;

  /**
   * Get the size of the result set
   * @return
   */
  [[nodiscard]] unsigned long long size() const;
 private:
  const std::filesystem::path _benchmarkr_results_dir;

  std::vector<std::unique_ptr<const Result>> _results;

  benchmarkr::SignificantResults _significant_success;
  benchmarkr::SignificantResults _failures;

  unsigned long long _success_count;
  unsigned long long _significant_success_count;
  unsigned long long _failure_count;
};
}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_DEV_BENCHMARKR_RESULTS_H_
