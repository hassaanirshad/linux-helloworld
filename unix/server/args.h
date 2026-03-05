#pragma once

#include "common/args.h"

typedef struct {
    common_args_t common;
} server_args_t;

int parse_server_args(int argc, char *argv[], server_args_t *out);
