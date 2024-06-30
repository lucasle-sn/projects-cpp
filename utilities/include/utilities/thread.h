#ifndef UTILITIES_THREAD_H
#define UTILITIES_THREAD_H

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <thread>

namespace qle {

/**
 * @brief Base class defining Thread
 */
class Thread {
 public:
  /**
   * @brief Construct a new Thread object
   */
  Thread() = default;

  /**
   * @brief Copy constructor deleted
   */
  Thread(const Thread &) = delete;

  /**
   * @brief Move constructor deleted
   */
  Thread(Thread &&) = delete;

  /**
   * @brief Copy assignment deleted
   */
  Thread &operator=(const Thread &) = delete;

  /**
   * @brief Move assignment deleted
   */
  Thread &operator=(Thread &&) = delete;

  /**
   * @brief Destroy the Thread object
   */
  virtual ~Thread() = default;

  /**
   * @brief Initialize Thread object elements
   */
  void init() { thread_ = std::thread(&Thread::run, this); }

  /**
   * @brief Deinitialize Thread object elements
   */
  void deinit() {
    if (thread_.joinable()) {
      thread_.join();
    }
  }

 private:
  /**
   * @brief Run Thread
   */
  virtual void run() = 0;

  /**
   * @brief Thread that runs run()
   */
  std::thread thread_;
};

}  // namespace qle

#endif  // UTILITIES_THREAD_H
