#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

const int MAX = 10;

pthread_mutex_t mutex_odd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_even = PTHREAD_MUTEX_INITIALIZER;

void *print_odd(void *arg) {
  while (1) {
    pthread_mutex_lock(&mutex_odd);
    int *count = (int *)arg;

    if (*count >= MAX) {
      pthread_mutex_unlock(&mutex_even);
      pthread_exit(NULL);
    }

    if (*count % 2 == 1) {
      printf("Odd: %d\n", (*count)++);
    }

    pthread_mutex_unlock(&mutex_even);
  }
}

void *print_even(void *arg) {
  while (1) {
    pthread_mutex_lock(&mutex_even);
    int *count = (int *)arg;

    if (*count >= MAX) {
      pthread_mutex_unlock(&mutex_odd);
      pthread_exit(NULL);
    }

    if (*count % 2 == 0) {
      printf("Even: %d\n", (*count)++);
    }

    pthread_mutex_unlock(&mutex_odd);
  }

  return NULL;
}

int main() {
  pthread_t thread_odd;
  pthread_t thread_even;

  int count = 0;

  pthread_create(&thread_even, NULL, &print_even, &count);
  pthread_create(&thread_odd, NULL, &print_odd, &count);

  pthread_join(thread_even, NULL);
  pthread_join(thread_odd, NULL);

  return 0;
}
