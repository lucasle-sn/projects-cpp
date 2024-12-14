#ifndef UTILITIES_LOG_CONFIG_H
#define UTILITIES_LOG_CONFIG_H

#include <cstdio>
#include <memory>
#include <mutex>

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

/**
 * @brief Logger config
 *
 * It is recommended to not call LoggerConfig directly. Please use
 * LoggerConfigHandler.
 */
class LoggerConfig {
 public:
  /**
   * @brief Constructor deleted
   */
  LoggerConfig() = delete;
  /**
   * @brief Copy constructor deleted
   */
  LoggerConfig(const LoggerConfig &) = delete;
  /**
   * @brief Move constructor deleted
   */
  LoggerConfig(LoggerConfig &&) = delete;
  /**
   * @brief Copy assignment deleted
   */
  LoggerConfig operator=(const LoggerConfig &) = delete;
  /**
   * @brief Move assignment deleted
   */
  LoggerConfig operator=(LoggerConfig &&) = delete;

  /**
   * @brief Create an instance of LoggerConfig
   *
   * @param loglevel Log level
   * @param logfile Log file
   *
   * @return An instance of LoggerConfig
   */
  static LoggerConfig *create(LogLevel::Level loglevel = LogLevel::INFO,
                              const char *logfile = nullptr) {
    if (!LogLevel::is_valid_log_level(loglevel)) {
      return nullptr;
    }
    std::lock_guard<std::mutex> lock(instance_mtx_);
    if (!instance_) {
      instance_ = new LoggerConfig(loglevel, logfile);
    }
    return instance_;
  }

  /**
   * @brief Destroy an instance of LoggerConfig
   */
  static void destroy() {
    if (!instance_) {
      return;
    }
    std::lock_guard<std::mutex> lock(instance_mtx_);
    if (instance_->logfile_) {
      fclose(instance_->logfile());
    }
    delete instance_;
    instance_ = nullptr;
  }

  /**
   * @brief LoggerConfig singleton instance
   *
   * @return LoggerConfig*
   */
  static LoggerConfig *instance() { return instance_; }

  /**
   * @brief Log level getter
   *
   * @return LogLevel::Level
   */
  static LogLevel::Level loglevel() { return instance_->loglevel_; }

  /**
   * @brief Log file getter
   *
   * @return FILE *
   */
  static FILE *logfile() { return instance_->logfile_; }

  std::mutex logmutex;  ///< logging mutex

 private:
  /**
   * @brief Construct LoggerConfig object
   *
   * @param loglevel Log level
   * @param logfile Log file
   */
  explicit LoggerConfig(LogLevel::Level loglevel = LogLevel::INFO,
                        const char *logfile = nullptr)
      : loglevel_(loglevel) {
    if (logfile) {
      logfile_ = fopen(logfile, "a");
    }
  }

  /**
   * @brief Default destructor
   */
  ~LoggerConfig() = default;

  static LoggerConfig *instance_;   ///< LoggerConfig instance
  static std::mutex instance_mtx_;  ///< LoggerConfig instance mutex

  LogLevel::Level loglevel_{LogLevel::INFO};  ///< Log level
  FILE *logfile_{nullptr};                    ///< Log file ptr
};

/**
 * @brief Wrapper for LoggerConfig
 */
class LoggerConfigHandler {
 public:
  /**
   * @brief Create an LoggerConfigHandler object
   *
   * @param loglevel Log level
   * @param logfile Log file
   */
  LoggerConfigHandler(LogLevel::Level loglevel = LogLevel::INFO,
                      const char *logfile = nullptr) {
    config_ = qle::LoggerConfig::create(loglevel, logfile);
  }

  /**
   * @brief Destroy object of LoggerConfigHandler
   */
  ~LoggerConfigHandler() { qle::LoggerConfig::destroy(); }

  /**
   * @brief Get logger config  ptr
   *
   * @return LoggerConfig *
   */
  LoggerConfig *get_config() const { return config_; }

 private:
  LoggerConfig *config_{nullptr};  ///< Logger config
};

}  // namespace qle

#endif  // UTILITIES_LOG_CONFIG_H
