#include <benchmarkr/variable_resolver.h>
#include "benchmarkr/init_context.h"

#include "benchmarkr-common/system.h"
#include "benchmarkr-common/resolver/resolver.h"
#include "benchmarkr-common/global_test_context.h"


static const char* INIT_CONTEXT_DESCRIPTION = "Initialize the Global Context.";
static const char* INIT_CONTEXT_USAGE = "benchmarkr init-context [Flags]";


static benchmarkr::CommandVariableResolver variable_resolver(int argc, char **argv) {
  return benchmarkr::CommandVariableResolverBuilder()
      .with_usage(INIT_CONTEXT_USAGE)
      .with_description(INIT_CONTEXT_DESCRIPTION)
      .with_benchmarkr_dir()
      .with_log_level()
      .build(argc, argv);
}


std::string benchmarkr::InitContext::help() const {
  return variable_resolver(0, nullptr).help();
}

void benchmarkr::InitContext::execute(int argc, char **argv) const {
  if (!is_root()) {
    std::cerr << "\n!!! Warning !!!\n  Some Context data may not get generated properly, run this command as root\n\n";
  }

  // get the variable resolver
  benchmarkr::CommandVariableResolver resolver = variable_resolver(argc - 1, argv + 1);

  benchmarkr::set_log_level(resolver.log_level());

  benchmarkr::executeInitContext(resolver);
}

void benchmarkr::executeInitContext(const CommandVariableResolver& resolver) {
  // initialize the context and write to a file
  benchmarkr::GlobalTestContext(resolver.benchmarkr_dir()).
      init().
      writeToFile();

  // notify success
  std::cout << "Done" << std::endl;
}