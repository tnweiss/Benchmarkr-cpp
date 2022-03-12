//
// Created by tyler on 10/17/21.
//

#include "benchmarkr-dev/console/verbose_console.h"

#include "benchmarkr-common/system.h"

// column proportions in the console
// Columns, Test Name, Upper Bound, Lower Bound, Duration
static const unsigned int num_columns = 4;
static const char* column_distribution_values[] = {"Test Name", "Upper Bound", "Lower Bound", "Duration"};
static const float column_distribution_percentages[] = {.55, .15, .15, .15};

// initialize the terminal object which will modify console settings if necessary
[[maybe_unused]] static Term::Terminal term; // NOLINT

///////////////////////////////////////// Column Distribution Helper /////////////////////////////////////////
benchmarkr::ColumnDistribution::ColumnDistribution(int total_columns,
                                                   const float distribution[]) {
  _distribution_size = 4; // TODO should be dynamic
  _distribution = new int[_distribution_size];

  for (int i=0; i<_distribution_size; i++) {
    _distribution[i] = static_cast<int>(std::floor(static_cast<double>(distribution[i]) *
        static_cast<double>(total_columns)));
  }
}

int benchmarkr::ColumnDistribution::spacing_at(int column) {
  return _distribution[column];
}

benchmarkr::ColumnDistribution::~ColumnDistribution() {
  delete _distribution;
}

bool benchmarkr::ColumnDistribution::contains(int val) {
  int curr = -1;
  for (int i=0; i<_distribution_size; i++) {
    curr += _distribution[i];
    if (curr == val) {
      return true;
    }
  }
  return false;
}

///////////////////////////////////////// Utility /////////////////////////////////////////
unsigned int benchmarkr::max_key_len(const std::map<std::string, std::string>& m) {
  unsigned int max_len = 0;
  for (auto const& k: m) {
    if (k.first.size() > max_len) {
      max_len = k.first.size();
    }
  }
  return max_len;
}

void benchmarkr::hr(int total, const char* prefix, const char* suffix) {
  std::cout << prefix;
  for (int i=0; i<total; i++) {
    std::cout << "#";
  }
  std::cout << suffix;
}

void benchmarkr::hr(int total, const char* prefix, const char* suffix,
                    benchmarkr::ColumnDistribution& column_distribution) {
  std::cout << prefix;
  for (int i=0; i<total; i++) {
    if (column_distribution.contains(i)) {
      std::cout << " ";
    } else {
      std::cout << "#";
    }
  }
  std::cout << suffix;
}

void benchmarkr::center(int total, const char* text){
  int w_half = (total / 2) - (strlen(text) / 2);
  std::cout << std::setw(w_half) << "" << std::setw(w_half) << std::left <<
            text << std::endl;
}

///////////////////////////////////////// Console /////////////////////////////////////////


void benchmarkr::VerboseConsole::print_headers(const std::map<std::string, std::string> &configuration) {
  // get the number of columns in the window
  int window_width = get_window_width();

  // print upper banner, center text, then lower banner
  hr(window_width, "\n", "\n");
  center(window_width, "Benchmarkr Tests");
  hr(window_width, "", "\n");

  // print configuration
  // get the max key len for spacing
  unsigned int mkl = max_key_len(configuration);
  // for each configuration value print it out
  for (auto const& k: configuration) {
    std::cout << std::setw(mkl) << std::left << k.first << " : " << k.second <<
              std::endl;
  }

  // add spacing between next item
  std::cout << std::endl;

  // generate the column distribution
  ColumnDistribution column_distribution(window_width,
                                         column_distribution_percentages);

  // print header information
  hr(window_width, "", "\n", column_distribution);
  for (int i=0; i<num_columns; i++) {
    std::cout << std::setw(column_distribution.spacing_at(i)) << std::left << column_distribution_values[i];
  }
  hr(window_width, "\n", "\n", column_distribution);
}

void benchmarkr::VerboseConsole::print_test(const std::string& test_name, unsigned long upper_bound,
                                            unsigned long lower_bound) {
  // get the number of columns in the window
  int window_width = get_window_width();

  // generate the column distribution
  ColumnDistribution column_distribution(window_width,
                                         column_distribution_percentages);

  std::cout << std::setw(column_distribution.spacing_at(0)) << test_name;
  std::cout << std::setw(column_distribution.spacing_at(1)) << upper_bound;
  std::cout << std::setw(column_distribution.spacing_at(2)) << lower_bound;
}

void benchmarkr::VerboseConsole::print_test_result(unsigned long duration, bool result) {
  // get the number of columns in the window
  int window_width = get_window_width();

  // generate the column distribution
  ColumnDistribution column_distribution(window_width,
                                         column_distribution_percentages);

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

  std::cout << std::setw(column_distribution.spacing_at(3)) <<
            result_text << std::endl;
}

void benchmarkr::VerboseConsole::print_failures(std::vector<const benchmarkr::Result *> &failures) {
  // if no failures occurred then return
  if (failures.empty()) {
    return;
  }

  // get the number of columns in the window
  int window_width = get_window_width();

  std::cout << Term::color(Term::fg::red);

  // print upper banner, center text, then lower banner
  hr(window_width, "\n", "\n");
  center(window_width, "Failures");
  hr(window_width, "", "\n");

  std::string output;

  for (const Result* r: failures) {
    output.append(  "Test:        ");
    output.append(r->name());
    output.append("\nDescription: ");
    output.append(r->description());
    output.append("\nReason:      ");
    output.append(r->outcome());
    output.append("\n\n\n");
  }

  std::cout << output << Term::color(Term::fg::reset);
}

void benchmarkr::VerboseConsole::print_significant_success(std::vector<const benchmarkr::Result *> &successes) {
  // if no failures occurred then return
  if (successes.empty()) {
    return;
  }

  // get the number of columns in the window
  int window_width = get_window_width();

  std::cout << Term::color(Term::fg::green);

  // print upper banner, center text, then lower banner
  hr(window_width, "\n", "\n");
  center(window_width, "Significant Successes");
  hr(window_width, "", "\n");

  std::string output;
  for (const Result* r: successes) {
    output.append("Test:                    ");
    output.append(r->name());
    output.append("\nPerformance Improvement: ");
    output.append(r->outcome());
    output.append("\n");
  }
  output.append("\nCelebrate and consider updating the standards.\n");

  std::cout << output << Term::color(Term::fg::reset);
}

void benchmarkr::VerboseConsole::print_summary(unsigned int total, unsigned int success,
                                               unsigned int significant_success,
                                               unsigned int failures) {
  // get the number of columns in the window
  int window_width = get_window_width() / 2;

  // print upper banner, center text, then lower banner
  hr(window_width, "\n\n\n", "\n");
  center(window_width, "Test Summary");
  hr(window_width, "", "\n");

  std::string result;
  if (failures > 0) {
    result.append(Term::color(Term::fg::red));
    result.append("FAILED");
  } else {
    result.append(Term::color(Term::fg::green));
    result.append("PASSED");
  }
  result.append(Term::color(Term::fg::reset));

  std::cout << "Successful Tests               :" << std::setw(4) << std::right << success << "/" << std::setw(4) << std::left << total << std::endl;
  std::cout << "Significantly Successful Tests :" << std::setw(4) << std::right << significant_success << "/" << std::setw(4) << std::left << total << std::endl;
  std::cout << "Failed Tests                   :" << std::setw(4) << std::right << failures << "/" << std::setw(4) << std::left << total << std::endl;
  std::cout << "Result                         : " << result << std::endl << std::endl;
}
