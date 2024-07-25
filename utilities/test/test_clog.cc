#include <gtest/gtest.h>
#include <memory>

#include <utilities/clog.h>
#include <utilities/test_fixture.h>

namespace {

static const char *cCLoggerName{"TestClog"};

class TestClog : public qle::TestFixture {
 protected:
  void SetUp() { qle::CLogger::set_log_level(qle::LogLevel::TRACE); }
  void TearDown() { qle::CLogger::set_log_level(qle::LogLevel::TRACE); }

  std::mutex &mtx_ = qle::TestFixture::mtx_;
};

TEST_F(TestClog, SetLogLevel) {
  std::array<int, 4> invalid_levels{-100, -1, qle::LogLevel::DISABLED + 1,
                                    qle::LogLevel::DISABLED + 100};
  std::array<int, 6> valid_levels{
      qle::LogLevel::TRACE,   qle::LogLevel::DEBUG, qle::LogLevel::INFO,
      qle::LogLevel::WARNING, qle::LogLevel::ERROR, qle::LogLevel::DISABLED};

  for (auto &level : invalid_levels) {
    ASSERT_FALSE(qle::CLogger::set_log_level(qle::LogLevel::Level(level)));
  }

  for (auto &level : valid_levels) {
    ASSERT_TRUE(qle::CLogger::set_log_level(qle::LogLevel::Level(level)));
  }
}

TEST_F(TestClog, LogVariousLevels) {
  std::lock_guard<std::mutex> guard(mtx_);

  auto logger = std::make_unique<qle::CLogger>(cCLoggerName);
  const char *msg{"Sample text"};

  {
    auto out =
        capture_output([&](const char *msg) { logger->trace(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "trace", cCLoggerName, msg);
    EXPECT_EQ(out["stdout"], buff);
  }
  {
    auto out =
        capture_output([&](const char *msg) { logger->debug(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "debug", cCLoggerName, msg);
    EXPECT_EQ(out["stdout"], buff);
  }
  {
    auto out = capture_output([&](const char *msg) { logger->info(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "info", cCLoggerName, msg);
    EXPECT_EQ(out["stdout"], buff);
  }
  {
    auto out = capture_output([&](const char *msg) { logger->warn(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "warn", cCLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }
  {
    auto out =
        capture_output([&](const char *msg) { logger->error(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "error", cCLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }
}

TEST_F(TestClog, LogMultipleTimes) {
  std::lock_guard<std::mutex> guard(mtx_);

  auto logger = std::make_unique<qle::CLogger>(cCLoggerName);
  const char *msg{"Sample text"};

  for (size_t i = 0; i < 10; i++) {
    char modified_msg[100]{};
    snprintf(modified_msg, sizeof(modified_msg), "%s %zu", msg, i);

    // Check log to stdout
    auto out = capture_output([&](const char *msg) { logger->info(msg); },
                              modified_msg);
    char buff_out[1024]{};
    snprintf(buff_out, sizeof(buff_out), "[%s] %s: %s\n", "info", cCLoggerName,
             modified_msg);
    EXPECT_EQ(out["stdout"], buff_out);

    // Check log to stderr
    out = capture_output([&](const char *msg) { logger->error(msg); },
                         modified_msg);
    char buff_err[1024]{};
    snprintf(buff_err, sizeof(buff_err), "[%s] %s: %s\n", "error", cCLoggerName,
             modified_msg);
    EXPECT_EQ(out["stderr"], buff_err);
  }
}

TEST_F(TestClog, LogHigherLevel) {
  std::lock_guard<std::mutex> guard(mtx_);

  qle::CLogger::set_log_level(qle::LogLevel::WARNING);
  auto logger = std::make_unique<qle::CLogger>(cCLoggerName);
  const char *msg{"Sample text"};

  /// If log level is WARNING, only log_warn() & log_error() output to stderr
  {
    // Expect NO log for log_trace, log_debug, log_info
    auto out =
        capture_output([&](const char *msg) { logger->trace(msg); }, msg);
    EXPECT_EQ(out["stdout"], "");

    out = capture_output([&](const char *msg) { logger->debug(msg); }, msg);
    EXPECT_EQ(out["stdout"], "");

    out = capture_output([&](const char *msg) { logger->info(msg); }, msg);
    EXPECT_EQ(out["stdout"], "");

    // Expect log_warn and log_error return non-empty
    out = capture_output([&](const char *msg) { logger->warn(msg); }, msg);
    char buff_warn[1024]{};
    snprintf(buff_warn, sizeof(buff_warn), "[%s] %s: %s\n", "warn",
             cCLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff_warn);

    out = capture_output([&](const char *msg) { logger->error(msg); }, msg);
    char buff_error[1024]{};
    snprintf(buff_error, sizeof(buff_error), "[%s] %s: %s\n", "error",
             cCLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff_error);
  }

  /// If log level is ERROR, only log_error() outputs to stderr
  qle::CLogger::set_log_level(qle::LogLevel::ERROR);
  {
    // Expect NO Log warn returned
    auto out = capture_output([&](const char *msg) { logger->warn(msg); }, msg);
    EXPECT_EQ(out["stderr"], "");

    // Expect Log error returned
    out = capture_output([&](const char *msg) { logger->error(msg); }, msg);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), "[%s] %s: %s\n", "error", cCLoggerName, msg);
    EXPECT_EQ(out["stderr"], buff);
  }

  /// If log level is DISABLED, no log to stderr / stdout is active
  qle::CLogger::set_log_level(qle::LogLevel::DISABLED);
  {
    // Expect NO Log error returned
    auto out =
        capture_output([&](const char *msg) { logger->error(msg); }, msg);
    EXPECT_EQ(out["stderr"], "");
  }
}

TEST_F(TestClog, LogComplexFormat) {
  std::lock_guard<std::mutex> guard(mtx_);

  auto logger = std::make_unique<qle::CLogger>(cCLoggerName);
  int tmp = 0;
  const char *format{"{} {} {} {}"};
  const char *expected_format{"[%s] %s: %s %zu %d %p\n"};

  const char *msg{"Sample text"};
  const size_t size_var = 1;
  const int int_var = -2;

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, void *ptr) {
          logger->trace(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "trace", cCLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stdout"], buff);
  }

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, void *ptr) {
          logger->debug(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "debug", cCLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stdout"], buff);
  }

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, void *ptr) {
          logger->info(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "info", cCLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stdout"], buff);
  }

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, void *ptr) {
          logger->warn(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "warn", cCLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stderr"], buff);
  }

  {
    auto out = capture_output(
        [&](const char *msg, size_t size_var, int int_var, void *ptr) {
          logger->error(format, msg, size_var, int_var, ptr);
        },
        msg, size_var, int_var, &tmp);
    char buff[1024]{};
    snprintf(buff, sizeof(buff), expected_format, "error", cCLoggerName, msg,
             size_var, int_var, &tmp);
    EXPECT_EQ(out["stderr"], buff);
  }
}

}  // namespace
