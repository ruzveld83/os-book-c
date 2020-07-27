#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length of a command */

int main(void) {
    char * args[MAX_LINE / 2 + 1]; /* command line arguments */
    bool background = false;
    int should_run = 1; /* flag to determine when to exit program */
    while (should_run) {
        printf("osh>");
        fflush(stdout);

        char command[MAX_LINE];
        fgets(command, MAX_LINE, stdin);

        {
            char * tok = strtok(command, " \n");
            char ** arg = args;
            while (tok != NULL) {
                background = !strcmp(tok, "&");
                *arg = tok;
                ++arg;
                tok = strtok(NULL, " \n");
            }
            if (arg != args && strcmp(*(arg - 1), "&") == 0) {
                *(arg - 1) = NULL;
            } else {
                *arg = NULL;
            }
        }

//        for (char ** arg = args; *arg != NULL; ++arg) {
//            printf("%s\n", *arg);
//        }
//        fflush(stdout);

        if (args[0] == NULL) {
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
