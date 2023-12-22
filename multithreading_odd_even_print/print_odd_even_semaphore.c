#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

const int MAX = 10;

sem_t mutex_odd, mutex_even;

void *print_odd(void *arg) {
  while (1) {
    sem_wait(&mutex_odd);
    int *count = (int *)arg;

    if (*count >= MAX) {
      sem_post(&mutex_even);
      break;
    }

    if (*count % 2 == 1) {
      printf("Odd: %d\n", (*count)++);
    }

    sem_post(&mutex_even);
  }
}

void *print_even(void *arg) {
  while (1) {
    sem_wait(&mutex_even);
    int *count = (int *)arg;

    if (*count >= MAX) {
      sem_post(&mutex_odd);
      break;
    }

    if (*count % 2 == 0) {
      printf("Even: %d\n", (*count)++);
    }

    sem_post(&mutex_odd);
  }

  return NULL;
}

int main() {
  pthread_t thread_odd, thread_even;

  sem_init(&mutex_odd, 0, 1);
  sem_init(&mutex_even, 0, 0);

  int count = 0;

  pthread_create(&thread_even, NULL, &print_even, &count);
  pthread_create(&thread_odd, NULL, &print_odd, &count);

  pthread_join(thread_even, NULL);
  pthread_join(thread_odd, NULL);

  return 0;
}
