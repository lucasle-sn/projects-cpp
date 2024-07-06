#include <gtest/gtest.h>
#include <memory>

#include <utilities/log.h>

namespace {

std::mutex mtx_;

class TestLog : public ::testing::Test {
 protected:
  void SetUp() { qle::Logger::set_log_level(qle::Logger::LogLevel::INFO); }
  void TearDown() { qle::Logger::set_log_level(qle::Logger::LogLevel::INFO); }
};

TEST_F(TestLog, LogVariousLevels) {
  std::lock_guard<std::mutex> guard(mtx_);

  const char *loggerName{"TestLog"};
  auto logger = std::make_unique<qle::Logger>(loggerName);
  const char *msg{"Sample text"};

  {
    testing::internal::CaptureStdout();
    logger->info(msg);
    auto out = testing::internal::GetCapturedStdout();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "info", loggerName, msg);
    EXPECT_EQ(out, buff);
  }

  {
    testing::internal::CaptureStderr();
    logger->debug(msg);
    auto out = testing::internal::GetCapturedStderr();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "debug", loggerName, msg);
    EXPECT_EQ(out, buff);
  }

  {
    testing::internal::CaptureStderr();
    logger->warn(msg);
    auto out = testing::internal::GetCapturedStderr();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "warn", loggerName, msg);
    EXPECT_EQ(out, buff);
  }

  {
    testing::internal::CaptureStderr();
    logger->error(msg);
    auto out = testing::internal::GetCapturedStderr();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "error", loggerName, msg);
    EXPECT_EQ(out, buff);
  }
}

TEST_F(TestLog, LogMultipleTimes) {
  std::lock_guard<std::mutex> guard(mtx_);

  const char *loggerName{"TestLog"};
  auto logger = std::make_unique<qle::Logger>(loggerName);
  const char *msg{"Sample text"};

  for (size_t i = 0; i < 10; i++) {
    char modifiedMsg[100]{};
    snprintf(modifiedMsg, sizeof(modifiedMsg), "%s %zu", msg, i);

    // Check log info
    testing::internal::CaptureStdout();
    logger->info(modifiedMsg);
    auto out = testing::internal::GetCapturedStdout();

    char buffInfo[1024]{};
    snprintf(buffInfo, sizeof(buffInfo), "[%s] %s: %s\n", "info", loggerName,
             modifiedMsg);
    EXPECT_EQ(out, buffInfo);

    // Check log error
    testing::internal::CaptureStderr();
    logger->error(modifiedMsg);
    out = testing::internal::GetCapturedStderr();

    char buffError[1024]{};
    snprintf(buffError, sizeof(buffError), "[%s] %s: %s\n", "error", loggerName,
             modifiedMsg);
    EXPECT_EQ(out, buffError);
  }
}

TEST_F(TestLog, LogHigherLevel) {
  std::lock_guard<std::mutex> guard(mtx_);

  const char *loggerName{"TestLog"};
  qle::Logger::set_log_level(qle::Logger::LogLevel::WARNING);
  auto loggerWarn = std::make_unique<qle::Logger>(loggerName);
  const char *msg{"Sample text"};

  {
    // Expect NO Log info returned
    testing::internal::CaptureStdout();
    loggerWarn->info(msg);
    auto out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "");
  }

  {
    // Expect Log warn returned
    testing::internal::CaptureStderr();
    loggerWarn->warn(msg);
    auto out = testing::internal::GetCapturedStderr();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "warn", loggerName, msg);
    EXPECT_EQ(out, buff);
  }

  {
    // Expect Log error returned
    testing::internal::CaptureStderr();
    loggerWarn->error(msg);
    auto out = testing::internal::GetCapturedStderr();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "error", loggerName, msg);
    EXPECT_EQ(out, buff);
  }

  qle::Logger::set_log_level(qle::Logger::LogLevel::ERROR);
  auto loggerError = std::make_unique<qle::Logger>(loggerName);
  {
    // Expect NO Log info returned
    testing::internal::CaptureStdout();
    loggerError->info(msg);
    auto out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "");
  }

  {
    // Expect Log warn returned
    testing::internal::CaptureStderr();
    loggerError->warn(msg);
    auto out = testing::internal::GetCapturedStderr();
    EXPECT_EQ(out, "");
  }

  {
    // Expect Log error returned
    testing::internal::CaptureStderr();
    loggerError->error(msg);
    auto out = testing::internal::GetCapturedStderr();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "error", loggerName, msg);
    EXPECT_EQ(out, buff);
  }
}

TEST_F(TestLog, LogComplexFormat) {
  std::lock_guard<std::mutex> guard(mtx_);

  const char *loggerName{"TestLog"};
  auto logger = std::make_unique<qle::Logger>(loggerName);
  int tmp;
  const char *format{"%s %zu %d %p"};
  const char *expected_format{"[%s] %s: %s %zu %d %p\n"};

  const char *msg{"Sample text"};
  const size_t var = 1;
  {
    testing::internal::CaptureStdout();
    logger->info(format, msg, var, -2, &tmp);
    auto out = testing::internal::GetCapturedStdout();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "info", loggerName, msg, var,
             -2, &tmp);
    EXPECT_EQ(out, buff);
  }

  {
    testing::internal::CaptureStderr();
    logger->warn(format, msg, var, -2, &tmp);
    auto out = testing::internal::GetCapturedStderr();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "warn", loggerName, msg, var,
             -2, &tmp);
    EXPECT_EQ(out, buff);
  }

  {
    testing::internal::CaptureStderr();
    logger->error(format, msg, var, -2, &tmp);
    auto out = testing::internal::GetCapturedStderr();

    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "error", loggerName, msg, var,
             -2, &tmp);
    EXPECT_EQ(out, buff);
  }
}

}  // namespace
