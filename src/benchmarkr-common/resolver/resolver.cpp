//
// Created by tyler on 2/23/22.
//

#include "benchmarkr-common/resolver/resolver.h"

#include <utility>

benchmarkr::UnresolvableException::UnresolvableException(std::string  what): _what(std::move(what)) {

}

const char *benchmarkr::UnresolvableException::what() const noexcept {
  return _what.c_str();
}

void benchmarkr::VariableResolver::push_back(std::unique_ptr<benchmarkr::Resolver> resolver) {
  _resolvers.push_back(std::move(resolver));
}

std::string benchmarkr::VariableResolver::to_str(const std::string& key) const {
  std::string resolved_value;

  for (auto &resolver: _resolvers) {
    resolved_value = resolver->resolve(key);

    if (!resolved_value.empty()) {
      break;
    }
  }

  if (resolved_value.empty()) {
    spdlog::error("Unable to resolve variable '{}'", key);
    throw benchmarkr::UnresolvableException("Unable to resolve variable '" +  key + "'");
  }

  spdlog::debug("Resolving Variable '{}' to String '{}'", key, resolved_value);
  return resolved_value;
}

std::string benchmarkr::VariableResolver::to_str(const std::string &key, const std::string& default_val) const {
  try {
    return to_str(key);
  } catch (benchmarkr::UnresolvableException& ex) {
    spdlog::debug("Resolving Variable '{}' to Default String '{}'", key, default_val);
    return default_val;
  }
}

int benchmarkr::VariableResolver::to_int(const std::string& key) const {
  std::string internal_val = to_str(key);

  try {
    return std::stoi(internal_val);
  } catch (std::exception& ex) {
    spdlog::error("Unable to cast {}('{}') to an int", key, internal_val);
    throw benchmarkr::UnresolvableException("Unable to cast " + key + "(" + internal_val + ") to an int");
  }
}

long benchmarkr::VariableResolver::to_long(const std::string& key) const {
  std::string internal_val = to_str(key);

  try {
    return std::stol(internal_val);
  } catch (std::exception& ex) {
    spdlog::error("Unable to cast {}('{}') to a long", key, internal_val);
    throw benchmarkr::UnresolvableException("Unable to cast " + key + "(" + internal_val + ") to a long");
  }
}

float benchmarkr::VariableResolver::to_float(const std::string& key) const {
  std::string internal_val = to_str(key);

  try {
    return std::stof(internal_val);
  } catch (std::exception& ex) {
    spdlog::error("Unable to cast {}('{}') to a float", key, internal_val);
    throw benchmarkr::UnresolvableException("Unable to cast " + key + "(" + internal_val + ") to an float");
  }
}

double benchmarkr::VariableResolver::to_double(const std::string& key) const {
  std::string internal_val = to_str(key);

  try {
    return std::stod(internal_val);
  } catch (std::exception& ex) {
    spdlog::error("Unable to cast {}('{}') to a double", key, internal_val);
    throw benchmarkr::UnresolvableException("Unable to cast " + key + "(" + internal_val + ") to an double");
  }
}
