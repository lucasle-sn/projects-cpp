#include <gtest/gtest.h>
#include <array>

#include <utilities/log_config.h>

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

class TestLoggerConfigHandler : public ::testing::Test {};

TEST_F(TestLoggerConfigHandler, Initialization) {
  std::array<int, 4> invalid_levels{-100, -1, qle::LogLevel::DISABLED + 1,
                                    qle::LogLevel::DISABLED + 100};
  std::array<int, 6> valid_levels{
      qle::LogLevel::TRACE,   qle::LogLevel::DEBUG, qle::LogLevel::INFO,
      qle::LogLevel::WARNING, qle::LogLevel::ERROR, qle::LogLevel::DISABLED};

  for (auto &level : invalid_levels) {
    auto logger_cfg =
        std::make_unique<qle::LoggerConfigHandler>(qle::LogLevel::Level(level));
    ASSERT_FALSE(logger_cfg->get_config());
  }

  for (auto &level : valid_levels) {
    auto logger_cfg =
        std::make_unique<qle::LoggerConfigHandler>(qle::LogLevel::Level(level));
    ASSERT_TRUE(logger_cfg->get_config());
    EXPECT_EQ(logger_cfg->get_config()->loglevel(), level);
    ASSERT_EQ(logger_cfg->get_config()->logfile(), nullptr);
  }

  {
    // Test only one config is created when multiple initializations are called
    auto logger_cfg_debug = std::make_unique<qle::LoggerConfigHandler>(
        qle::LogLevel::Level(qle::LogLevel::DEBUG));
    ASSERT_TRUE(logger_cfg_debug->get_config());
    EXPECT_EQ(logger_cfg_debug->get_config()->loglevel(), qle::LogLevel::DEBUG);
    ASSERT_EQ(logger_cfg_debug->get_config()->logfile(), nullptr);

    auto logger_cfg_info = std::make_unique<qle::LoggerConfigHandler>(
        qle::LogLevel::Level(qle::LogLevel::INFO));
    ASSERT_TRUE(logger_cfg_info->get_config() ==
                logger_cfg_debug->get_config());
    EXPECT_EQ(logger_cfg_info->get_config()->loglevel(), qle::LogLevel::DEBUG);

    auto logger_cfg_error = std::make_unique<qle::LoggerConfigHandler>(
        qle::LogLevel::Level(qle::LogLevel::INFO));
    ASSERT_TRUE(logger_cfg_error->get_config() ==
                logger_cfg_debug->get_config());
    EXPECT_EQ(logger_cfg_error->get_config()->loglevel(), qle::LogLevel::DEBUG);
  }

  {
    const char *tmpfile = "/tmp/test_log_config.txt";
    auto logger_cfg_debug = std::make_unique<qle::LoggerConfigHandler>(
        qle::LogLevel::Level(qle::LogLevel::DEBUG), tmpfile);
    ASSERT_TRUE(logger_cfg_debug->get_config());
    EXPECT_TRUE(logger_cfg_debug->get_config()->logfile());
  }
}

}  // namespace
