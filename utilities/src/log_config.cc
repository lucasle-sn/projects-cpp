#include <utilities/log_config.h>

namespace qle {

LoggerConfig * LoggerConfig::instance_{nullptr};
std::mutex LoggerConfig::instance_mtx_;

}  // namespace qle
