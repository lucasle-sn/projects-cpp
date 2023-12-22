#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <thread>
#include <vector>

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
   * @brief Destroy the Thread object
   */
  virtual ~Thread() = default;

  /**
   * @brief Initialize Thread object elements
   */
  void init() { thread = std::thread(&Thread::run, this); }

  /**
   * @brief Deinitialize Thread object elements
   */
  void deinit() {
    if (thread.joinable()) {
      thread.join();
    }
  }

 private:
  /**
   * @brief Thread that runs run()
   */
  std::thread thread;

  /**
   * @brief Run Thread
   */
  virtual void run() = 0;
};

/**
 * @brief ThreadA inherrited from Thread class
 */
class ThreadA : public Thread {
 private:
  /**
   * @brief Override run()
   */
  void run() override {
    for (size_t i = 0; i < 10; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      printf("Thread A%zu\n", i);
    }
  }
};

/**
 * @brief ThreadB inherrited from Thread class
 */
class ThreadB : public Thread {
 private:
  /**
   * @brief Override run() function
   */
  void run() override {
    for (size_t i = 0; i < 10; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      printf("Thread B%zu\n", i);
    }
  }
};

int main() {
  std::vector<std::unique_ptr<Thread>> objects;

  objects.push_back(std::make_unique<ThreadA>());
  objects.push_back(std::make_unique<ThreadB>());

  for (auto &obj : objects) {
    obj->init();
  }

  for (auto &obj : objects) {
    obj->deinit();
  }

  return EXIT_SUCCESS;
}
