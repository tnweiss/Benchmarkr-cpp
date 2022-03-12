//
// Created by tyler on 10/20/21.
//

#include "benchmarkr/help.h"

static const char* HELP_TEXT = R"(

Usage
  benchmarkr [ACTION] [FLAGS]

Actions
  [init]         Initialize the Elastic Indices and Kibana Dashboards.
  [init-context] Initialize the Global Test Context.
  [upload]       Upload results and erase them from the local machine.
  [upload-watch] Watch the results folder and immediately upload results to the remote elastic server.

Help
  Run "benchmarkr [ACTION] --help" for help with a specific action

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

const char *benchmarkr::Help::help() const {
  return HELP_TEXT;
}

