#pragma once

#include "common/args.h"

typedef struct {
    common_args_t common;
} client_args_t;

int parse_client_args(int argc, char *argv[], client_args_t *out);
