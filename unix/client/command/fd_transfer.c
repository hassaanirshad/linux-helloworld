#include "fd_transfer.h"
#include "common/file_op.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

static int do_recv_fd(int server_fd) {
    char cmsg_buf[CMSG_SPACE(sizeof(int))];
    char data;

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

    if (recvmsg(server_fd, &msg, 0) < 0) {
        perror("recvmsg");
        return -1;
    }

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    if (!cmsg || cmsg->cmsg_level != SOL_SOCKET || cmsg->cmsg_type != SCM_RIGHTS) {
        fprintf(stderr, "fd_transfer: no fd received\n");
        return -1;
    }

    int received_fd;
    memcpy(&received_fd, CMSG_DATA(cmsg), sizeof(int));
    print_fd_info(received_fd);
    return received_fd;
}

static int do_send_response(int server_fd, const char *response) {
    ssize_t n = send(server_fd, response, strlen(response), 0);
    if (n < 0) {
        perror("send");
        return -1;
    }
    return 0;
}

void handle_fd_transfer(int server_fd) {
    int received_fd = do_recv_fd(server_fd);
    if (received_fd < 0) {
        do_send_response(server_fd, "FAILURE");
        return;
    }

    const char *write_buf = "hello from client";
    if (write_fd(received_fd, write_buf, strlen(write_buf)) < 0) {
        close(received_fd);
        do_send_response(server_fd, "FAILURE");
        return;
    }

    close(received_fd);
    do_send_response(server_fd, "SUCCESS");
}
