#include "file_op.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stddef.h>

int write_fd(int fd, const void *buf, size_t len) {
    size_t written = 0;
    while (written < len) {
        ssize_t n = write(fd, (const char *)buf + written, len - written);
        if (n < 0) {
            perror("write");
            return -1;
        }
        written += n;
    }
    fprintf(stderr, "write_fd: wrote %zu bytes\n", written);
    if (fsync(fd) < 0) {
        perror("fsync");
        return -1;
    }
    return 0;
}

ssize_t read_fd(int fd, void *buf, size_t len) {
    ssize_t n = read(fd, buf, len);
    if (n < 0) {
        perror("read");
        return -1;
    }
    fprintf(stderr, "read_fd: read %zd bytes\n", n);
    return n;
}

static off_t get_file_size(int fd) {
    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        return -1;
    }
    fprintf(stderr, "file size: %ld bytes\n", (long)st.st_size);
    return st.st_size;
}

int print_fd_info(int fd) {
    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        return -1;
    }

    char path[4096];
    char proc_path[64];
    snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", fd);
    ssize_t path_len = readlink(proc_path, path, sizeof(path) - 1);
    if (path_len < 0) {
        perror("readlink");
        return -1;
    }
    path[path_len] = '\0';

    fprintf(stderr, "fd_transfer: [fd=%d, path=%s, inode=%lu, size=%ld, mode=%o]\n",
            fd, path, (unsigned long)st.st_ino, (long)st.st_size, (unsigned)st.st_mode);
    return 0;
}
