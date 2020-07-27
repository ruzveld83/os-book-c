#include <stdio.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char * argv[], char * envp[]) {
    if (argc != 2) {
        fprintf(stderr, "Bad argc: %d\n", argc);
        return 1;
    }
    char * command = argv[1];

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        fprintf(stderr, "Error creating a pipe: %d\n", errno);
        return 1;
    }

    pid_t fork_pid = fork();
    if (fork_pid == -1) {
        fprintf(stderr, "Error forking: %d\n", errno);
        return 1;
    } else if (fork_pid == 0) {
        close(pipe_fd[READ_END]);

        struct timeval t0;
        if (gettimeofday(&t0, NULL) != 0) {
            fprintf(stderr, "Error getting time: %d\n", errno);
            return 1;
        }

        write(pipe_fd[WRITE_END], &t0, sizeof(struct timeval));
        close(pipe_fd[WRITE_END]);

        execlp(command, command, NULL);
        fprintf(stderr, "Error in exec\n");
    } else {
        wait(NULL);
        close(pipe_fd[WRITE_END]);
        struct timeval t0;
        read(pipe_fd[READ_END], &t0, sizeof(struct timeval));
        close(pipe_fd[READ_END]);

        time_t t0_sec = t0.tv_sec;
        suseconds_t t0_usec = t0.tv_usec;

        struct timeval t1;
        if (gettimeofday(&t1, NULL) != 0) {
            fprintf(stderr, "Error getting time: %d\n", errno);
            return 1;
        }
        time_t t1_sec = t1.tv_sec;
        suseconds_t t1_usec = t1.tv_usec;

        double res = t1_sec - t0_sec + (t1_usec - t0_usec) * 1e-6;
        fprintf(stderr, "Time elapsed: %.4f\n", res);
    }
    return 0;
}
