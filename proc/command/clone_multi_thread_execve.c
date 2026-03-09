#define _GNU_SOURCE
#include "clone_multi_thread_execve.h"
#include "common/file_op.h"
#include "common/log.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <semaphore.h>
#include <sys/types.h>

#define CHILD_STACK_SIZE (1024 * 1024)

static sem_t children_done;

static int thread_fn_1(void *arg) {
    (void)arg;
    log_proc_info("clone_multi_thread_execve", "child_1");
    run_file_op("clone_multi_thread_execve", "child_1");
    sem_post(&children_done);
    return 0;
}

static int thread_fn_2(void *arg) {
    (void)arg;
    log_proc_info("clone_multi_thread_execve", "child_2");
    run_file_op("clone_multi_thread_execve", "child_2");
    sem_post(&children_done);
    return 0;
}

int cmd_clone_multi_thread_execve(void) {
    sem_init(&children_done, 0, 0);

    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;

    void *stack1 = malloc(CHILD_STACK_SIZE);
    if (!stack1) {
        perror("malloc");
        return -1;
    }

    void *stack2 = malloc(CHILD_STACK_SIZE);
    if (!stack2) {
        perror("malloc");
        free(stack1);
        return -1;
    }

    pid_t tid1 = clone(thread_fn_1, (char *)stack1 + CHILD_STACK_SIZE, flags, NULL);
    if (tid1 < 0) {
        perror("clone tid1");
        free(stack1);
        free(stack2);
        return -1;
    }

    pid_t tid2 = clone(thread_fn_2, (char *)stack2 + CHILD_STACK_SIZE, flags, NULL);
    if (tid2 < 0) {
        perror("clone tid2");
        free(stack1);
        free(stack2);
        return -1;
    }

    // wait for both threads to finish their file ops
    sem_wait(&children_done);
    sem_wait(&children_done);

    log_proc_info("clone_multi_thread_execve", "parent");
    const char *path = "/tmp/proc_clone_multi_thread_execve_parent";
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "touch %s && rm %s", path, path);

    char *argv[] = { "/bin/sh", "-c", cmd, NULL };
    char *envp[] = { NULL };
    execve("/bin/sh", argv, envp);

    perror("execve");
    return 1;
}
