#include "vfork.h"
#include "common/file_op.h"
#include "common/log.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int cmd_vfork(void) {
    pid_t pid = vfork();
    if (pid < 0) {
        perror("vfork");
        return -1;
    }

    if (pid == 0) {
        log_proc_info("vfork", "child");
        run_file_op("vfork", "child");
        _exit(0);
    }

    // parent resumes here only after child calls _exit()
    log_proc_info("vfork", "parent");
    run_file_op("vfork", "parent");

    waitpid(pid, NULL, 0);
    return 0;
}
