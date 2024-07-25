#include <utilities/clog.h>

namespace qle {

// Set default log level
LogLevel::Level CLogger::log_level_ = LogLevel::INFO;
std::mutex CLogger::mutex_log_level_;

}  // namespace qle
