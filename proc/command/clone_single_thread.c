#define _GNU_SOURCE
#include "clone_single_thread.h"
#include "common/file_op.h"
#include "common/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <semaphore.h>
#include <sys/types.h>

#define CHILD_STACK_SIZE (1024 * 1024)

static sem_t thread_done;

static int thread_fn(void *arg) {
    (void)arg;
    log_proc_info("clone_single_thread", "child");
    run_file_op("clone_single_thread", "child");
    sem_post(&thread_done);
    return 0;
}

int cmd_clone_single_thread(void) {
    sem_init(&thread_done, 0, 0);

    void *stack = malloc(CHILD_STACK_SIZE);
    if (!stack) {
        perror("malloc");
        return -1;
    }
    void *stack_top = (char *)stack + CHILD_STACK_SIZE;

    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;
    pid_t tid = clone(thread_fn, stack_top, flags, NULL);
    if (tid < 0) {
        perror("clone");
        free(stack);
        return -1;
    }

    log_proc_info("clone_single_thread", "parent");
    run_file_op("clone_single_thread", "parent");

    sem_wait(&thread_done);
    free(stack);
    sem_destroy(&thread_done);
    return 0;
}
