#include <inttypes.h>
#include <stdio.h>

#define MIN_PID 300
#define MAX_PID 5000
#define TOTAL_PIDS (MAX_PID - MIN_PID)
#define NUMBER_WIDTH 32
#define NUMBER_MAX UINT32_MAX
#define ARRAY_SIZE (TOTAL_PIDS / NUMBER_WIDTH + (TOTAL_PIDS % NUMBER_WIDTH == 0 ? 0 : 1))

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
}

int main() {
    allocate_map();
    for (int i = 0; i < 32; ++i) {
        printf("%d\n", allocate_pid());
    }
    printf("--------\n");
    printf("%d\n", allocate_pid());
    printf("%d\n", allocate_pid());
    printf("%d\n", allocate_pid());
    printf("%d\n", allocate_pid());
    release_pid(334);
    release_pid(304);
    printf("%d\n", allocate_pid());
    printf("%d\n", allocate_pid());
    printf("%d\n", allocate_pid());

    return 0;
}
