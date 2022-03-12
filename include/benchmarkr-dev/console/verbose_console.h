//
// Created by tyler on 10/17/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_CONSOLE_VERBOSE_CONSOLE_H_
#define BENCHMARKR_SRC_BENCHMARKR_CONSOLE_VERBOSE_CONSOLE_H_

#include "console.h"

namespace benchmarkr {

/**
 * utility class for getting column distributions by proportion
 */
struct ColumnDistribution {
  /**
   * Initialize the distribution class with the total number of spaces
   * and distribution percentages
   */
  ColumnDistribution(int, const float[]);

  ~ColumnDistribution();

  /**
   * get a specific columns width
   * @return
   */
  int spacing_at(int);

  /**
   * does the distribution contain the value
   * @return
   */
  bool contains(int);
 private:
  int _distribution_size;
  int* _distribution;
};

class VerboseConsole: public Console{
 public:
  void print_headers(const std::map<std::string, std::string>&) final;
  void print_test(const std::string&, unsigned long, unsigned long) final;
  void print_test_result(unsigned long, bool) final;
  void print_failures(std::vector<const benchmarkr::Result*>&) final;
  void print_significant_success(std::vector<const benchmarkr::Result*>&) final;
  void print_summary(unsigned int, unsigned int, unsigned int, unsigned int) final;
};

/**
 * horizontal rule
 */
void hr(int, const char*, const char*);

/**
 * horizontal rule
 */
void hr(int, const char*, const char*,
        ColumnDistribution&);

/**
 * horizontal rule
 */
void center(int, const char*);

/**
 * Utility function to get the max length of a key
 * @return
 */
unsigned int max_key_len(const std::map<std::string, std::string>&);

}
#endif //BENCHMARKR_SRC_BENCHMARKR_CONSOLE_VERBOSE_CONSOLE_H_
