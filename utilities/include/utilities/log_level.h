#ifndef UTILITIES_LOG_LEVEL_H
#define UTILITIES_LOG_LEVEL_H

namespace qle {

/**
 * @brief LogLevel class
 */
class LogLevel {
 public:
  /**
   * @brief Level enum
   */
  enum Level {
    TRACE = 0,  ///< Trace
    DEBUG,      ///< Debug
    INFO,       ///< Info
    WARNING,    ///< Warning
    ERROR,      ///< Error
    DISABLED    ///< Disabled
  };

  /**
   * @brief Check if log level is valid
   *
   * @param level Log level
   * @return true/false
   */
  static bool is_valid_log_level(Level level) noexcept {
    return ((level >= Level::TRACE) && (level <= Level::DISABLED));
  }

  /**
   * @brief Convert log level to string
   *
   * @param level Log level
   * @return const char*
   */
  static const char *log_level_to_string(Level level) noexcept {
    switch (level) {
      case Level::TRACE:
        return "trace";
      case Level::DEBUG:
        return "debug";
      case Level::INFO:
        return "info";
      case Level::WARNING:
        return "warn";
      case Level::ERROR:
        return "error";
      case Level::DISABLED:
      default:
        return nullptr;
    }
  }
};

}  // namespace qle

#endif  // UTILITIES_LOG_LEVEL_H
