#include <stdio.h>
#include <unistd.h>

int main() {
    fprintf(stderr, "Forking...\n");
    pid_t pid = fork();
    fprintf(stderr, "Forked. PID: %d\n", getpid());
    if (pid == 0) {
        sleep(10);
        fprintf(stderr, "Done %d\n", getpid());
    } else {
        sleep(20);
        fprintf(stderr, "Done %d\n", getpid());
    }
    return 0;
}
