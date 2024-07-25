#include <utilities/log.h>

namespace qle {

// Set default log level
LogLevel::Level Logger::log_level_ = LogLevel::INFO;
std::mutex Logger::mutex_log_level_;

}  // namespace qle
