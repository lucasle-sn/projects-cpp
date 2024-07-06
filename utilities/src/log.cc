#include <utilities/log.h>

namespace qle {

// Set default log level
Logger::LogLevel Logger::logLevel_ = Logger::LogLevel::INFO;
std::mutex Logger::mutex_log_level_;

}  // namespace qle
