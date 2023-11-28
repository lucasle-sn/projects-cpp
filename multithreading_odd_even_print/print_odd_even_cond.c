#include<stdio.h>
#include<pthread.h>
#include <unistd.h>
#include <stdbool.h>

const int MAX = 10;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;

void *print_odd(void *arg) {
    bool is_count_available = true;
    while (is_count_available) {
        pthread_mutex_lock(&condition_mutex);
        int *count = (int *) arg;
        while (*count % 2 != 1 && *count < MAX) {
            pthread_cond_wait(&condition_cond, &condition_mutex);
        }
        pthread_mutex_unlock(&condition_mutex);

        pthread_mutex_lock(&mutex);
        is_count_available = (*count < MAX);

        if (!is_count_available) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        printf("Odd: %d\n", (*count)++);

        pthread_cond_signal(&condition_cond);
        pthread_mutex_unlock(&mutex);
    }
}

void *print_even(void *arg) {
    bool is_count_available = true;
    while (is_count_available) {
        pthread_mutex_lock(&condition_mutex);
        int *count = (int *) arg;
        while (*count % 2 != 0 && *count < MAX) {
            pthread_cond_wait(&condition_cond, &condition_mutex);
        }
        pthread_mutex_unlock(&condition_mutex);

        pthread_mutex_lock(&mutex);
        is_count_available = (*count < MAX);

        if (!is_count_available) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        printf("Even: %d\n", (*count)++);

        pthread_cond_signal(&condition_cond);
        pthread_mutex_unlock(&mutex);
    }
}


int main() {
    pthread_t thread_odd, thread_even;

    int count = 0;

    pthread_create(&thread_even, NULL, &print_even, &count);
    pthread_create(&thread_odd, NULL, &print_odd, &count);

    pthread_join(thread_even, NULL);
    pthread_join(thread_odd, NULL);

    pthread_cond_destroy(&condition_cond);
    pthread_mutex_destroy(&condition_mutex);
    pthread_mutex_destroy(&mutex);

    return 0;
}
