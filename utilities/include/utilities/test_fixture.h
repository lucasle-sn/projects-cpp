#ifndef UTILITIES_TEST_FIXTURE_H
#define UTILITIES_TEST_FIXTURE_H

#include <gtest/gtest.h>

#include <cstring>
#include <map>

#include <utilities/log_config.h>

namespace qle {

/**
 * @brief TestFixture class
 */
class TestFixture : public ::testing::Test {
 public:
  /**
   * @brief Function to capture the output from a function
   *
   * @tparam Func
   * @tparam Args
   * @param func Function to capture output
   * @param args Arguments of the function
   * @return std::map<std::string, std::string> for <"stdout","stderr">
   */
  template <typename Func, typename... Args>
  std::map<std::string, std::string> capture_output(Func func, Args &&...args) {
    std::map<std::string, std::string> output;

    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();

    // Call the provided function with arguments
    func(std::forward<Args>(args)...);

    // Get the output from custom stdout/stderr catcher
    output["stdout"] = testing::internal::GetCapturedStdout();
    output["stderr"] = testing::internal::GetCapturedStderr();
    return output;
  }

 protected:
  virtual void SetUp() {
    logger_cfg_handler_ =
        std::make_unique<qle::LoggerConfigHandler>(qle::LogLevel::DEBUG);
  }

  static std::mutex mtx_;  ///< mutex to prevent tests running in parallel

 private:
  std::unique_ptr<qle::LoggerConfigHandler> logger_cfg_handler_{nullptr};
};

}  // namespace qle

#endif  // UTILITIES_TEST_FIXTURE_H
