#include "file_op.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 256

int file_op_verbose = 0;

int run_file_op(const char *command_name, const char *proc_desc) {
    char path[128];
    snprintf(path, sizeof(path), "/tmp/proc_%s_%s", command_name, proc_desc);

    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) {
        perror("open");
        return -1;
    }
    if (file_op_verbose)
        fprintf(stderr, "file_op [%s]: opened %s (fd=%d)\n", command_name, path, fd);

    size_t len = strlen(command_name);
    ssize_t nw = write(fd, command_name, len);
    if (nw < 0) {
        perror("write");
        close(fd);
        return -1;
    }
    if (file_op_verbose)
        fprintf(stderr, "file_op [%s]: wrote %zd bytes\n", command_name, nw);

    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        close(fd);
        return -1;
    }

    char buf[BUF_SIZE];
    ssize_t nr = read(fd, buf, sizeof(buf) - 1);
    if (nr < 0) {
        perror("read");
        close(fd);
        return -1;
    }
    buf[nr] = '\0';
    if (file_op_verbose)
        fprintf(stderr, "file_op [%s]: read %zd bytes: \"%s\"\n", command_name, nr, buf);

    close(fd);
    return 0;
}
