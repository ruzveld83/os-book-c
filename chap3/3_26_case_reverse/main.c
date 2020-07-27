#include <stdio.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

/*
 * Here two pipes should have been used.
 * See https://stackoverflow.com/questions/10141678/why-cant-a-pipe-created-using-pipe-be-used-as-a-bi-directional-pipe
 */
int main() {
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
        char * message_to_send = "Hi There";
        write(pipe_fd[WRITE_END], message_to_send, strlen(message_to_send) + 1);
        fprintf(stderr, "C: WRITTEN %lu\n", strlen(message_to_send));
        char received_message[256];
        read(pipe_fd[READ_END], &received_message, 256);
        fprintf(stderr, "C: READ %lu '%s'\n", strlen(received_message), received_message);
        close(pipe_fd[READ_END]);
        close(pipe_fd[WRITE_END]);
    } else {
        char received_message[256];
        // sleep(1); // uncomment to break things
        read(pipe_fd[READ_END], &received_message, 256);
        fprintf(stderr, "P: READ %lu '%s'\n", strlen(received_message), received_message);
        char * c = received_message;
        int i = 0;
        for (; *c != '\0' && i < 256; ++c, ++i) {
            if (*c >= 'a' && *c <= 'z') {
                *c -= 32;
            } else if (*c >= 'A' && *c <= 'Z') {
                *c += 32;
            }
        }
        write(pipe_fd[WRITE_END], &received_message, strlen(received_message));
        fprintf(stderr, "P: WRITTEN %lu\n", strlen(received_message));
        wait(NULL);
    }
    return 0;
}
