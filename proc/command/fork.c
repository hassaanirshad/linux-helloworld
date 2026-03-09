#include "fork.h"
#include "common/file_op.h"
#include "common/log.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int cmd_fork(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        log_proc_info("fork", "child");
        run_file_op("fork", "child");
        _exit(0);
    }

    log_proc_info("fork", "parent");
    run_file_op("fork", "parent");
    waitpid(pid, NULL, 0);
    return 0;
}
