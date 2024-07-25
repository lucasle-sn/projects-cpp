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
   * @param log_level log level
   * @return true/false
   */
  static bool set_log_level(LogLevel log_level) {
    if ((log_level < LogLevel::TRACE) || (log_level > LogLevel::DISABLED)) {
      return false;
    }
    std::lock_guard<std::mutex> guard(mutex_log_level_);
    log_level_ = log_level;
    return true;
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
    if ((level < log_level_) || (level >= LogLevel::DISABLED)) {
      return;
    }

    char log_msg[1024]{};
    vsnprintf(log_msg, sizeof(log_msg), format, args);

    char full_log_msg[1024]{};
    snprintf(full_log_msg, sizeof(full_log_msg), "[%s] %s: %s",
             log_level_to_string(level), logger_name_, log_msg);

    std::lock_guard<std::mutex> guard(mutex_log_);
    switch (level) {
      case LogLevel::TRACE:
      case LogLevel::DEBUG:
      case LogLevel::INFO:
        fprintf(stdout, "%s\n", full_log_msg);
        break;
      case LogLevel::WARNING:
      case LogLevel::ERROR:
        fprintf(stderr, "%s\n", full_log_msg);
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
  const char *log_level_to_string(LogLevel level) const noexcept {
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
      default:
        return nullptr;
    }
  }

  const char *logger_name_;            ///< Logger name
  std::mutex mutex_log_;               ///< Mutex logging
  static std::mutex mutex_log_level_;  ///< Mutex log_level
  static LogLevel log_level_;          ///< Log level
};

}  // namespace qle

#endif  // UTILITIES_LOG_H
