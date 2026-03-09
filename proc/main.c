#include "args.h"
#include "common/file_op.h"
#include "command/clone_single_thread.h"
#include "command/clone_multi_thread.h"
#include "command/clone_non_thread.h"
#include "command/fork.h"
#include "command/vfork.h"
#include "command/clone_single_thread_execve.h"
#include "command/clone_multi_thread_execve.h"
#include "command/clone_non_thread_execve.h"

#include <stdio.h>

static int dispatch(const proc_args_t *args) {
    switch (args->command) {
    case PROC_CMD_CLONE_SINGLE_THREAD:        return cmd_clone_single_thread();
    case PROC_CMD_CLONE_MULTI_THREAD:         return cmd_clone_multi_thread();
    case PROC_CMD_CLONE_NON_THREAD:           return cmd_clone_non_thread();
    case PROC_CMD_FORK:                       return cmd_fork();
    case PROC_CMD_VFORK:                      return cmd_vfork();
    case PROC_CMD_CLONE_SINGLE_THREAD_EXECVE: return cmd_clone_single_thread_execve();
    case PROC_CMD_CLONE_MULTI_THREAD_EXECVE:  return cmd_clone_multi_thread_execve();
    case PROC_CMD_CLONE_NON_THREAD_EXECVE:    return cmd_clone_non_thread_execve();
    }
    fprintf(stderr, "unhandled command\n");
    return -1;
}

int main(int argc, char *argv[]) {
    proc_args_t args;
    if (parse_proc_args(argc, argv, &args) != 0)
        return 1;

    file_op_verbose = args.verbose;
    return dispatch(&args) == 0 ? 0 : 1;
}
