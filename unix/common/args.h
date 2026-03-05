#pragma once

typedef enum {
    COMMON_CMD_FD_TRANSFER,
} common_command_t;

typedef struct {
    const char *socket_path;
    common_command_t command;
} common_args_t;

int parse_args(int argc, char *argv[], common_args_t *out);
