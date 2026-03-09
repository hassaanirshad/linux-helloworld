#define _GNU_SOURCE
#include "clone_single_thread_execve.h"
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

static int thread_fn(void *arg) {
    (void)arg;
    log_proc_info("clone_single_thread_execve", "child");
    const char *path = "/tmp/proc_clone_single_thread_execve_child";
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "touch %s && rm %s", path, path);

    char *argv[] = { "/bin/sh", "-c", cmd, NULL };
    char *envp[] = { NULL };
    execve("/bin/sh", argv, envp);

    perror("execve");
    return 1;
}

int cmd_clone_single_thread_execve(void) {
    void *stack = malloc(CHILD_STACK_SIZE);
    if (!stack) {
        perror("malloc");
        return -1;
    }
    void *stack_top = (char *)stack + CHILD_STACK_SIZE;

    log_proc_info("clone_single_thread_execve", "parent");
    run_file_op("clone_single_thread_execve", "parent");

    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;
    pid_t tid = clone(thread_fn, stack_top, flags, NULL);
    if (tid < 0) {
        perror("clone");
        free(stack);
        return -1;
    }

    // pause() suspends the parent thread until execve in the child thread
    // replaces the entire process image, at which point this never returns
    pause();
    return 0;
}
