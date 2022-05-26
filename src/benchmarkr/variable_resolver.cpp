#include <utility>

#include "benchmarkr/variable_resolver.h"

#include <benchmarkr-common/resolver/default_resolver.h>
#include <benchmarkr-common/resolver/remote_config_resolver.h>
#include <benchmarkr-common/resolver/environment_resolver.h>
#include <benchmarkr-common/resolver/cli_args.h>
#include "benchmarkr-common/defaults.h"

static const char* HELP_FIELD_FMT = "{:3}, {:16}, {:16} {:10}";

static const char* ELASTIC_ORIGIN_SHORT_EXTERNAL = "-e";
static const char* ELASTIC_ORIGIN_EXTERNAL = "--elastic-origin";
static const char* ELASTIC_ORIGIN_INTERNAL = "BM_ELASTIC_ORIGIN";
static const char* ELASTIC_ORIGIN_DESCRIPTION = "";
static const char* ELASTIC_DEFAULT = "http://localhost:9200";

static const char* KIBANA_ORIGIN_SHORT_EXTERNAL = "-k";
static const char* KIBANA_ORIGIN_EXTERNAL = "--kibana-origin";
static const char* KIBANA_ORIGIN_INTERNAL = "BM_KIBANA_ORIGIN";
static const char* KIBANA_ORIGIN_DESCRIPTION = "";
static const char* KIBANA_DEFAULT = "http://localhost:5601";

static const char* AUTH_TYPE_SHORT_EXTERNAL = "-a";
static const char* AUTH_TYPE_EXTERNAL = "--auth-type";
static const char* AUTH_TYPE_INTERNAL = "BM_AUTH_TYPE";
static const char* AUTH_TYPE_DESCRIPTION = "";

static const char* USERNAME_SHORT_EXTERNAL = "-u";
static const char* USERNAME_EXTERNAL = "--username";
static const char* USERNAME_INTERNAL = "BM_USERNAME";
static const char* USERNAME_DESCRIPTION = "";

static const char* PASSWORD_SHORT_EXTERNAL = "-p";
static const char* PASSWORD_EXTERNAL = "--password";
static const char* PASSWORD_INTERNAL = "BM_PASSWORD";
static const char* PASSWORD_DESCRIPTION = "";

static const char* LOG_LEVEL_SHORT_EXTERNAL = "-l";
static const char* LOG_LEVEL_EXTERNAL = "--log-level";
static const char* LOG_LEVEL_INTERNAL = "BM_LOG_LEVEL";
static const char* LOG_LEVEL_DESCRIPTION = "";
static const char* LOG_LEVEL_DEFAULT = "disabled";

static const char* INTERVAL_SHORT_EXTERNAL = "-i";
static const char* INTERVAL_EXTERNAL = "--interval";
static const char* INTERVAL_INTERNAL = "BM_INTERVAL";
static const char* INTERVAL_DESCRIPTION = "";
static const char* INTERVAL_DEFAULT = "60";

static const char* BM_DIR_SHORT_EXTERNAL = "-b";
static const char* BM_DIR_EXTERNAL = "--bm-dir";
static const char* BM_DIR_INTERNAL = "BM_DIR";
static const char* BM_DIR_DESCRIPTION = "";


