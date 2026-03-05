#pragma once

#include <stddef.h>
#include <sys/types.h>

int print_fd_info(int fd);
int write_fd(int fd, const void *buf, size_t len);
ssize_t read_fd(int fd, void *buf, size_t len);
