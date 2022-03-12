//
// Created by tnwei on 2/7/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_SIGNIFICANT_RESULTS_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_SIGNIFICANT_RESULTS_H_

#include "result.h"

namespace benchmarkr {

/**
 * Set of significant results
 *   does not hold the data, just an indexing tool
 */
class SignificantResults {
 public:
  /**
   * Instantiate
   * @param max_size the max size of significant results
   */
  explicit SignificantResults(unsigned short max_size);

  /**
   *
   */
  ~SignificantResults();

  /**
   * Add the results to the set
   * @param result
   * @return true if the result was saved
   */
  bool add(Result* result);

  /**
   *
   * @return
   */
  [[nodiscard]] unsigned int size() const;

  /**
   * return the data structure as a vector
   * @return
   */
  [[nodiscard]] std::vector<const Result*> as_vector() const;
 private:
  unsigned int _max_size;
  unsigned int _size;

  /**
   * Data structure to create a sorted linked list
   */
  class SignificantNode {
   public:
    /**
     * Instantiate the default values
     */
    explicit SignificantNode(Result* result);

    /**
     * Get the pointer to the next node
     * @return
     */
    [[nodiscard]] SignificantNode* next() const;

    /**
     * Get the results delta
     * @return
     */
    [[nodiscard]] double abs_delta() const;

    /**
     * Get the pointer to the result
     * @return
     */
    [[nodiscard]] const Result* result() const;

    /**
     * Set the next value
     * @param after
     */
    void set_next(SignificantNode* after);
   private:
    SignificantNode* _next;
    const Result* _result;
  };

  SignificantNode* _end;
};

}
#endif //BENCHMARKR_INCLUDE_BENCHMARKR_SIGNIFICANT_RESULTS_H_
