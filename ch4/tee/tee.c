/* TEE(1)
 * 
 * NAME
 *      tee - read from standard input and write to standard output and files
 * 
 * SYNOPSYS
 *      tee [OPTION]... [FILE]...
 * 
 * DESCRIPTION
 *      Minimum implementation of Linux command tee(1). 
 *      Copy standard input to each FILE, and also to standard output.
 * 
 *      -a
 *          append to given FILEs, do not overwrite
 * 
 *      -h
 *          display help and exit
 * 
 * AUTHOR
 *      Written by Dmytro Zykov.
 *
 * January 2023
 */

#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define MAX_FILES   128

void usage(const char *prog, int status)
{
    fprintf(stderr, "Usage: %s [OPTION] [FILE]...\n", prog);
    fprintf(stderr, "Copy standard input ot each FILE, and also to standard output.\n\n");
    fprintf(stderr, "\t-a \t\t append to the given FILEs, do not overwrite\n");
    fprintf(stderr, "\t-h \t\t display help and exit\n");
    _exit(status);
}

void exit_failure(const char *s)
{
    perror(s);
    _exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int append = 0;
    char opt;   // parse command line arguments
    while ((opt = getopt(argc, argv, "a:h")) != -1) {
        switch (opt)
        {
        case '?':
            usage(argv[0], EXIT_FAILURE);
            break;
        case 'h':
            usage(argv[0], EXIT_SUCCESS);
        case 'a':
            append = 1;
        default:
            break;
        }
    }

    if (optind > argc) {
        usage(argv[0], EXIT_FAILURE);
    }
    int optc = optind;
    if (append)
        --optc;
    

    int oflags = O_CREAT | O_WRONLY;
    oflags |= append ? O_APPEND : O_TRUNC;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP |
        S_IWGRP | S_IROTH | S_IWOTH;
    
    int fds[MAX_FILES] = {-1};
    int file_count = 0;
    for (int i = optc; i < argc; ++i) {
        if (file_count >= MAX_FILES) {
            fprintf(stderr, "Too many files. 128 max.\n");
            _exit(EXIT_FAILURE);
        }
        fds[i - optc] = open(argv[i], oflags, mode);
        if (fds[i - optind] == -1) {
            exit_failure("Failed to open file");
        } 
        ++file_count;
    }

    char buf[BUFFER_SIZE];
    int rc;
    // read from STDIN to buffer
    while ((rc = read(STDIN_FILENO, buf, BUFFER_SIZE)) > 0) {
        // write to STDOUT
        if (write(STDOUT_FILENO, buf, rc) != rc) {
            exit_failure("Failed to write to STDOUT");
        }

        // write to every file
        for (int i = 0; i < file_count; ++i) {
            if (write(fds[i], buf, rc) != rc) {
                exit_failure("Failed to write to a file");
            }
        }
    }

    if (rc == -1) {
        exit_failure("Failed to read from STDIN");
    }

    for (int i = 0; i < file_count; ++i) {
        if (close(fds[i]) == -1) {
            exit_failure("Failed to close a file");
        }
    }

    exit(EXIT_SUCCESS);
}
