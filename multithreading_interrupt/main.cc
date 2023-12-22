#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <atomic>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;

std::atomic<bool> status{false};
std::atomic<bool> runnable{true};

void change_status(const char *thread, bool signal) {
  printf("%s, change status: %s -> %s\n", thread, status ? "true" : "false",
         status ? "false" : "true");
  status = signal;
  printf("Signaling %s\n", signal ? "\"true\"" : "\"false\"");
  pthread_cond_signal(&condition_cond);
}

void set_status(const char *thread) { change_status(thread, true); }

void reset_status(const char *thread) { change_status(thread, false); }

bool get_status(const char *thread) {
  if (thread != NULL) {
    printf("%s: status = %s\n", thread, status ? "true" : "false");
  }
  return status;
}

void wait_until_status(bool signal) {
  pthread_mutex_lock(&condition_mutex);
  if (status != signal) {
    pthread_cond_wait(&condition_cond, &condition_mutex);
  }
  pthread_mutex_unlock(&condition_mutex);
}

void *run_main(void *arg) {
  (void)arg;
  const char *name = "MAIN thread";
  while (runnable) {
    printf("----- Main thread: ------>\n");
    set_status(name);

    wait_until_status(false);

    printf("%s: Doing something\n", name);
  }
  return nullptr;
}

void *run_interrupt(void *arg) {
  (void)arg;
  const char *name = "INTERRUPT thread";
  int count = 0;

  while (runnable) {
    wait_until_status(true);

    printf("%s: Doing something\n", name);
    reset_status(name);

    count++;
    if (count >= 5) {
      runnable = false;
    }
  }
  return nullptr;
}

int main() {
  pthread_t thread_main, thread_interrupt;

  pthread_create(&thread_main, NULL, &run_main, NULL);
  pthread_create(&thread_interrupt, NULL, &run_interrupt, NULL);

  pthread_join(thread_main, NULL);
  pthread_join(thread_interrupt, NULL);
}
