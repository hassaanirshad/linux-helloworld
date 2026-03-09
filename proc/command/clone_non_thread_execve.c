#define _GNU_SOURCE
#include "clone_non_thread_execve.h"
#include "common/file_op.h"
#include "common/log.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CHILD_STACK_SIZE (1024 * 1024)

static int child_fn(void *arg) {
    (void)arg;
    log_proc_info("clone_non_thread_execve", "child");
    const char *path = "/tmp/proc_clone_non_thread_execve_child";
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "touch %s && rm %s", path, path);

    char *argv[] = { "/bin/sh", "-c", cmd, NULL };
    char *envp[] = { NULL };
    execve("/bin/sh", argv, envp);

    perror("execve");
    return 1;
}

int cmd_clone_non_thread_execve(void) {
    void *stack = malloc(CHILD_STACK_SIZE);
    if (!stack) {
        perror("malloc");
        return -1;
    }
    void *stack_top = (char *)stack + CHILD_STACK_SIZE;

    pid_t pid = clone(child_fn, stack_top, SIGCHLD, NULL);
    if (pid < 0) {
        perror("clone");
        free(stack);
        return -1;
    }

    log_proc_info("clone_non_thread_execve", "parent");
    run_file_op("clone_non_thread_execve", "parent");

    waitpid(pid, NULL, 0);
    free(stack);
    return 0;
}
