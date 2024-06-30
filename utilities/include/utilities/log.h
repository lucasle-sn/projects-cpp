#ifndef UTILITIES_LOG_H
#define UTILITIES_LOG_H

#include <cstdio>
#include <mutex>

namespace unimelb {

/**
 * @brief Logger class
 */
class Logger {
 public:
  /**
   * @brief LogLevel enum
   */
  enum LogLevel { INFO = 0, WARNING, ERROR };

  /**
   * @brief Constructor deleted
   */
  Logger() = delete;

  /**
   * @brief Copy constructor deleted
   */
  Logger(const Logger &) = delete;

  /**
   * @brief Move constructor deleted
   */
  Logger(Logger &&) = delete;

  /**
   * @brief Copy assignment deleted
   */
  Logger &operator=(const Logger &) = delete;

  /**
   * @brief Move assignment deleted
   */
  Logger &operator=(Logger &&) = delete;

  /**
   * @brief Construct a new Logger object
   *
   * @param loggerName Logger name
   * @param level Log level
   */
  explicit Logger(const char *loggerName,
                  LogLevel level = LogLevel::INFO) noexcept
      : loggerName_(loggerName), logLevel_(level) {}

  /**
   * @brief Destroy the Logger object
   */
  ~Logger() = default;

  /**
   * @brief Log info to stdout
   *
   * @param message Log message
   */
  void info(const char *message) noexcept { log(LogLevel::INFO, message); }

  /**
   * @brief Log warning to stderr
   *
   * @param message Log message
   */
  void warn(const char *message) noexcept { log(LogLevel::WARNING, message); }

  /**
   * @brief Log error to stderr
   *
   * @param message Log message
   */
  void error(const char *message) noexcept { log(LogLevel::ERROR, message); }

 private:
  /**
   * @brief Log data
   *
   * @param level Log level
   * @param message Log message
   */
  void log(LogLevel level, const char *message) noexcept {
    if (level < logLevel_) {
      return;
    }

    std::lock_guard<std::mutex> guard(mutex_);
    char logMessage[1024]{};
    snprintf(logMessage, sizeof(logMessage), "[%s] %s: %s",
             logLevelToString(level), loggerName_, message);

    if (level == LogLevel::INFO) {
      fprintf(stdout, "%s\n", logMessage);
    } else {
      fprintf(stderr, "%s\n", logMessage);
    }
  }

  /**
   * @brief Convert log level to string
   *
   * @param level Log level
   * @return const char*
   */
  const char *logLevelToString(LogLevel level) noexcept {
    switch (level) {
      case LogLevel::INFO:
        return "info";
      case LogLevel::WARNING:
        return "warn";
      case LogLevel::ERROR:
      default:
        return "error";
    }
  }

  const char *loggerName_;  ///< Logger name
  LogLevel logLevel_;       ///< Log level
  std::mutex mutex_;        ///< Mutex
};

}  // namespace unimelb

#endif  // UTILITIES_LOG_H
