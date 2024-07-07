#include <gtest/gtest.h>
#include <memory>

#include <utilities/log.h>
#include <utilities/test_fixture.h>

namespace {

std::mutex mtx_;
const char *cLoggerName{"TestLog"};

class TestLog : public qle::TestFixture {
 protected:
  void SetUp() { qle::Logger::set_log_level(qle::Logger::LogLevel::INFO); }
  void TearDown() { qle::Logger::set_log_level(qle::Logger::LogLevel::INFO); }
};

TEST_F(TestLog, LogVariousLevels) {
  std::lock_guard<std::mutex> guard(mtx_);

  auto logger = std::make_unique<qle::Logger>(cLoggerName);
  const char *msg{"Sample text"};

  {
    auto out = capture_output([&](const char *msg) { logger->info(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "info", cLoggerName, msg);
    EXPECT_EQ(out["stdout"], buff);
  }

  {
    auto out =
        capture_output([&](const char *msg) { logger->debug(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "debug", cLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }
  {
    auto out = capture_output([&](const char *msg) { logger->warn(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "warn", cLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }
  {
    auto out =
        capture_output([&](const char *msg) { logger->error(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "error", cLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }
}

TEST_F(TestLog, LogMultipleTimes) {
  std::lock_guard<std::mutex> guard(mtx_);

  auto logger = std::make_unique<qle::Logger>(cLoggerName);
  const char *msg{"Sample text"};

  for (size_t i = 0; i < 10; i++) {
    char modified_msg[100]{};
    snprintf(modified_msg, sizeof(modified_msg), "%s %zu", msg, i);

    // Check log info
    auto out = capture_output([&](const char *msg) { logger->info(msg); },
                              modified_msg);
    char buff_out[1024]{};
    snprintf(buff_out, sizeof(buff_out), "[%s] %s: %s\n", "info", cLoggerName,
             modified_msg);
    EXPECT_EQ(out["stdout"], buff_out);

    // Check log error
    out = capture_output([&](const char *msg) { logger->error(msg); },
                         modified_msg);
    char buff_err[1024]{};
    snprintf(buff_err, sizeof(buff_err), "[%s] %s: %s\n", "error", cLoggerName,
             modified_msg);
    EXPECT_EQ(out["stderr"], buff_err);
  }
}

TEST_F(TestLog, LogHigherLevel) {
  std::lock_guard<std::mutex> guard(mtx_);

  qle::Logger::set_log_level(qle::Logger::LogLevel::WARNING);
  auto logger = std::make_unique<qle::Logger>(cLoggerName);
  const char *msg{"Sample text"};

  {
    // Expect NO Log info returned
    auto out = capture_output([&](const char *msg) { logger->info(msg); }, msg);
    EXPECT_EQ(out["stdout"], "");
  }

  {
    // Expect Log warn returned
    auto out = capture_output([&](const char *msg) { logger->warn(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "warn", cLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }

  {
    // Expect Log error returned
    auto out =
        capture_output([&](const char *msg) { logger->error(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "error", cLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }

  qle::Logger::set_log_level(qle::Logger::LogLevel::ERROR);
  {
    // Expect NO Log info returned
    auto out = capture_output([&](const char *msg) { logger->info(msg); }, msg);
    EXPECT_EQ(out["stdout"], "");
  }

  {
    // Expect NO Log warn returned
    auto out = capture_output([&](const char *msg) { logger->warn(msg); }, msg);
    EXPECT_EQ(out["stderr"], "");
  }

  {
    // Expect Log error returned
    auto out =
        capture_output([&](const char *msg) { logger->error(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "error", cLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }

  qle::Logger::set_log_level(qle::Logger::LogLevel::DISABLED);
  {
    // Expect NO Log error returned
    auto out =
        capture_output([&](const char *msg) { logger->error(msg); }, msg);
    EXPECT_EQ(out["stderr"], "");
  }
}

TEST_F(TestLog, LogComplexFormat) {
  std::lock_guard<std::mutex> guard(mtx_);

  auto logger = std::make_unique<qle::Logger>(cLoggerName);
  int tmp;
  const char *format{"%s %zu %d %p"};
  const char *expected_format{"[%s] %s: %s %zu %d %p\n"};

  const char *msg{"Sample text"};
  const size_t size_var = 1;
  const int int_var = -2;

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, int *ptr) {
          logger->info(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "info", cLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stdout"], buff);
  }

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, int *ptr) {
          logger->debug(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "debug", cLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stderr"], buff);
  }

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, int *ptr) {
          logger->warn(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "warn", cLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stderr"], buff);
  }

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, int *ptr) {
          logger->error(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "error", cLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stderr"], buff);
  }
}

}  // namespace
