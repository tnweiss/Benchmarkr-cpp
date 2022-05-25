//
// Created by tnwei on 3/4/2022.
//

#include <benchmarkr/upload_watch.h>
#include <benchmarkr/upload.h>
#include "benchmarkr/command.h"
#include "benchmarkr/help.h"
#include "benchmarkr/init.h"
#include "benchmarkr/init-context.h"
#include "benchmarkr/print_version.h"

/**
 * Select the proper command given the action command
 * @param argc
 * @param argv
 * @return
 */
static std::unique_ptr<benchmarkr::Command> command(const std::string& action) {
  if (action == "init-context") {
    return std::make_unique<benchmarkr::InitContext>();
  } else if (action == "init") {
    return std::make_unique<benchmarkr::Init>();
  } else if (action == "upload") {
    return std::make_unique<benchmarkr::Upload>();
  } else if (action == "upload-watch") {
    return std::make_unique<benchmarkr::UploadWatch>();
  } else if (action == "version") {
    return std::make_unique<benchmarkr::PrintVersion>();
  } else {
    return std::make_unique<benchmarkr::Help>();
  }
}


int main(int argc, char* argv[]) {
  try {
    // ensure enough arguments for an action
    if (argc < 2) {
      std::cerr << std::endl << "Missing required action, see..." << benchmarkr::help_text() << std::endl;
      exit(1);
    }

    // parse action
    std::string action = argv[1];

    // get the command
    std::unique_ptr<benchmarkr::Command> cmd = command(action);

    // if asking for help on the specific command ex.
    //      benchmarkr [ACTION] --help
    //   then display the help for that action
    if (argc > 2 && std::strcmp(argv[2], "--help") == 0) {
      std::cout << cmd->help();
    } else {
      // execute the action
      cmd->execute(argc, argv);
    }
  } catch (std::exception& exception) {
    std::cerr << exception.what();
    exit(1);
  }
}