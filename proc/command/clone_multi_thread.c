#define _GNU_SOURCE
#include "clone_multi_thread.h"
#include "common/file_op.h"
#include "common/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <semaphore.h>
#include <sys/types.h>

#define CHILD_STACK_SIZE (1024 * 1024)

static sem_t threads_done;

static int thread_fn_1(void *arg) {
    (void)arg;
    log_proc_info("clone_multi_thread", "child_1");
    run_file_op("clone_multi_thread", "child_1");
    sem_post(&threads_done);
    return 0;
}

static int thread_fn_2(void *arg) {
    (void)arg;
    log_proc_info("clone_multi_thread", "child_2");
    run_file_op("clone_multi_thread", "child_2");
    sem_post(&threads_done);
    return 0;
}

int cmd_clone_multi_thread(void) {
    sem_init(&threads_done, 0, 0);

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

    log_proc_info("clone_multi_thread", "parent");
    run_file_op("clone_multi_thread", "parent");

    sem_wait(&threads_done);
    sem_wait(&threads_done);

    free(stack1);
    free(stack2);
    sem_destroy(&threads_done);
    return 0;
}
