#ifndef UTILITIES_CLOG_H
#define UTILITIES_CLOG_H

#include <fmt/core.h>
#include <fmt/format.h>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <mutex>

namespace qle {

/**
 * @brief CLogger class
 */
class CLogger {
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
  CLogger() = delete;

  /**
   * @brief Copy constructor deleted
   */
  CLogger(const CLogger &) = delete;

  /**
   * @brief Move constructor deleted
   */
  CLogger(CLogger &&) = delete;

  /**
   * @brief Copy assignment deleted
   */
  CLogger &operator=(const CLogger &) = delete;

  /**
   * @brief Move assignment deleted
   */
  CLogger &operator=(CLogger &&) = delete;

  /**
   * @brief Construct a new CLogger object
   *
   * @param logger_name CLogger name
   */
  explicit CLogger(const char *logger_name) noexcept
      : logger_name_(logger_name) {}

  /**
   * @brief Destroy the CLogger object
   */
  ~CLogger() = default;

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
   * @param args Follow-up arguments
   */
  template <typename... Args>
  void trace(const char *format, Args... args) noexcept {
    log(LogLevel::TRACE, format, args...);
  }

  /**
   * @brief Log debug to stdout
   *
   * @param format Format
   * @param args Follow-up arguments
   */
  template <typename... Args>
  void debug(const char *format, Args... args) noexcept {
    log(LogLevel::DEBUG, format, args...);
  }

  /**
   * @brief Log info to stdout
   *
   * @param format Format
   * @param args Follow-up arguments
   */
  template <typename... Args>
  void info(const char *format, Args... args) noexcept {
    log(LogLevel::INFO, format, args...);
  }

  /**
   * @brief Log warning to stderr
   *
   * @param format Format
   * @param args Follow-up arguments
   */
  template <typename... Args>
  void warn(const char *format, Args... args) noexcept {
    log(LogLevel::WARNING, format, args...);
  }

  /**
   * @brief Log error to stderr
   *
   * @param format Format
   * @param args Follow-up arguments
   */
  template <typename... Args>
  void error(const char *format, Args... args) noexcept {
    log(LogLevel::ERROR, format, args...);
  }

 private:
  /**
   * @brief Log data
   *
   * @param level Log level
   * @param format Format
   * @param args Follow-up arguments
   */
  template <typename... Args>
  void log(LogLevel level, const char *format, Args... args) noexcept {
    if ((level < log_level_) || (level >= LogLevel::DISABLED)) {
      return;
    }

    std::string full_log_msg =
        fmt::format("[{}] {}: {}", log_level_to_string(level), logger_name_,
                    fmt::format(format, args...));

    std::lock_guard<std::mutex> guard(mutex_log_);
    switch (level) {
      case LogLevel::TRACE:
      case LogLevel::DEBUG:
      case LogLevel::INFO:
        std::cout << full_log_msg << std::endl;
        break;
      case LogLevel::WARNING:
      case LogLevel::ERROR:
        std::cerr << full_log_msg << std::endl;
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

#endif  // UTILITIES_CLOG_H
