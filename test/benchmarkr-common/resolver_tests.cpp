//
// Created by tyler on 2/23/22.
//

#include "catch.hpp"

#include "unit_test.h"
#include "benchmarkr-common/util.h"

#include "benchmarkr-common/resolver/cli_args.h"
#include "benchmarkr-common/resolver/default_resolver.h"
#include "benchmarkr-common/resolver/environment_resolver.h"
#include "benchmarkr-common/resolver/remote_config_resolver.h"


static const char* IRRELEVANT_NAME = "--some-other-arg";
static const char* IRRELEVANT_VALUE = "DOESNT_MATTER";
static const char* TEST_EXTERNAL_NAME = "--test";
static const char* TEST_EXTERNAL_SHORT_NAME = "-t";
static const char* TEST_INTERNAL_NAME = "TEST";
static const char* DEFAULT_VALUE = "default value";
static const char* VALUE = "TESTING_CLI_ARGS_IS_FUN";


TEST_CASE("CLIArgTest") {
  benchmarkr::CLIArg arg(TEST_EXTERNAL_SHORT_NAME, TEST_EXTERNAL_NAME,
                          TEST_INTERNAL_NAME);


  TestArgs cli_args_short({TEST_EXTERNAL_SHORT_NAME, VALUE});
  TestArgs cli_args({TEST_EXTERNAL_NAME, VALUE});

  int i = 1;

  SECTION("ParseValueWithName") {
    int argc = cli_args.argc();
    char** argv = cli_args.argv();

    REQUIRE(arg.set(&i, argc, argv));
    REQUIRE(arg.value() == VALUE);
  }

  SECTION("ParseValueWithShortName") {
    int argc = cli_args_short.argc();
    char** argv = cli_args_short.argv();

    REQUIRE(arg.set(&i, argc, argv));
    REQUIRE(arg.value() == VALUE);
  }

}

TEST_CASE("CLIArgDefaultTest") {
  benchmarkr::CLIArg arg(TEST_EXTERNAL_SHORT_NAME, TEST_EXTERNAL_NAME,
                                 TEST_INTERNAL_NAME, std::string(DEFAULT_VALUE));

  TestArgs cli_args_irrelevant({IRRELEVANT_NAME, IRRELEVANT_VALUE});

  int i = 1;
  int argc = cli_args_irrelevant.argc();
  char** argv = cli_args_irrelevant.argv();

  REQUIRE(!arg.set(&i, argc, argv));
  REQUIRE(arg.value() == DEFAULT_VALUE);
}

TEST_CASE("CLIArgRequiredTest") {
  benchmarkr::CLIArg arg(TEST_EXTERNAL_SHORT_NAME, TEST_EXTERNAL_NAME,
                         TEST_INTERNAL_NAME, true);

  TestArgs cli_args_irrelevant({IRRELEVANT_NAME, IRRELEVANT_VALUE});

  int i = 1;
  int argc = cli_args_irrelevant.argc();
  char** argv = cli_args_irrelevant.argv();

  REQUIRE(!arg.set(&i, argc, argv));
  REQUIRE_THROWS_AS(arg.validate(), benchmarkr::CLIArgParseException);
}

TEST_CASE("CLIArgsTest") {
  benchmarkr::CLIArg a("-a", "--test-a", "A");
  benchmarkr::CLIArg b("-b", "--test-b", "B");

  TestArgs cli_args({"-a", "A_VAL", "-b", "B_VAL"});

  int argc = cli_args.argc();
  char** argv = cli_args.argv();

  benchmarkr::CLIArgs cli_arg_resolver;
  cli_arg_resolver.add_arg(a);
  cli_arg_resolver.add_arg(b);

  cli_arg_resolver.parse(argc, argv);

  REQUIRE(cli_arg_resolver.resolve("A") == "A_VAL");
  REQUIRE(cli_arg_resolver.resolve("B") == "B_VAL");
  REQUIRE(cli_arg_resolver.resolve("C").empty());
}

TEST_CASE("CLIArgsUnknwonArgTest") {
  benchmarkr::CLIArg a("-a", "--test-a", "A");

  TestArgs cli_args({"-a", "A_VAL", "-b", "B_VAL"});

  int argc = cli_args.argc();
  char** argv = cli_args.argv();

  benchmarkr::CLIArgs cli_arg_resolver;
  cli_arg_resolver.add_arg(a);

  REQUIRE_THROWS_AS(cli_arg_resolver.parse(argc, argv), benchmarkr::CLIArgParseException);
}

TEST_CASE("DefaultResolver") {
  benchmarkr::DefaultResolver resolver;

  resolver.set_default("a", "AVal");
  resolver.set_default("b", "BVal");

  REQUIRE(resolver.resolve("a") == "AVal");
  REQUIRE(resolver.resolve("c").empty());
}

TEST_CASE("EnvironmentResolver") {
  benchmarkr::EnvironmentResolver resolver;

  benchmarkr::env_set("a", "AVal");

  REQUIRE(resolver.resolve("a") == "AVal");
  REQUIRE(resolver.resolve("c").empty());
}

TEST_CASE("RemoteConfigResolver") {
  benchmarkr::RemoteConfigResolver resolver;
  REQUIRE(!resolver.resolve("index.json").empty());
}

TEST_CASE("VariableResolver") {
  benchmarkr::VariableResolver resolver;

  // highest priority
  std::unique_ptr<benchmarkr::CLIArgs> cli_resolver = std::make_unique<benchmarkr::CLIArgs>();
  benchmarkr::CLIArg a("-a", "--test-a", "a");
  cli_resolver->add_arg(a);
  TestArgs cli_args({"-a", "A_VAL"});
  int argc = cli_args.argc();
  char** argv = cli_args.argv();
  cli_resolver->parse(argc, argv);

  // second highest priortiy
  std::unique_ptr<benchmarkr::EnvironmentResolver> env_resolver = std::make_unique<benchmarkr::EnvironmentResolver>();
  benchmarkr::env_set("a", "A_ENV_VAL");
  benchmarkr::env_set("b", "B_ENV_VAL");

  // third highest priority
  std::unique_ptr<benchmarkr::DefaultResolver> def_resolver = std::make_unique<benchmarkr::DefaultResolver>();
  def_resolver->set_default("a", "A_DEF_VAL");
  def_resolver->set_default("b", "B_DEF_VAL");
  def_resolver->set_default("c", "C_DEF_VAL");
  def_resolver->set_default("long", "1");
  def_resolver->set_default("int", "2");
  def_resolver->set_default("double", "3.3");
  def_resolver->set_default("float", "4.4");

  // add the resolvers
  resolver.push_back(std::move(cli_resolver));
  resolver.push_back(std::move(env_resolver));
  resolver.push_back(std::move(def_resolver));

  REQUIRE(resolver.to_str("a") == "A_VAL");
  REQUIRE(resolver.to_str("b") == "B_ENV_VAL");
  REQUIRE(resolver.to_str("c") == "C_DEF_VAL");

  REQUIRE(resolver.to_long("long") == 1);
  REQUIRE_THROWS_AS(resolver.to_long("a"), benchmarkr::UnresolvableException);

  REQUIRE(resolver.to_int("int") == 2);
  REQUIRE_THROWS_AS(resolver.to_int("a"), benchmarkr::UnresolvableException);

  REQUIRE(resolver.to_double("double") == 3.3);
  REQUIRE_THROWS_AS(resolver.to_double("a"), benchmarkr::UnresolvableException);

  REQUIRE(resolver.to_float("float") == 4.4f);
  REQUIRE_THROWS_AS(resolver.to_float("a"), benchmarkr::UnresolvableException);
}