#include <gtest/gtest.h>
#include <memory>

#include <utilities/tmp.h>

namespace {

class TestTmp : public ::testing::Test {};

TEST_F(TestTmp, LogVariousLevels) {
  const char *loggerName{"TestTmp"};
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

TEST_F(TestTmp, LogMultipleTimes) {
  const char *loggerName{"TestTmp"};
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

TEST_F(TestTmp, LogHigherLevel) {
  const char *loggerName{"TestTmp"};
  auto loggerWarn =
      std::make_unique<qle::Logger>(loggerName, qle::Logger::LogLevel::WARNING);
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

  auto loggerError =
      std::make_unique<qle::Logger>(loggerName, qle::Logger::LogLevel::ERROR);
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

}  // namespace
