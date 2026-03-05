#include "args.h"

int parse_client_args(int argc, char *argv[], client_args_t *out) {
    return parse_args(argc, argv, &out->common);
}
