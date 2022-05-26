//
// Created by tnwei on 5/25/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_VARIABLE_RESOLVER_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_VARIABLE_RESOLVER_H_

#include <vector>
#include "benchmarkr-common/resolver/resolver.h"

namespace benchmarkr {

  class CommandVariableResolver {
   public:
    CommandVariableResolver(const std::string&, const std::string&, const std::vector<std::string>&,
        benchmarkr::VariableResolver);

    [[nodiscard]] std::string elastic_origin() const;
    [[nodiscard]] std::string kibana_origin() const;
    [[nodiscard]] std::string auth_type() const;
    [[nodiscard]] std::string username() const;
    [[nodiscard]] std::string password() const;
    [[nodiscard]] std::string log_level() const;
    [[nodiscard]] int interval() const;
    [[nodiscard]] std::string benchmarkr_dir() const;
    [[nodiscard]] std::string index_json() const;
    [[nodiscard]] std::string setup_json() const;
    [[nodiscard]] std::string dashboard_ndjson() const;
    [[nodiscard]] std::string index_pattern_ndjson() const;

    [[nodiscard]] std::string help() const;
   private:
    const std::string _help;
    const benchmarkr::VariableResolver _variable_resolver;
  };

  class CommandVariableResolverBuilder {
   public:
    CommandVariableResolverBuilder();

    CommandVariableResolverBuilder& with_description(std::string description);
    CommandVariableResolverBuilder& with_usage(std::string usage);
    CommandVariableResolverBuilder& with_elastic_origin();
    CommandVariableResolverBuilder& with_kibana_origin();
    CommandVariableResolverBuilder& with_auth_type();
    CommandVariableResolverBuilder& with_username();
    CommandVariableResolverBuilder& with_password();
    CommandVariableResolverBuilder& with_log_level();
    CommandVariableResolverBuilder& with_interval();
    CommandVariableResolverBuilder& with_benchmarkr_dir();

    CommandVariableResolver build(int argc, char** argv);
   private:
    std::string _description;
    std::string _usage;

    bool _with_elastic_origin;
    bool _with_kibana_origin;
    bool _with_auth_type;
    bool _with_username;
    bool _with_password;
    bool _with_log_level;
    bool _with_interval;
    bool _with_benchmarkr_dir;
  };

}


#endif //BENCHMARKR_INCLUDE_BENCHMARKR_VARIABLE_RESOLVER_H_
