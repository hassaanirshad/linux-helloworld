#include "args.h"
#include "command/fd_transfer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MSG_BUF_SIZE 4096


static int server_start(const char *socket_path) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    unlink(socket_path);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    if (listen(fd, 1) < 0) {
        perror("listen");
        close(fd);
        return -1;
    }

    fprintf(stderr, "listening on %s\n", socket_path);
    return fd;
}

static int server_accept(int server_fd) {
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0)
        perror("accept");
    return client_fd;
}

static void server_handle(int client_fd, const server_args_t *args) {
    switch (args->common.command) {
    case COMMON_CMD_FD_TRANSFER:
        handle_fd_transfer(client_fd);
        break;
    }
}

static void server_stop(int server_fd, const char *socket_path) {
    unlink(socket_path);
    close(server_fd);
}

int main(int argc, char *argv[]) {
    server_args_t args;
    if (parse_server_args(argc, argv, &args) != 0)
        return 1;

    int server_fd = server_start(args.common.socket_path);
    if (server_fd < 0)
        return 1;

    int client_fd = server_accept(server_fd);
    if (client_fd < 0) {
        server_stop(server_fd, args.common.socket_path);
        return 1;
    }

    server_handle(client_fd, &args);

    close(client_fd);

    server_stop(server_fd, args.common.socket_path);

    return 0;
}
