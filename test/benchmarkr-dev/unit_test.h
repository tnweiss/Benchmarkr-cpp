//
// Created by tyler on 2/4/22.
//

#ifndef BENCHMARKR_TEST_BENCHMARKR_DEV_UNIT_TEST_H_
#define BENCHMARKR_TEST_BENCHMARKR_DEV_UNIT_TEST_H_

class TestArgs {
 public:
  /**
    * parse string arguments into dev cli args
    * @param arguments
    * @return
    */
  explicit TestArgs(std::vector<std::string> arguments);

  /**
   * Destructor
   */
  ~TestArgs();

  /**
   * Copy constructor
   */
  TestArgs(const TestArgs&) = delete;

  /**
   * Copy Assignment constructor
   * @param other
   * @return
   */
  TestArgs& operator=(const TestArgs& other) = delete;

  /**
   * Move Constructor
   * @param other
   */
  TestArgs(TestArgs&& other) noexcept: _argc(other._argc), _argv(other._argv) {
    other._argv = nullptr;
  }

  TestArgs& operator=(TestArgs&& other) noexcept // move assignment
  {
    std::swap(_argv, other._argv);
    _argc = other._argc;
    return *this;
  }

  /**
   * get the arguments
   * @return
   */
  [[nodiscard]] char** argv() const;

  /**
   * Get the argument count
   * @return
   */
  [[nodiscard]] int argc() const;

 private:
  char** _argv;
  int _argc;
};
#endif //BENCHMARKR_TEST_BENCHMARKR_DEV_UNIT_TEST_H_
