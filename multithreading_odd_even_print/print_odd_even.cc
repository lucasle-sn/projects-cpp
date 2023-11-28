#include <thread>
#include <mutex>
#include <stdio.h>
#include <unistd.h>

std::mutex mtx_odd;
std::mutex mtx_even;

const int MAX = 10;

void print_odd(void *args) {
    while (1){ 
        mtx_odd.lock();
        
        int *count = (int*)args;

        if (*count >= MAX) {
            break;
        }
        if (*count % 2 == 1) {
            printf("Odd: %d\n", (*count)++);
        }

        mtx_even.unlock();
    }
    mtx_even.unlock();
}

void print_even(void *args) {
    while (1){
        mtx_even.lock();
        
        int *count = (int*)args;

        if (*count >= MAX) {
            break;
        }
        if (*count % 2 == 0) {
            printf("Even: %d\n", (*count)++);
        }
        mtx_odd.unlock();
    }
    mtx_odd.unlock();
}

int main() {
    int count = 1;

    std::thread thread_odd{print_odd, &count};
    std::thread thread_even{print_even, &count};

    thread_odd.join();
    thread_even.join();

    return 0;
}
