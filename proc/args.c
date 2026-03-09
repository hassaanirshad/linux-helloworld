#include "args.h"

#include <stdio.h>
#include <string.h>

static const struct {
    const char     *name;
    proc_command_t  cmd;
} command_map[] = {
    { "clone_single_thread",        PROC_CMD_CLONE_SINGLE_THREAD        },
    { "clone_multi_thread",         PROC_CMD_CLONE_MULTI_THREAD         },
    { "clone_non_thread",           PROC_CMD_CLONE_NON_THREAD           },
    { "fork",                       PROC_CMD_FORK                       },
    { "vfork",                      PROC_CMD_VFORK                      },
    { "clone_single_thread_execve", PROC_CMD_CLONE_SINGLE_THREAD_EXECVE },
    { "clone_multi_thread_execve",  PROC_CMD_CLONE_MULTI_THREAD_EXECVE  },
    { "clone_non_thread_execve",    PROC_CMD_CLONE_NON_THREAD_EXECVE    },
};

static int parse_command(const char *str, proc_command_t *out) {
    for (size_t i = 0; i < sizeof(command_map) / sizeof(command_map[0]); i++) {
        if (strcmp(str, command_map[i].name) == 0) {
            *out = command_map[i].cmd;
            return 0;
        }
    }
    fprintf(stderr, "unknown command: %s\n", str);
    return -1;
}

const char *proc_command_name(proc_command_t command) {
    for (size_t i = 0; i < sizeof(command_map) / sizeof(command_map[0]); i++) {
        if (command_map[i].cmd == command)
            return command_map[i].name;
    }
    return "unknown";
}

static void print_help(const char *prog) {
    fprintf(stdout, "usage: %s --command <command> [--verbose]\n\n", prog);
    fprintf(stdout, "commands:\n");
    for (size_t i = 0; i < sizeof(command_map) / sizeof(command_map[0]); i++)
        fprintf(stdout, "  %s\n", command_map[i].name);
}

int parse_proc_args(int argc, char *argv[], proc_args_t *out) {
    const char *command = NULL;
    out->verbose = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help(argv[0]);
            return -1;
        } else if (strcmp(argv[i], "--command") == 0 && i + 1 < argc) {
            command = argv[++i];
        } else if (strcmp(argv[i], "--verbose") == 0) {
            out->verbose = 1;
        } else {
            fprintf(stderr, "unknown argument: %s\n", argv[i]);
            return -1;
        }
    }

    if (!command) {
        fprintf(stderr, "missing required flag: --command\n");
        return -1;
    }

    return parse_command(command, &out->command);
}
