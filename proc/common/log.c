#include "log.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

void log_proc_info(const char *command_name, const char *proc_desc) {
    pid_t pid  = getpid();
    pid_t tgid = pid;  // TGID == PID in Linux process model
    pid_t tid  = (pid_t)syscall(SYS_gettid);
    pid_t pgid = getpgid(0);
    pid_t ppid = getppid();

    fprintf(stderr,
            "[%s/%s] pid=%d tgid=%d tid=%d pgid=%d ppid=%d\n",
            command_name, proc_desc, pid, tgid, tid, pgid, ppid);
}
