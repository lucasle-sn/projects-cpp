#ifndef UTILITIES_LOG_H
#define UTILITIES_LOG_H

#include <cstdarg>
#include <cstdio>
#include <mutex>

#include <utilities/log_config.h>

namespace qle {

/**
 * @brief Logger class
 */
class Logger {
 public:
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
      : logger_name_(logger_name) {
    logger_config_ = LoggerConfig::create();
  }

  /**
   * @brief Destroy the Logger object
   */
  ~Logger() = default;

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
  void log(LogLevel::Level level, const char *format, va_list args) noexcept;

  const char *logger_name_;               ///< Logger name
  LoggerConfig *logger_config_{nullptr};  ///< Logger config
};

}  // namespace qle

#endif  // UTILITIES_LOG_H
