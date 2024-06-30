#include <utilities/log.h>
#include <utilities/thread.h>

#include <gtest/gtest.h>

namespace {

// Default sleep duration of thread
static constexpr uint8_t cThreadSleepDurationMs{2};

const std::string cThreadAName{"Thread A"};
const std::string cThreadBName{"Thread B"};

/**
 * @brief ThreadA inherrited from Thread class
 */
class ThreadA : public qle::Thread {
 private:
  /**
   * @brief Override run()
   */
  void run() override {
    for (size_t i = 0; i < 10; i++) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(cThreadSleepDurationMs));
      fprintf(stdout, "%s %zu\n", cThreadAName.c_str(), i);
    }
  }
};

/**
 * @brief ThreadB inherrited from Thread class
 */
class ThreadB : public qle::Thread {
 private:
  /**
   * @brief Override run() function
   */
  void run() override {
    for (size_t i = 10; i < 20; i++) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(cThreadSleepDurationMs));
      fprintf(stderr, "%s %zu\n", cThreadBName.c_str(), i);
    }
  }
};

class TestThread : public ::testing::Test {};

TEST_F(TestThread, MultiThread) {
  std::array<std::unique_ptr<qle::Thread>, 2> objects;

  objects.at(0) = std::make_unique<ThreadA>();
  objects.at(1) = std::make_unique<ThreadB>();

  for (auto &obj : objects) {
    obj->init();
  }

  ::testing::internal::CaptureStdout();
  ::testing::internal::CaptureStderr();

  for (auto &obj : objects) {
    obj->deinit();
  }

  auto stdout_output = ::testing::internal::GetCapturedStdout();
  auto stderr_output = ::testing::internal::GetCapturedStderr();

  std::string expected_stdout;
  std::string expected_stderr;
  for (size_t i = 0; i < 10; i++) {
    expected_stdout += (cThreadAName + " " + std::to_string(i) + "\n");
  }
  for (size_t i = 10; i < 20; i++) {
    expected_stderr += (cThreadBName + " " + std::to_string(i) + "\n");
  }

  EXPECT_EQ(stdout_output, expected_stdout);
  EXPECT_EQ(stderr_output, expected_stderr);
}

}  // namespace
