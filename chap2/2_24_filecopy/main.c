//#include <stdio.h>
//#include <fcntl.h>
//#include <errno.h>
//#include <string.h>
//#include <unistd.h>
//
//#define MAX_FILE_NAME_LENGTH 128
//#define BUF_SIZE 32
//
//int main() {
//    char in_file_name[MAX_FILE_NAME_LENGTH], out_file_name[MAX_FILE_NAME_LENGTH];
//    int in_file_desc, out_file_desc;
//    printf("Type a name of an input file: ");
//    scanf("%s", in_file_name);
//    printf("%s\n", in_file_name);
//    in_file_desc = open(in_file_name, O_RDONLY);
//    if (in_file_desc == -1) {
//        int error_number = errno;
//        printf("File '%s' doesn't exist or is not readable. Errno: %d\n", in_file_name, error_number);
//        printf("Error message: %s\n", strerror(error_number));
//        return 1;
//    }
//
//    printf("Type a name of an output file: ");
//    scanf("%s", out_file_name);
//    printf("%s\n", out_file_name);
//    out_file_desc = open(out_file_name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
//    if (out_file_desc == -1) {
//        int error_number = errno;
//        printf("File '%s' doesn't exist or is not readable. Errno: %d\n", out_file_name, error_number);
//        printf("Error message: %s\n", strerror(error_number));
//        if (close(in_file_desc) == -1) {
//            printf("Error closing an input file. Errno: %d. Message: %s\n", errno, strerror(errno));
//        }
//        return 1;
//    }
//
//    char buf[BUF_SIZE];
//    ssize_t bytes_read;
//    while ((bytes_read = read(in_file_desc, buf, BUF_SIZE)) > 0) {
//        ssize_t bytes_written = write(out_file_desc, buf, bytes_read);
//        if (bytes_read != bytes_written) {
//            printf("Error writing to file. Expected to write %zd chars, actually wrote %zd", bytes_read, bytes_written);
//            return 1;
//        }
//    }
//    if (bytes_read == -1) {
//        printf("Error reading file. Errno: %d. Message: %s\n", errno, strerror(errno));
//    }
//
//    if (close(in_file_desc) == -1) {
//        printf("Error closing an input file. Errno: %d. Message: %s\n", errno, strerror(errno));
//        return 1;
//    }
//    if (close(out_file_desc) == -1) {
//        printf("Error closing an output file. Errno: %d. Message: %s\n", errno, strerror(errno));
//        return 1;
//    }
//    return 0;
//}
