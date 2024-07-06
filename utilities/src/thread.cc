#include <utilities/thread.h>

#include <utilities/log.h>

#include <cstring>
#include <memory>

static auto logger = std::make_unique<qle::Logger>("Thread");

namespace qle {

void Thread::init() noexcept {
  thread_ = std::thread([this]() {
    running_ = true;
    if ((thread_name_ != nullptr) && (strlen(thread_name_) != 0)) {
      pthread_setname_np(thread_.native_handle(), thread_name_);
    }

    logger->info("Start thread %s", thread_name_);
    run();
    logger->info("End thread %s", thread_name_);
  });
}

void Thread::deinit() noexcept {
  running_ = false;
  if (thread_.joinable()) {
    logger->info("Joining thread %s", thread_name_);
    thread_.join();
  } else {
    logger->error("Thread %s is not joinable", thread_name_);
  }
}

}  // namespace qle