benchmarkr::CommandVariableResolverBuilder::CommandVariableResolverBuilder(): _description(), _usage(),
  _with_auth_type(false), _with_benchmarkr_dir(false), _with_elastic_origin(false), _with_interval(false),
  _with_kibana_origin(false), _with_log_level(false), _with_password(false), _with_username(false) {

}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_description(std::string description) {
  _description = std::move(description);
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_usage(std::string usage) {
  _usage = std::move(usage);
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_elastic_origin() {
  _with_elastic_origin = true;
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_kibana_origin() {
  _with_kibana_origin = true;
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_auth_type() {
  _with_auth_type = true;
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_username() {
  _with_username = true;
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_password() {
  _with_password = true;
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_log_level() {
  _with_log_level = true;
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_interval() {
  _with_interval = true;
  return *this;
}

benchmarkr::CommandVariableResolverBuilder &benchmarkr::CommandVariableResolverBuilder::with_benchmarkr_dir() {
  _with_benchmarkr_dir = true;
  return *this;
}

benchmarkr::CommandVariableResolver benchmarkr::CommandVariableResolverBuilder::build(int argc, char** argv) {
  benchmarkr::VariableResolver variable_resolver;

  std::unique_ptr<benchmarkr::DefaultResolver> default_resolver =
      std::make_unique<benchmarkr::DefaultResolver>();
  std::unique_ptr<benchmarkr::RemoteConfigResolver> remote_config_resolver =
      std::make_unique<benchmarkr::RemoteConfigResolver>();
  std::unique_ptr<benchmarkr::EnvironmentResolver> environment_resolver =
      std::make_unique<benchmarkr::EnvironmentResolver>();
  std::unique_ptr<benchmarkr::CLIArgs> cli_args =
      std::make_unique<benchmarkr::CLIArgs>();

  std::vector<std::string> fields;

  if (_with_benchmarkr_dir) {
    fields.push_back(fmt::format(HELP_FIELD_FMT, BM_DIR_SHORT_EXTERNAL, BM_DIR_EXTERNAL, BM_DIR_INTERNAL, BM_DIR_DESCRIPTION));
    default_resolver->set_default(BM_DIR_INTERNAL, benchmarkr::DEFAULT_BM_DIR);
    cli_args->add_arg(benchmarkr::CLIArg(BM_DIR_SHORT_EXTERNAL, BM_DIR_EXTERNAL, BM_DIR_EXTERNAL));
  }

  if (_with_interval) {
    fields.push_back(fmt::format(HELP_FIELD_FMT, INTERVAL_SHORT_EXTERNAL, INTERVAL_EXTERNAL, INTERVAL_INTERNAL, INTERVAL_DESCRIPTION));
    default_resolver->set_default(INTERVAL_INTERNAL, INTERVAL_DEFAULT);
    cli_args->add_arg(benchmarkr::CLIArg(INTERVAL_SHORT_EXTERNAL, INTERVAL_EXTERNAL, INTERVAL_INTERNAL));
  }

  if (_with_log_level) {
    fields.push_back(fmt::format(HELP_FIELD_FMT, LOG_LEVEL_SHORT_EXTERNAL, LOG_LEVEL_EXTERNAL, LOG_LEVEL_INTERNAL, LOG_LEVEL_DESCRIPTION));
    default_resolver->set_default(LOG_LEVEL_INTERNAL, LOG_LEVEL_DEFAULT);
    cli_args->add_arg(benchmarkr::CLIArg(LOG_LEVEL_SHORT_EXTERNAL, LOG_LEVEL_EXTERNAL, LOG_LEVEL_INTERNAL));
  }

  if (_with_password) {
    fields.push_back(fmt::format(HELP_FIELD_FMT, PASSWORD_SHORT_EXTERNAL, PASSWORD_EXTERNAL, PASSWORD_INTERNAL, PASSWORD_DESCRIPTION));
    cli_args->add_arg(benchmarkr::CLIArg(PASSWORD_SHORT_EXTERNAL, PASSWORD_EXTERNAL, PASSWORD_INTERNAL));
  }

  if (_with_username) {
    fields.push_back(fmt::format(HELP_FIELD_FMT, USERNAME_SHORT_EXTERNAL, USERNAME_EXTERNAL, USERNAME_INTERNAL, USERNAME_DESCRIPTION));
    cli_args->add_arg(benchmarkr::CLIArg(USERNAME_SHORT_EXTERNAL, USERNAME_EXTERNAL, USERNAME_INTERNAL));
  }

  if (_with_auth_type) {
    fields.push_back(fmt::format(HELP_FIELD_FMT, AUTH_TYPE_SHORT_EXTERNAL, AUTH_TYPE_EXTERNAL, AUTH_TYPE_INTERNAL, AUTH_TYPE_DESCRIPTION));
    cli_args->add_arg(benchmarkr::CLIArg(AUTH_TYPE_SHORT_EXTERNAL, AUTH_TYPE_EXTERNAL, AUTH_TYPE_INTERNAL));
  }

  if (_with_elastic_origin) {
    fields.push_back(fmt::format(HELP_FIELD_FMT, ELASTIC_ORIGIN_SHORT_EXTERNAL, ELASTIC_ORIGIN_EXTERNAL, ELASTIC_ORIGIN_INTERNAL, ELASTIC_ORIGIN_DESCRIPTION));
    default_resolver->set_default(ELASTIC_ORIGIN_INTERNAL,ELASTIC_DEFAULT);
    cli_args->add_arg(benchmarkr::CLIArg(ELASTIC_ORIGIN_SHORT_EXTERNAL, ELASTIC_ORIGIN_EXTERNAL, ELASTIC_ORIGIN_INTERNAL));
  }

  if (_with_kibana_origin) {
    fields.push_back(fmt::format(HELP_FIELD_FMT, KIBANA_ORIGIN_SHORT_EXTERNAL, KIBANA_ORIGIN_EXTERNAL, KIBANA_ORIGIN_INTERNAL, KIBANA_ORIGIN_DESCRIPTION));
    default_resolver->set_default(KIBANA_ORIGIN_INTERNAL,KIBANA_DEFAULT);
    cli_args->add_arg(benchmarkr::CLIArg(KIBANA_ORIGIN_SHORT_EXTERNAL, KIBANA_ORIGIN_EXTERNAL, KIBANA_ORIGIN_INTERNAL));
  }

  cli_args->parse(argc, argv);

  variable_resolver.push_back(std::move(cli_args));
  variable_resolver.push_back(std::move(environment_resolver));
  variable_resolver.push_back(std::move(remote_config_resolver));
  variable_resolver.push_back(std::move(default_resolver));

  return {_description, _usage, fields, std::move(variable_resolver)};
}

static std::string join(const std::vector<std::string>& strings, const std::string& delim) {
  std::string data;
  for (const std::basic_string<char>& str: strings) {
    data += str + delim;
  }
  return data;
}


benchmarkr::CommandVariableResolver::CommandVariableResolver(
    const std::string& description, const std::string& usage, const std::vector<std::string>& fields,
    benchmarkr::VariableResolver variable_resolver):
    _help(fmt::format("Description\n  {}\n\nUsage\n  {}\n\nFlags\n  {}", description, usage,
                      join(fields, "\n  "))),
    _variable_resolver(std::move(variable_resolver))
    {

}

std::string benchmarkr::CommandVariableResolver::help() const {
  return _help;
}

std::string benchmarkr::CommandVariableResolver::benchmarkr_dir() const {
  return _variable_resolver.to_str(BM_DIR_INTERNAL);
}

int benchmarkr::CommandVariableResolver::interval() const {
  return _variable_resolver.to_int(INTERVAL_INTERNAL);
}

std::string benchmarkr::CommandVariableResolver::log_level() const {
  return _variable_resolver.to_str(LOG_LEVEL_INTERNAL);
}

std::string benchmarkr::CommandVariableResolver::password() const {
  return _variable_resolver.to_str(PASSWORD_INTERNAL);
}

std::string benchmarkr::CommandVariableResolver::username() const {
  return _variable_resolver.to_str(USERNAME_INTERNAL);
}

std::string benchmarkr::CommandVariableResolver::auth_type() const {
  return _variable_resolver.to_str(AUTH_TYPE_INTERNAL);
}

std::string benchmarkr::CommandVariableResolver::kibana_origin() const {
  return _variable_resolver.to_str(KIBANA_ORIGIN_INTERNAL);
}

std::string benchmarkr::CommandVariableResolver::elastic_origin() const {
  return _variable_resolver.to_str(ELASTIC_ORIGIN_INTERNAL);
}

std::string benchmarkr::CommandVariableResolver::index_json() const {
  return _variable_resolver.to_str("index.json");
}

std::string benchmarkr::CommandVariableResolver::setup_json() const {
  return _variable_resolver.to_str("setup.json");
}

std::string benchmarkr::CommandVariableResolver::index_pattern_ndjson() const {
  return _variable_resolver.to_str("indexPattern.ndjson");
}

std::string benchmarkr::CommandVariableResolver::dashboard_ndjson() const {
  return _variable_resolver.to_str("dashboard.ndjson");
}
