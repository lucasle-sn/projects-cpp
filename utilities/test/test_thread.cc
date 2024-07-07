#include <utilities/thread.h>

#include <gtest/gtest.h>
#include <array>

using BaseThread = qle::Thread;

namespace {

class TestThread : public ::testing::Test {};

/// Class Counter as shared resource
class Counter {
 public:
  void increment() {
    std::lock_guard<std::mutex> guard(mtx_);
    ++count_;
  }

  int get_count() const { return count_; }

 private:
  int count_ = 0;
  mutable std::mutex mtx_;
};

/**
 * @brief CounterThread inherrited from Thread class
 */
class CounterThread : public BaseThread {
 public:
  explicit CounterThread(const char *name, Counter &counter, int counter_limit)
      : BaseThread(name), counter_(counter), counter_limit_(counter_limit) {}

 private:
  void run() override {
    for (size_t i = 0; i < counter_limit_; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      counter_.increment();
    }
  }

  Counter &counter_;
  int counter_limit_ = 0;
};

class TestingThread : public BaseThread {
 public:
  explicit TestingThread(const char *name, int expected_count = 100)
      : BaseThread(name), expected_count_(expected_count) {}
  explicit TestingThread(int expected_count)
      : BaseThread(), expected_count_(expected_count) {}

  /// Check whether run() function is triggered
  bool run_triggered() const { return (count_ == expected_count_); };

 private:
  void run() override { count_ = expected_count_; }

  int count_ = 0;
  int expected_count_ = -1;
};

TEST_F(TestThread, MultiThreads) {
  Counter counter;

  std::array<std::unique_ptr<BaseThread>, 2> threads;
  threads.at(0) = std::make_unique<CounterThread>("ThreadA", counter, 100);
  threads.at(1) = std::make_unique<CounterThread>("ThreadB", counter, 200);

  for (auto &thread : threads) {
    thread->init();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  ASSERT_TRUE(threads[0]->running());
  ASSERT_TRUE(threads[1]->running());

  for (auto &thread : threads) {
    thread->deinit();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  ASSERT_FALSE(threads[0]->running());
  ASSERT_FALSE(threads[1]->running());

  EXPECT_EQ(counter.get_count(), 300);
}

TEST_F(TestThread, Usage) {
  {
    // Fail to use
    auto thread = std::make_unique<TestingThread>("Thread", 101);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_FALSE(thread->running());
    thread->deinit();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_FALSE(thread->run_triggered());
  }

  {
    // Correct usage with named thread, unnamed thread & nullptr named thread
    std::array<std::unique_ptr<TestingThread>, 3> threads;
    threads[0] = std::make_unique<TestingThread>("NamedThread", 300);  // Named
    threads[1] = std::make_unique<TestingThread>(222);          // Unnamed
    threads[2] = std::make_unique<TestingThread>(nullptr, -1);  // nullptr

    for (auto &thread : threads) {
      thread->init();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      ASSERT_TRUE(thread->running());
      thread->deinit();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      EXPECT_TRUE(thread->run_triggered());
      ASSERT_FALSE(thread->running());
    }
  }

  {
    class LengthyNameThread : public BaseThread {
     public:
      LengthyNameThread()
          : BaseThread("This Thread has a really long long name") {}

     protected:
      void run() override{};
    };

    // Should have return error log for lengthy name
    auto thread = std::make_unique<LengthyNameThread>();
    thread->init();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_FALSE(thread->running());
    thread->deinit();
  }
}

}  // namespace
