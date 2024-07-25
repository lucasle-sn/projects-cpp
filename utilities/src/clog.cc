#include <utilities/clog.h>

namespace qle {

// Set default log level
CLogger::LogLevel CLogger::log_level_ = CLogger::LogLevel::INFO;
std::mutex CLogger::mutex_log_level_;

}  // namespace qle
