#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length of a command */
#define MAX_ARGS MAX_LINE / 2 + 1
#define READ_END 0
#define WRITE_END 1

int main(void) {
    char command[MAX_LINE] = "";
    char prev_command[MAX_LINE] = "";
    char * args[MAX_ARGS]; /* command line arguments */
    char * args_pipe[MAX_ARGS]; /* command line arguments after pipe */
    bool background = false;
    bool invalid_command = true;
    char out_file[MAX_LINE] = "";
    char in_file[MAX_LINE] = "";
    bool input_from_file = false;
    bool output_to_file = false;
    bool has_pipe = false;
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
            int arg_index = 0;
            background = false;
            invalid_command = false;
            in_file[0] = '\0';
            out_file[0] = '\0';
            input_from_file = false;
            output_to_file = false;
            has_pipe = false;

            char * tok = strtok(command, " \n");
            char prev_tok[MAX_LINE] = "";
            while (tok != NULL) {
                if (background) {
                    printf("No input is expected after '&'\n");
                    invalid_command = true;
                    break;
                }
                if (arg_index >= MAX_ARGS - 1) {
                    printf("Too many args\n");
                    invalid_command = true;
                    break;
                }

                if (strcmp(prev_tok, "<") == 0) {
                    strcpy(in_file, tok);
                } else if (strcmp(prev_tok, ">") == 0) {
                    strcpy(out_file, tok);
                }

                if (strcmp(tok, "&") == 0) {
                    background = true;
                } else if (strcmp(tok, "<") == 0) {
                    input_from_file = true;
                } else if (strcmp(tok, ">") == 0) {
                    output_to_file = true;
                } else if (strcmp(tok, "|") == 0) {
                    if (has_pipe) {
                        printf("Multiple pipes is unsupported\n");
                        invalid_command = true;
                        break;
                    }
                    has_pipe = true;
                    args[arg_index] = NULL;
                    arg_index = 0;
                } else if (!output_to_file && !input_from_file) {
                    if (!has_pipe) {
                        args[arg_index] = tok;
                    } else {
                        args_pipe[arg_index] = tok;
                    }
                    ++arg_index;
                }

                strcpy(prev_tok, tok);
                tok = strtok(NULL, " \n");
            }
            if (!has_pipe) {
                args[arg_index] = NULL;
                args_pipe[0] = NULL;
            } else {
                args_pipe[arg_index] = NULL;
            }
        }

        if (input_from_file && in_file[0] == '\0') {
            printf("No input file specified\n");
            invalid_command = true;
        } else if (output_to_file && out_file[0] == '\0') {
            printf("No output file specified\n");
            invalid_command = true;
        } else if (has_pipe && (input_from_file || output_to_file)) {
            printf("Piping and redirecting input/output at the same time is unsupported\n");
            invalid_command = true;
        } else if (has_pipe && args_pipe[0] == NULL) {
            printf("Nowhere to pipe to\n");
            invalid_command = true;
        }

//        for (int i = 0; args[i] != NULL && i < MAX_ARGS; ++i) {
//            printf("args[%d]: %s\n", i, args[i]);
//        }
//        for (int i = 0; args_pipe[i] != NULL && i < MAX_ARGS; ++i) {
//            printf("args_pipe[%d]: %s\n", i, args_pipe[i]);
//        }

        if (args[0] == NULL || invalid_command) {
            continue;
        }

        pid_t pid = fork();
        if (pid == -1) {
            printf("Error forking\n");
            continue;
        } else if (pid == 0 && !has_pipe) {
            if (input_from_file) {
                int fd = open(in_file, O_RDONLY);
                if (fd < 0) {
                    printf("Error opening '%s'. Errno: %d, message: %s\n", in_file, errno, strerror(errno));
                    return 1;
                }
                printf("Opened file '%s' with desc '%d'\n", in_file, fd);
                if (dup2(fd, STDIN_FILENO) < 0) {
                    printf("Error in dup2 '%s'. Errno: %d, message: %s\n", in_file, errno, strerror(errno));
                    return 1;
                }
            }
            if (output_to_file)  {
                int fd = open(out_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
                if (fd < 0) {
                    printf("Error opening '%s'. Errno: %d, message: %s\n", out_file, errno, strerror(errno));
                    fflush(stdout);
                    return 1;
                }
                printf("Opened file '%s' with desc '%d'\n", out_file, fd);
                if (dup2(fd, STDOUT_FILENO) < 0) {
                    printf("Error in dup2 '%s'. Errno: %d, message: %s\n", in_file, errno, strerror(errno));
                    fflush(stdout);
                    return 1;
                }
            }
            fflush(stdout);
            execvp(args[0], args);
            printf("Error in exec. Errno: %d, message: %s\n", errno, strerror(errno));
            fflush(stdout);
            return 1;
        } else if (pid == 0 && has_pipe) {
            int pipe_fds[2];
            if (pipe(pipe_fds) < 0) {
                printf("Error in pipe\n");
                fflush(stdout);
                return 1;
            }
            printf("Read fd: %d, write fd: %d\n", pipe_fds[READ_END], pipe_fds[WRITE_END]);
            pid_t inner_pid = fork();
            if (inner_pid == -1) {
                printf("Error forking\n");
                fflush(stdout);
                return 1;
            }
            if (inner_pid == 0) {
                close(pipe_fds[READ_END]);
                dup2(pipe_fds[WRITE_END], STDOUT_FILENO);
                execvp(args[0], args);
                printf("Error in exec. Errno: %d, message: %s\n", errno, strerror(errno));
                fflush(stdout);
                return 1;
            } else {
                close(pipe_fds[WRITE_END]);
                dup2(pipe_fds[READ_END], STDIN_FILENO);
                waitpid(inner_pid, NULL, 0);
                execvp(args_pipe[0], args_pipe);
                printf("Error in inner exec. Errno: %d, message: %s\n", errno, strerror(errno));
                fflush(stdout);
                return 1;
            }
        } else if (!background) {
            waitpid(pid, NULL, 0);
        }
    }
    return 0;
}
