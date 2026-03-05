#include "args.h"

int parse_server_args(int argc, char *argv[], server_args_t *out) {
    return parse_args(argc, argv, &out->common);
}
