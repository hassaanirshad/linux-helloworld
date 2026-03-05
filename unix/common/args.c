#include "args.h"

#include <stdio.h>
#include <string.h>

static int parse_command(const char *str, common_command_t *out) {
    if (strcmp(str, "fd_transfer") == 0) {
        *out = COMMON_CMD_FD_TRANSFER;
        return 0;
    }
    fprintf(stderr, "unknown command: %s\n", str);
    return -1;
}

int parse_args(int argc, char *argv[], common_args_t *out) {
    const char *socket_path = NULL;
    const char *command = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--unix-sock-path") == 0 && i + 1 < argc) {
            socket_path = argv[++i];
        } else if (strcmp(argv[i], "--command") == 0 && i + 1 < argc) {
            command = argv[++i];
        } else {
            fprintf(stderr, "unknown argument: %s\n", argv[i]);
            return -1;
        }
    }

    if (!socket_path) {
        fprintf(stderr, "missing required flag: --unix-sock-path\n");
        return -1;
    }
    if (!command) {
        fprintf(stderr, "missing required flag: --command\n");
        return -1;
    }

    out->socket_path = socket_path;
    return parse_command(command, &out->command);
}
