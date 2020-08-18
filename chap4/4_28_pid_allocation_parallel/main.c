#include <inttypes.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_PID 300
#define MAX_PID 5000
#define TOTAL_PIDS (MAX_PID - MIN_PID)
#define NUMBER_WIDTH 32
#define NUMBER_MAX UINT32_MAX
#define ARRAY_SIZE (TOTAL_PIDS / NUMBER_WIDTH + (TOTAL_PIDS % NUMBER_WIDTH == 0 ? 0 : 1))
#define N_THREADS 100

typedef uint32_t num;

struct bitset_struct {
    num numbers[ARRAY_SIZE];
} bitset;

int allocate_map(void) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        bitset.numbers[i] = 0;
    }
    return 1;
}

int allocate_pid(void) {
    int i;
    for (i = 0; i != ARRAY_SIZE && bitset.numbers[i] == NUMBER_MAX; ++i) {}
    num number = bitset.numbers[i];
    if (i == ARRAY_SIZE) {
        return -1;
    }
    int p = 0;
    num mask = 1u;
    for (; (number | mask) == number; ++p, mask <<= 1u) {}
    int pid = i * NUMBER_WIDTH + p + MIN_PID;
    if (pid > MAX_PID) {
        return -1;
    }
    bitset.numbers[i] |= mask;
    printf("Allocated %d\n", pid);
    return pid;
}

/* Releases a pid */
void release_pid(int pid) {
    if (pid < MIN_PID || pid > MAX_PID) {
        return;
    }
    int i = (pid - MIN_PID) / NUMBER_WIDTH;
    int p = (pid - MIN_PID) % NUMBER_WIDTH;
    num mask = 1u << p;
    bitset.numbers[i] ^= mask;
    printf("Released %d\n", pid);
}

void * runner(void * param) {
    while (true) {
        int pid = allocate_pid();
        int sleep_seconds = rand() % 5;
        sleep(sleep_seconds);
        release_pid(pid);
    }
    pthread_exit(0);
}

int main() {
    srand(0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for (int i = 0; i != N_THREADS; ++i) {
        pthread_t tid;
        pthread_create(&tid, &attr, runner, NULL);
    }

    sleep(10);
    return 0;
}
