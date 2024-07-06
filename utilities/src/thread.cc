#include <utilities/thread.h>

#include <utilities/log.h>

#include <cstring>
#include <memory>

static auto logger = std::make_unique<qle::Logger>("Thread");

namespace qle {

void Thread::init() noexcept {
  thread_ = std::thread([this]() {
    if ((thread_name_ != nullptr) && (strlen(thread_name_) != 0)) {
      if (pthread_setname_np(thread_.native_handle(), thread_name_) != 0) {
        logger->error("Fail to set up thread name \"%s\"", thread_name_);
        running_ = false;
        return;
      }
    }
    running_ = true;
    logger->debug("Start thread %s", thread_name_);
    run();
    logger->debug("End thread %s", thread_name_);
  });
}

void Thread::deinit() noexcept {
  running_ = false;
  if (thread_.joinable()) {
    logger->debug("Joining thread %s", thread_name_);
    thread_.join();
  } else {
    logger->debug("Thread %s is not joinable", thread_name_);
  }
}

}  // namespace qle
