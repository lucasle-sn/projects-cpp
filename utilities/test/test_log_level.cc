#include <gtest/gtest.h>
#include <array>

#include <utilities/log_level.h>

namespace {

class TestLogLevel : public ::testing::Test {};

TEST_F(TestLogLevel, TestLogLevelValidity) {
  std::array<int, 4> invalid_levels{-100, -1, qle::LogLevel::DISABLED + 1,
                                    qle::LogLevel::DISABLED + 100};
  std::array<int, 6> valid_levels{
      qle::LogLevel::TRACE,   qle::LogLevel::DEBUG, qle::LogLevel::INFO,
      qle::LogLevel::WARNING, qle::LogLevel::ERROR, qle::LogLevel::DISABLED};

  for (auto &level : invalid_levels) {
    ASSERT_FALSE(
        qle::LogLevel::is_valid_log_level(qle::LogLevel::Level(level)));
  }

  for (auto &level : valid_levels) {
    ASSERT_TRUE(qle::LogLevel::is_valid_log_level(qle::LogLevel::Level(level)));
  }
}

TEST_F(TestLogLevel, TestLogLevelString) {
  /* clang-format off */
  ASSERT_TRUE(strcmp(qle::LogLevel::log_level_to_string(qle::LogLevel::TRACE), "trace") == 0);
  ASSERT_TRUE(strcmp(qle::LogLevel::log_level_to_string(qle::LogLevel::DEBUG), "debug") == 0);
  ASSERT_TRUE(strcmp(qle::LogLevel::log_level_to_string(qle::LogLevel::INFO), "info") == 0);
  ASSERT_TRUE(strcmp(qle::LogLevel::log_level_to_string(qle::LogLevel::WARNING), "warn") == 0);
  ASSERT_TRUE(strcmp(qle::LogLevel::log_level_to_string(qle::LogLevel::ERROR), "error") == 0);
  ASSERT_EQ(qle::LogLevel::log_level_to_string(qle::LogLevel::DISABLED), nullptr);
  /* clang-format on */
}

}  // namespace
