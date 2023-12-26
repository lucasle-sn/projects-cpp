#include <stdbool.h>
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <mutex>
#include <thread>

std::mutex mutex;
std::mutex condition_mutex;
std::condition_variable cv;

std::atomic<bool> status{false};
std::atomic<bool> runnable{true};

void change_status(const char *thread, bool signal) {
  printf("%s, change status: %s -> %s\n", thread, status ? "true" : "false",
         status ? "false" : "true");
  status = signal;
  cv.notify_one();
}

void set_status(const char *thread) { change_status(thread, true); }

void reset_status(const char *thread) { change_status(thread, false); }

bool get_status(const char *thread) {
  if (thread != nullptr) {
    printf("%s: status = %s\n", thread, status ? "true" : "false");
  }
  return status;
}

void wait_until_status(bool signal) {
  std::unique_lock<std::mutex> lock(condition_mutex);
  cv.wait(lock, [&signal] { return (status == signal); });
  condition_mutex.unlock();
}

void run_main(void *arg) {
  (void)arg;
  const char *name = "MAIN thread";
  while (runnable) {
    printf("----- Main thread: ------>\n");
    set_status(name);

    wait_until_status(false);
  }
}

void run_interrupt(void *arg) {
  (void)arg;
  const char *name = "INTERRUPT thread";
  int count = 0;

  while (runnable) {
    wait_until_status(true);

    reset_status(name);

    count++;
    runnable = count < 5;
    if (count >= 5) {
      runnable = false;
    }
  }
}

int main() {
  std::thread thread_main{run_main, nullptr};
  std::thread thread_interrupt{run_interrupt, nullptr};

  thread_main.join();
  thread_interrupt.join();
}
