#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length of a command */
#define MAX_ARGS MAX_LINE / 2 + 1

int main(void) {
    char command[MAX_LINE] = "";
    char prev_command[MAX_LINE] = "";
    char * args[MAX_ARGS]; /* command line arguments */
    bool background = false;
    bool invalid_command = true;
    int should_run = 1; /* flag to determine when to exit program */
    while (should_run) {
        printf("osh>");
        fflush(stdout);

        fgets(command, MAX_LINE, stdin);

        if (strcmp(command, "!!\n") == 0) {
            if (prev_command[0] == '\0') {
                printf("No commands in history\n");
            }
            strcpy(command, prev_command);
        } else if (strcmp(command, "exit\n") == 0) {
            should_run = 0;
            continue;
        } else if (strcmp(command, "") != 0 && strcmp(command, "\n") != 0) {
            strcpy(prev_command, command);
        }

        /* parsing a new command */
        {
            int i = 0;
            background = false;
            invalid_command = false;
            char * tok = strtok(command, " \n");
            while (tok != NULL) {
                if (background) {
                    printf("No input is expected after '&'\n");
                    invalid_command = true;
                    break;
                }
                if (i >= MAX_ARGS - 1) {
                    printf("Too many args\n");
                    invalid_command = true;
                    break;
                }
                if (strcmp(tok, "&") == 0) {
                    background = true;
                } else {
                    args[i] = tok;
                }
                tok = strtok(NULL, " \n");
                ++i;
            }
            args[i] = NULL;
        }

        if (args[0] == NULL || invalid_command) {
            continue;
        }

        pid_t pid = fork();
        if (pid == -1) {
            printf("Error forking\n");
            continue;
        } else if (pid == 0) {
            execvp(args[0], args);
        } else if (!background) {
            waitpid(pid, NULL, 0);
        }
    }
    return 0;
}
