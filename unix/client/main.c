#include "args.h"
#include "command/fd_transfer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MSG_BUF_SIZE 4096


static int client_connect(const char *socket_path) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(fd);
        return -1;
    }

    return fd;
}

static void client_handle(int fd, const client_args_t *args) {
    switch (args->common.command) {
    case COMMON_CMD_FD_TRANSFER:
        handle_fd_transfer(fd);
        break;
    }
}

int main(int argc, char *argv[]) {
    client_args_t args;
    if (parse_client_args(argc, argv, &args) != 0)
        return 1;

    int fd = client_connect(args.common.socket_path);
    if (fd < 0)
        return 1;

    client_handle(fd, &args);

    close(fd);
    return 0;
}
