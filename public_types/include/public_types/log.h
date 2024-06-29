#ifndef PUBLIC_TYPES_LOG_H
#define PUBLIC_TYPES_LOG_H

namespace unimelb {

#include <cstdio>
#include <memory>
#include <mutex>
#include <thread>

/**
 * @brief Logger class
 */
class Logger {
 public:
  /**
   * @brief LogLevel enum class
   */
  enum class LogLevel { INFO, WARNING, ERROR };

  /**
   * @brief Constructor deleted
   */
  Logger() = default;

  /**
   * @brief Copy constructor deleted
   */
  Logger(const Logger &) = default;

  /**
   * @brief Move constructor deleted
   */
  Logger(Logger &&) = default;

  /**
   * @brief Copy assignment deleted
   */
  Logger &operator=(const Logger &) = default;

  /**
   * @brief Move assignment deleted
   */
  Logger &operator=(Logger &&) = default;

  /**
   * @brief Constructor
   *
   * @param loggerName Logger name
   * @param level Log level
   */
  explicit Logger(const char *loggerName, LogLevel level = LogLevel::INFO)
      : loggerName_(loggerName), logLevel_(level) {}

  /**
   * @brief Log info
   *
   * @param message Log message
   */
  void info(const char *message) { log(LogLevel::INFO, message); }

  /**
   * @brief Log warn
   *
   * @param message Log message
   */
  void warn(const char *message) { log(LogLevel::WARNING, message); }

  /**
   * @brief Log error
   *
   * @param message Log message
   */
  void error(const char *message) { log(LogLevel::ERROR, message); }

 private:
  /**
   * @brief Log data
   *
   * @param level Log level
   * @param message Log message
   */
  void log(LogLevel level, const char *message) {
    if (level < logLevel_) {
      return;
    }

    std::lock_guard<std::mutex> guard(mutex_);
    char logMessage[1024]{};
    snprintf(logMessage, sizeof(logMessage), "[%s] %s: %s",
             logLevelToString(level), loggerName_, message);

    if (level == LogLevel::ERROR) {
      fprintf(stderr, "%s\n", logMessage);
    } else {
      fprintf(stdout, "%s\n", logMessage);
    }
  }

  /**
   * @brief Convert llog level to string
   *
   * @param level Log level
   * @return const char*
   */
  const char *logLevelToString(LogLevel level) {
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

#endif  // PUBLIC_TYPES_LOG_H
