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
 * @brief ThreadA inherrited from Thread class
 */
class ThreadA : public BaseThread {
 public:
  explicit ThreadA(const char *name, Counter &counter, int counter_limit)
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

/**
 * @brief ThreadB inherrited from Thread class
 */
class ThreadB : public BaseThread {
 public:
  explicit ThreadB(const char *name, Counter &counter, int counter_limit)
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

TEST_F(TestThread, MultiThreads) {
  Counter counter;

  std::array<std::unique_ptr<BaseThread>, 2> threads;
  threads.at(0) = std::make_unique<ThreadA>("ThreadA", counter, 100);
  threads.at(1) = std::make_unique<ThreadB>("ThreadB", counter, 200);

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
  class DerivedThread : public BaseThread {
   public:
    DerivedThread() : BaseThread("BaseThread") {}

    bool run_triggered() const { return (count_ == 500); };

   protected:
    void run() override { count_ = 500; }
    int count_ = 0;
  };

  {
    // Fail to use
    auto thread = std::make_unique<DerivedThread>();
    ASSERT_FALSE(thread->running());
    thread->deinit();
    EXPECT_FALSE(thread->run_triggered());
  }
  {
    // Correct usage
    auto thread = std::make_unique<DerivedThread>();
    thread->init();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_TRUE(thread->running());
    thread->deinit();
    EXPECT_TRUE(thread->run_triggered());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_FALSE(thread->running());
  }

  {
    class UnnamedThread : public BaseThread {
     public:
      UnnamedThread() : BaseThread() {}

      bool run_triggered() const { return (count_ == 10); };

     protected:
      void run() override { count_ = 10; }
      int count_ = 0;
    };

    // Still work properly
    auto thread = std::make_unique<UnnamedThread>();
    thread->init();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_TRUE(thread->running());
    thread->deinit();
    EXPECT_TRUE(thread->run_triggered());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_FALSE(thread->running());
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
