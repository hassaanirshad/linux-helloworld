#include "fd_transfer.h"
#include "common/file_op.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#define MSG_BUF_SIZE 4096


static int do_send_msg(int client_fd) {
    // Create a temp file and pass its fd to the client
    const char *tmp_path = "/tmp/fd_transfer_tmp";
    int tmp_fd = open(tmp_path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (tmp_fd < 0) {
        perror("open");
        return -1;
    }
    unlink(tmp_path);

    if (print_fd_info(tmp_fd) < 0) {
        close(tmp_fd);
        return -1;
    }

    const char *write_buf = "hello from server";
    if (write_fd(tmp_fd, write_buf, strlen(write_buf)) < 0) {
        close(tmp_fd);
        return -1;
    }

    // rewind so the client reads from the beginning
    if (lseek(tmp_fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        close(tmp_fd);
        return -1;
    }

    char cmsg_buf[CMSG_SPACE(sizeof(int))];
    char data = '\0';

    struct iovec iov = {
        .iov_base = &data,
        .iov_len  = sizeof(data),
    };

    struct msghdr msg = {
        .msg_iov        = &iov,
        .msg_iovlen     = 1,
        .msg_control    = cmsg_buf,
        .msg_controllen = sizeof(cmsg_buf),
    };

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type  = SCM_RIGHTS;
    cmsg->cmsg_len   = CMSG_LEN(sizeof(int));
    memcpy(CMSG_DATA(cmsg), &tmp_fd, sizeof(int));

    if (sendmsg(client_fd, &msg, 0) < 0) {
        perror("sendmsg");
        close(tmp_fd);
        return -1;
    }

    close(tmp_fd);
    return 0;
}

static int do_recv_msg(int client_fd) {
    char buf[MSG_BUF_SIZE];

    struct iovec iov = {
        .iov_base = buf,
        .iov_len  = sizeof(buf) - 1,
    };

    struct msghdr msg = {
        .msg_iov    = &iov,
        .msg_iovlen = 1,
    };

    ssize_t n = recvmsg(client_fd, &msg, 0);
    if (n < 0) {
        perror("recvmsg");
        return -1;
    }
    buf[n] = '\0';

    if (strcmp(buf, "SUCCESS") == 0) {
        fprintf(stderr, "fd_transfer: client reported SUCCESS\n");
        return 0;
    } else if (strcmp(buf, "FAILURE") == 0) {
        fprintf(stderr, "fd_transfer: client reported FAILURE\n");
        return -1;
    }

    fprintf(stderr, "fd_transfer: unexpected response: %s\n", buf);
    return -1;
}

void handle_fd_transfer(int client_fd) {
    if (do_send_msg(client_fd) < 0) {
        return;
    }

    if (do_recv_msg(client_fd) < 0) {
        return;
    }
}
