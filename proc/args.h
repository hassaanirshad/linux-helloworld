#pragma once

typedef enum {
    PROC_CMD_CLONE_SINGLE_THREAD,
    PROC_CMD_CLONE_MULTI_THREAD,
    PROC_CMD_CLONE_NON_THREAD,
    PROC_CMD_FORK,
    PROC_CMD_VFORK,
    PROC_CMD_CLONE_SINGLE_THREAD_EXECVE,
    PROC_CMD_CLONE_MULTI_THREAD_EXECVE,
    PROC_CMD_CLONE_NON_THREAD_EXECVE,
} proc_command_t;

typedef struct {
    proc_command_t command;
    int verbose;
} proc_args_t;

int parse_proc_args(int argc, char *argv[], proc_args_t *out);
const char *proc_command_name(proc_command_t command);
