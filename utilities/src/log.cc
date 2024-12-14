#include <utilities/log.h>

namespace qle {

void Logger::log(LogLevel::Level level, const char *format,
                 va_list args) noexcept {
  if (!LogLevel::is_valid_log_level(level)) {
    return;
  }

  if (level < logger_config_->loglevel()) {
    return;
  }

  char log_msg[1024]{};
  vsnprintf(log_msg, sizeof(log_msg), format, args);

  char full_log_msg[1029]{};
  snprintf(full_log_msg, sizeof(full_log_msg), "[%s] %s: %s",
           LogLevel::log_level_to_string(level), logger_name_, log_msg);

  std::lock_guard<std::mutex> guard(logger_config_->logmutex);
  if (logger_config_->logfile()) {
    if (level != LogLevel::DISABLED) {
      fprintf(logger_config_->logfile(), "%s\n", full_log_msg);
    }
    return;
  }

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

}  // namespace qle
