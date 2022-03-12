//
// Created by Tyler on 10/4/2021.
//

#include "benchmarkr-dev/benchmarkr.h"

#include "benchmarkr-common/defaults.h"
#include "benchmarkr-common/resolver/resolver.h"
#include "benchmarkr-common/resolver/cli_args.h"
#include "benchmarkr-common/resolver/default_resolver.h"
#include "benchmarkr-common/resolver/remote_config_resolver.h"
#include "benchmarkr-common/resolver/environment_resolver.h"

#include "benchmarkr-dev/results.h"
#include "benchmarkr-dev/test_filter.h"
#include "benchmarkr-dev/test_context.h"
#include "benchmarkr-dev/console/console.h"
#include "benchmarkr-dev/console/ser_des.h"


static const char* ITERATIONS_SHORT_EXTERNAL = "-i";
static const char* ITERATIONS_EXTERNAL = "--iterations";
static const char* ITERATIONS_INTERNAL_KEY = "BM_ITERATIONS";

static const char* TEST_FILTERS_SHORT_EXTERNAL = "-t";
static const char* TEST_FILTERS_EXTERNAL = "--test";
static const char* TEST_FILTERS_INTERNAL_KEY = "BM_TEST_FILTERS";

static const char* CONSOLE_SHORT_EXTERNAL = "-c";
static const char* CONSOLE_EXTERNAL = "--console";
static const char* CONSOLE_INTERNAL_KEY = "BM_CONSOLE";

static const char* BM_DIR_SHORT_EXTERNAL = "-b";
static const char* BM_DIR_EXTERNAL = "--bm-dir";
static const char* BM_DIR_INTERNAL_KEY = "BM_DIR";

static const char* LOG_LEVEL_SHORT_EXTERNAL = "-l";
static const char* LOG_LEVEL_EXTERNAL = "--log-level";
static const char* LOG_LEVEL_INTERNAL = "BM_LOG_LEVEL";

/**
 * Command line parameters
 * @return
 */
static benchmarkr::CLIArgs cli_args() {
  benchmarkr::CLIArgs args;

  args.add_arg(benchmarkr::CLIArg(TEST_FILTERS_SHORT_EXTERNAL, TEST_FILTERS_EXTERNAL, TEST_FILTERS_INTERNAL_KEY));
  args.add_arg(benchmarkr::CLIArg(ITERATIONS_SHORT_EXTERNAL, ITERATIONS_EXTERNAL, ITERATIONS_INTERNAL_KEY));
  args.add_arg(benchmarkr::CLIArg(CONSOLE_SHORT_EXTERNAL, CONSOLE_EXTERNAL, CONSOLE_INTERNAL_KEY));
  args.add_arg(benchmarkr::CLIArg(BM_DIR_SHORT_EXTERNAL, BM_DIR_EXTERNAL, BM_DIR_INTERNAL_KEY));
  args.add_arg(benchmarkr::CLIArg(LOG_LEVEL_SHORT_EXTERNAL, LOG_LEVEL_EXTERNAL, LOG_LEVEL_INTERNAL));

  return args;
}

/**
 * Test Variable Resolver
 * Priority DEFAULT -> Remote Config -> ENV -> CLI
 * @return
 */
static benchmarkr::VariableResolver resolver(int argc, char *argv[]) {
  benchmarkr::VariableResolver resolver;

  // defaults
  std::unique_ptr<benchmarkr::DefaultResolver> default_resolver = std::make_unique<benchmarkr::DefaultResolver>();
  default_resolver->set_default(ITERATIONS_INTERNAL_KEY, "1");
  default_resolver->set_default(CONSOLE_INTERNAL_KEY, "Verbose");
  default_resolver->set_default(TEST_FILTERS_INTERNAL_KEY, "");
  default_resolver->set_default(BM_DIR_INTERNAL_KEY, benchmarkr::DEFAULT_BM_DIR);
  default_resolver->set_default(LOG_LEVEL_INTERNAL, "disabled");

  benchmarkr::CLIArgs command_line_resolver = cli_args();
  std::unique_ptr<benchmarkr::CLIArgs> cli_args = std::make_unique<benchmarkr::CLIArgs>(command_line_resolver);
  cli_args->parse(argc, argv);

  resolver.push_back(std::move(cli_args));
  resolver.push_back(std::make_unique<benchmarkr::EnvironmentResolver>());
  resolver.push_back(std::make_unique<benchmarkr::RemoteConfigResolver>());
  resolver.push_back(std::move(default_resolver));

  return resolver;
}

static void set_log_level(const std::string& log_level) {
  if (log_level == "disabled") {
    spdlog::set_level(spdlog::level::off);
  } else if (log_level == "debug") {
    spdlog::set_level(spdlog::level::debug);
  } else if (log_level == "info") {
    spdlog::set_level(spdlog::level::info);
  } else {
    std::cerr << std::endl <<"Unknown log level " << log_level << std::endl;
    exit(1);
  }
}

#ifndef BENCHMARKR_NO_MAIN
int main(int argc, char *argv[]) {
  // get the application resolver
  auto variable_resolver = resolver(argc, argv);

  // set the application log level
  set_log_level(variable_resolver.to_str(LOG_LEVEL_INTERNAL));

  // create the test filter
  benchmarkr::TestFilter tf(variable_resolver.to_str(TEST_FILTERS_INTERNAL_KEY, ""));

  // Get all the registered test generators
  std::vector<benchmarkr::func_t>& registered_tests = benchmarkr::registered_bm_tests();

  // Get the test context which will supplement out test results
  benchmarkr::TestContext test_context(variable_resolver.to_str(BM_DIR_INTERNAL_KEY));

  // create a list that will hold the test objects
  std::vector<std::unique_ptr<const benchmarkr::Test>> tests;
  tests.reserve(registered_tests.size());

  // the number of times to run the test
  const int iterations = variable_resolver.to_int(ITERATIONS_INTERNAL_KEY);
  spdlog::error("Iterations {}", iterations);

  // Results container
  benchmarkr::Results results(registered_tests.size() * iterations,
                              variable_resolver.to_str(BM_DIR_INTERNAL_KEY));

  // create console interface
  std::unique_ptr<benchmarkr::Console> console =
      benchmarkr::deserialize(variable_resolver.to_str(CONSOLE_INTERNAL_KEY));

  // random number generator used to shuffle tests
  auto rd = std::random_device("");
  std::mt19937 g(rd());

  // use the generators to create instances of the tests
  benchmarkr::Test* t;
  for (auto & registered_test : registered_tests) {
    t = registered_test();

    if (tf.passes(t)) {
      tests.emplace_back(registered_test());
    }
  }

  // print the header + configuration information
  console->print_headers(test_context.full_context_printable());

  for (int iteration=0; iteration < iterations; iteration++) {
    // shuffle the tests for each iteration
    std::shuffle(std::begin(tests), std::end(tests), g);

    // for each test, run it and store the results
    for (const std::unique_ptr<const benchmarkr::Test>& test: tests) {
      console->print_test(test->name(), test->upper_bound_us(), test->lower_bound_us());
      auto result = test->run();
      console->print_test_result(result->duration_us(), result->success());

      // transfer ownership of the pointer to the results collection
      results.process(std::move(result));
    }
  }

  auto failures = results.failures();
  console->print_failures(failures);
  auto significant_success = results.significant_success();
  console->print_significant_success(significant_success);
  console->print_summary(results.size(), results.success_count(), results.significant_success_count(),
                         results.failure_count());

  // save results to FS
  results.write_results(test_context);

  if (!results.failures().empty()) {
    exit(1);
  }

  return 0;
}
#endif
