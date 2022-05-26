#include <benchmarkr/test_connection.h>

#include "benchmarkr/help.h"

static const char* HELP_TEXT = R"(

Usage
  benchmarkr [Action] [Flags]

Actions
  [init]            Initialize the Elastic Indices and Kibana Dashboards.
  [init-context]    Initialize the Global Test Context.
  [upload]          Upload results and erase them from the local machine.
  [upload-watch]    Watch the results folder and immediately upload results to the remote elastic server.
  [version]         Print the Benchmarkr client version.
  [test-connection] Test connection to the remote elasticsearch server and report the status of the indices.

Flags
  Run "benchmarkr [Action] --help" for flag options and command descriptions

)";


const char* benchmarkr::help_text() {
  return HELP_TEXT;
}

void benchmarkr::Help::execute(int argc, char* argv[]) const {
  if (std::strcmp(argv[1], "help") != 0) {
    std::cout << std::endl << "Unrecognized Command '" << argv[1] << "'";
  }

  std::cout << HELP_TEXT << std::flush;
}

std::string benchmarkr::Help::help() const {
  return HELP_TEXT;
}
