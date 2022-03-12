//
// Created by tyler on 10/17/21.
//

#include "benchmarkr-dev/console/simple_console.h"

// initialize the terminal object which will modify console settings if necessary
[[maybe_unused]] static Term::Terminal term; // NOLINT

void benchmarkr::SimpleConsole::print_headers(const std::map<std::string, std::string> &) {
  std::cout << std::endl
            << "  ###############################" << std::endl
            << "          Benchmarkr Test        " << std::endl
            << "  ###############################" << std::endl;
}

void benchmarkr::SimpleConsole::print_test(const std::string& test_name, unsigned long upper_bound,
                                           unsigned long lower_bound) {
  std::cout << std::setw(20) << std::left << "| Test" <<        ": " << test_name << std::endl
            << std::setw(20) << std::left << "| Lower Bound" << ": " << lower_bound  << std::endl
            << std::setw(20) << std::left << "| Upper Bound" << ": " << upper_bound << std::endl
            << std::setw(20) << std::left << "| Duration" <<    ": ";
}

void benchmarkr::SimpleConsole::print_test_result(unsigned long duration, bool result) {
  std::string result_text;
  // if passed mark it green, otherwise red
  if(result) {
    result_text.append(Term::color(Term::fg::green));
  } else {
    result_text.append(Term::color(Term::fg::red));
  }
  // if duration is not set and the test failed an exception occured
  if (duration == 0 && !result) {
    result_text.append("<ERROR>");
  } else {
    result_text.append(std::to_string(duration));
  }
  // disable the color
  result_text.append(Term::color(Term::fg::reset));
  result_text.append("\n");

  std::cout << result_text << std::endl;
}

void benchmarkr::SimpleConsole::print_failures(std::vector<const benchmarkr::Result *> &failures) {
  if (failures.empty()) {
    return;
  }

  std::string output;
  output.append("  ########## Failures ##########\n");

  for (const Result* r: failures) {
    output.append(  "| Test        : ");
    output.append(r->name());
    output.append("\n| Description : ");
    output.append(r->description());
    output.append("\n| Reason      : ");
    output.append(r->outcome());
    output.append("\n\n");
  }

  std::cout << Term::color(Term::fg::red) << output << Term::color(Term::fg::reset);
}

void benchmarkr::SimpleConsole::print_significant_success(std::vector<const benchmarkr::Result *> &successes) {
  if (successes.empty()) {
    return;
  }

  std::string output;
  output.append("  #### Significant Successes ####\n");
  for (const Result* r: successes) {
    output.append("| Test                    : ");
    output.append(r->name());
    output.append("\n| Performance Improvement : ");
    output.append(r->outcome());
    output.append("\n");
  }
  output.append("\nCelebrate and consider updating the standards.\n");

  std::cout << Term::color(Term::fg::green) << output << Term::color(Term::fg::reset);
}

void benchmarkr::SimpleConsole::print_summary(unsigned int, unsigned int, unsigned int, unsigned int) {
  // don't print anything
}
