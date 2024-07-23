#ifndef UTILITIES_LOG_H
#define UTILITIES_LOG_H

#include <cstdarg>
#include <cstdio>
#include <mutex>

namespace qle {

/**
 * @brief Logger class
 */
class Logger {
 public:
  /**
   * @brief LogLevel enum
   */
  enum LogLevel {
    TRACE = 0,  ///< Trace
    DEBUG,      ///< Debug
    INFO,       ///< Info
    WARNING,    ///< Warning
    ERROR,      ///< Error
    DISABLED    ///< Disabled
  };

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
   * @param logger_name Logger name
   */
  explicit Logger(const char *logger_name) noexcept
      : logger_name_(logger_name) {}

  /**
   * @brief Destroy the Logger object
   */
  ~Logger() = default;

  /**
   * @brief Set the log level
   *
   * @param logLevel log level
   */
  static void set_log_level(LogLevel logLevel) {
    std::lock_guard<std::mutex> guard(mutex_log_level_);
    logLevel_ = logLevel;
  }

  /**
   * @brief Log trace to stdout
   *
   * @param format Format
   * @param ...
   */
  void trace(const char *format, ...) noexcept {
    va_list args;
    va_start(args, format);
    log(LogLevel::TRACE, format, args);
    va_end(args);
  }

  /**
   * @brief Log debug to stdout
   *
   * @param format Format
   * @param ...
   */
  void debug(const char *format, ...) noexcept {
    va_list args;
    va_start(args, format);
    log(LogLevel::DEBUG, format, args);
    va_end(args);
  }

  /**
   * @brief Log info to stdout
   *
   * @param format Format
   * @param ...
   */
  void info(const char *format, ...) noexcept {
    va_list args;
    va_start(args, format);
    log(LogLevel::INFO, format, args);
    va_end(args);
  }

  /**
   * @brief Log warning to stderr
   *
   * @param format Format
   * @param ...
   */
  void warn(const char *format, ...) noexcept {
    va_list args;
    va_start(args, format);
    log(LogLevel::WARNING, format, args);
    va_end(args);
  }

  /**
   * @brief Log error to stderr
   *
   * @param format Format
   * @param ...
   */
  void error(const char *format, ...) noexcept {
    va_list args;
    va_start(args, format);
    log(LogLevel::ERROR, format, args);
    va_end(args);
  }

 private:
  /**
   * @brief Log data
   *
   * @param level Log level
   * @param message Log message
   */
  void log(LogLevel level, const char *format, va_list args) noexcept {
    if ((level < logLevel_) || (level == LogLevel::DISABLED)) {
      return;
    }

    char logMessage[1024]{};
    vsnprintf(logMessage, sizeof(logMessage), format, args);

    char fullLogMessage[1024]{};
    snprintf(fullLogMessage, sizeof(fullLogMessage), "[%s] %s: %s",
             logLevelToString(level), logger_name_, logMessage);

    std::lock_guard<std::mutex> guard(mutex_log_);
    switch (level) {
      case LogLevel::TRACE:
      case LogLevel::DEBUG:
      case LogLevel::INFO:
        fprintf(stdout, "%s\n", fullLogMessage);
        break;
      case LogLevel::WARNING:
      case LogLevel::ERROR:
        fprintf(stderr, "%s\n", fullLogMessage);
        break;
      case LogLevel::DISABLED:
      default:
        return;
    }
  }

  /**
   * @brief Convert log level to string
   *
   * @param level Log level
   * @return const char*
   */
  const char *logLevelToString(LogLevel level) const noexcept {
    switch (level) {
      case LogLevel::TRACE:
        return "trace";
      case LogLevel::DEBUG:
        return "debug";
      case LogLevel::INFO:
        return "info";
      case LogLevel::WARNING:
        return "warn";
      case LogLevel::ERROR:
        return "error";
      case LogLevel::DISABLED:
        return "disabled";
      default:
        return nullptr;
    }
  }

  const char *logger_name_;            ///< Logger name
  std::mutex mutex_log_;               ///< Mutex logging
  static std::mutex mutex_log_level_;  ///< Mutex logLevel
  static LogLevel logLevel_;           ///< Log level
};

}  // namespace qle

#endif  // UTILITIES_LOG_H
