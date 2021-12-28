#include "ush.h"

int fg(t_proc **pids) {
    t_proc *prev;
    // t_proc *current;
    int status;

    // prev = NULL;
    // current = *pids;
    // while (current) {
    //     if (kill(current->pid, 0) == -1)
    //         if (errno == ESRCH)

    // }
    if (*pids) {
        // if (split[0]) {
        //     prev = NULL;
        //     current = *pids;
        //     while (current) {
        //         current = *pids;
        //         if (prev->pid == strtoimax(split[0], NULL, 10))
        //             prev = current;
        //     }
        // } else {
        prev = (*pids)->prev;
        kill((*pids)->pid, SIGCONT); // resume child process
        waitpid((*pids)->pid, &status, WUNTRACED);
        free(*pids);
        *pids = prev;
        // }
    } else {
        // mx_printerr("ush: fg: current: no such job\n");
    }
    return 0;
}
