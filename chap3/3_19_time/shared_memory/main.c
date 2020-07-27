#include <stdio.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[], char * envp[]) {
    if (argc != 2) {
        fprintf(stderr, "Bad argc: %d\n", argc);
        return 1;
    }
    char * command = argv[1];

    int shm_fd;
    struct timeval * shm_ptr;
    const char * shm_name = "time_shm_file";

    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(struct timeval));
    shm_ptr = (struct timeval *) mmap(0, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pid_t fork_pid = fork();
    if (fork_pid == -1) {
        fprintf(stderr, "Error forking: %d\n", errno);
        return 1;
    } else if (fork_pid == 0) {
        struct timeval t0;
        if (gettimeofday(&t0, NULL) != 0) {
            fprintf(stderr, "Error getting time: %d\n", errno);
            return 1;
        }
        *shm_ptr = t0;
        execlp(command, command, NULL);
        fprintf(stderr, "Error in exec\n");

    } else {
        wait(NULL);
        time_t t0_sec = shm_ptr->tv_sec;
        suseconds_t t0_usec = shm_ptr->tv_usec;
        struct timeval t1;
        if (gettimeofday(&t1, NULL) != 0) {
            fprintf(stderr, "Error getting time: %d\n", errno);
            return 1;
        }
        time_t t1_sec = t1.tv_sec;
        suseconds_t t1_usec = t1.tv_usec;
        double res = t1_sec - t0_sec + (t1_usec - t0_usec) * 1e-6;
        fprintf(stderr, "Time elapsed: %.4f\n", res);
        shm_unlink(shm_name);
    }
    return 0;
}
