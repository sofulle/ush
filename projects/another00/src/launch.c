#include "ush.h"

int launch(char **split, t_proc **pids) {
    pid_t pid;
    t_proc *proc;
    // pid_t wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        signal(SIGTSTP, SIG_DFL);
        if (execvp(split[0], split) == -1) {
            mx_printerr("ush: command not found: ");
            mx_printerr(split[0]);
            mx_printerr("\n");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("ush");
    } else {
        proc = malloc(sizeof *proc);
        proc->pid = pid;
        proc->prev = *pids;
        *pids = proc;
        waitpid(pid, &status, WUNTRACED);
        if (WIFSTOPPED(status)) {
            printf("\nzsh: suspended ");
            for (status = 0; split[status]; split++)
                printf(" %s", split[status]);
            putchar('\n');
        }
        // printf("%d", status);
    }

    return 1;
}
