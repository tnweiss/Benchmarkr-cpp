//
// Created by tyler on 11/1/21.
//

#ifndef BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_RESOLVERS_RESOLVER_H_
#define BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_RESOLVERS_RESOLVER_H_

#include <string>

namespace benchmarkr {

/**
 * Exception when resolving variables
 */
class UnresolvableException: std::exception {
  public:
    explicit UnresolvableException(std::string  what);
    [[nodiscard]] const char * what () const noexcept override;
  private:
    std::string _what;
};

class Resolver {
 public:
  /**
   * Resolve the provided variable
   *
   * Expected input may be HOST_X, HOST_Y, should not include
   *   the selector text or variable notation such as ${VAR:HOST_X}
   *
   * @return a string representation of the variable, or an empty string
   *   if the value could not be resolved
   */
  virtual std::string resolve(const std::string &) = 0;
};

class VariableResolver {
 public:
  /**
   * Add a resolver to the list of resolvers. The order matters,
   * highest priority comes first
   * @param resolver
   */
  void push_back(std::unique_ptr<benchmarkr::Resolver> resolver);

  /**
   * Resolve the variable and convert it to a string
   * @param key
   * @return
   */
  [[nodiscard]] std::string to_str(const std::string& key) const;

  /**
   * Resolve the value if possible, otherwise return the default
   * @param key
   * @param default_val
   * @return
   */
  [[nodiscard]] std::string to_str(const std::string& key, const std::string& default_val) const;

  /**
   * Resolve the variable and convert it to a int
   * @param key
   * @return
   */
  [[nodiscard]] int to_int(const std::string& key) const;

  /**
   * Resolve the variable and convert it to a long
   * @param key
   * @return
   */
  [[nodiscard]] long to_long(const std::string& key) const;

  /**
   * Resolve the variable and convert it to a float
   * @param key
   * @return
   */
  [[nodiscard]] float to_float(const std::string& key) const;

  /**
   * Resolve the variable and convert it to a double
   * @param key
   * @return
   */
  [[nodiscard]] double to_double(const std::string& key) const;

 private:
  std::vector<std::unique_ptr<benchmarkr::Resolver>> _resolvers;
};
}
#endif //BENCHMARKR_SRC_BENCHMARKR_COMMANDS_INIT_RESOLVERS_RESOLVER_H_
