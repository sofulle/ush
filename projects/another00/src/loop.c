#include "ush.h"

int cycle(char *splat, t_proc *pids, char wd[2][FILENAME_MAX]) {
    char **split;
    // char *spleet = strdup(splat);

    split = get_split(splat, " \t\r\n\a");

    /*  */ if (strncmp(split[0], "export", 7) == 0) { // unfinished?
        export(&split[1]);
    } else if (strncmp(split[0], "unset", 6) == 0) {
        unset(&split[1]);
    } else if (strncmp(split[0], "exit", 5) == 0) {
        return -1;
    } else if (strncmp(split[0], "fg", 3) == 0) {
        fg(&pids);
    } else if (strncmp(split[0], "env", 4) == 0) {
        puts("!!NYI!!");
    } else if (strncmp(split[0], "cd", 3) == 0) {
        /* how to change dir to a soft link??*/
        cd(&split[1], wd);
    } else if (strncmp(split[0], "pwd", 4) == 0) {
        getcwd(wd[1], FILENAME_MAX);
        printf("%s\n", wd[1]);
    } else if (strncmp(split[0], "which", 6) == 0) {
        which(&split[1]); // no -s flag
    } else if (strncmp(split[0], "echo", 5) == 0) {
        echo(&split[1]);
    } else {
        launch(split, &pids);
    }
    return 0;
}

int loop(void) {
    char *line;
    char **split;
    char wd[2][FILENAME_MAX];
    t_proc *pids;
    int i;

    getcwd(wd[0], FILENAME_MAX);
    getcwd(wd[1], FILENAME_MAX);
    pids = NULL;

    if (!isatty(STDIN_FILENO)) {

        line = get_line();
        if (line[0] == '\n' && line[1] == '\0') {
            free(line);
            return 0;
        }
        split = get_split(line, ";");
        for (i = 0; split[i]; i++) {
            if (cycle(split[i], pids, wd) == -1) {
                i = 0;
                break;
            }
        }
        return 0;
    }
    i = 1;
    while (i) {
        printf("%s", "u$h> ");
        line = get_line();
        if (line[0] == '\n') {
            free(line);
            i = 1;
            continue;
        }
        split = get_split(line, ";");
        for (i = 0; split[i]; i++) {
            if (cycle(split[i], pids, wd) == -1) {
                i = 0;
                break;
            }
        }
    }
    return 0;
}
