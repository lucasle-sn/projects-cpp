#ifndef UTILITIES_THREAD_H
#define UTILITIES_THREAD_H

#include <atomic>
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
   * @brief Construct a new Thread object
   *
   * @param thread_name Name of thread
   */
  explicit Thread(const char *thread_name) noexcept
      : thread_name_(thread_name){};

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
  void init() noexcept;

  /**
   * @brief Deinitialize Thread object elements
   */
  void deinit() noexcept;

  /**
   * @brief Get running status of thread
   *
   * @return true/false
   */
  bool running() const noexcept { return running_; }

 protected:
  /**
   * @brief Run Thread
   */
  virtual void run() = 0;

 private:
  std::thread thread_;                ///< Main thread
  const char *thread_name_;           ///< Name of thread
  std::atomic<bool> running_{false};  ///< Running status
};

}  // namespace qle

#endif  // UTILITIES_THREAD_H
