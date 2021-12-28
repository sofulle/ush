#include "ush.h"
#include <unistd.h>

void which(char **split) {
    int i;
    int j;
    int k;
    int l;
    short flags;
    char **s;
    char *path;
    struct dirent *dirent_new;
    DIR *dir;
    // char unresolved[FILENAME_MAX + 1];
    char resolved[FILENAME_MAX + 1];
    const char *builtins[106] = {
        "alias",         "autoload",  "bg",
        "bindkey",       "break",     "builtin",
        "bye",           "cap",       "cd",
        "chdir",         "clone",     "command",
        "comparguments", "compcall",  "compctl",
        "compdescribe",  "compfiles", "compgroups",
        "compquote",     "comptags",  "comptry",
        "compvalues",    "continue",  "declare",
        "dirs",          "disable",   "disown",
        "echo",          "echotc",    "echoti",
        "emulate",       "enable",    "eval",
        "exec",          "exit",      "export",
        "false",         "fc",        "fg",
        "float",         "functions", "getcap",
        "getln",         "getopts",   "hash",
        "history",       "integer",   "jobs",
        "kill",          "let",       "limit",
        "local",         "log",       "logout",
        "noglob",        "popd",      "print",
        "printf",        "pushd",     "pushln",
        "pwd",           "r",         "read",
        "readonly",      "rehash",    "return",
        "sched",         "set",       "setcap",
        "setopt",        "shift",     "source",
        "stat",          "suspend",   "test",
        "times",         "trap",      "true",
        "ttyctl",        "type",      "typeset",
        "ulimit",        "umask",     "unalias",
        "unfunction",    "unhash",    "unlimit",
        "unset",         "unsetopt",  "vared",
        "wait",          "whence",    "where",
        "which",         "zcompile",  "zformat",
        "zftp",          "zle",       "zmodload",
        "zparseopts",    "zprof",     "zpty",
        "zregexparse",   "zsocket",   "zstyle",
        "ztcp"};

    flags = 0;
    i = 0;
    while (true) {
        j = 0;
        if (split[i][j++] == '-') {
            while (split[i][j]) {
                switch (split[i][j]) {
                case 'a':
                    flags |= flag_a;
                    break;
                case 's':
                    flags |= flag_s;
                    break;
                default:
                    break;
                }
                j++;
            }
        } else {
            break;
        }
        i++;
    }

    for (j = 0; j < 106; j++) {
        if (!strcmp(split[i], builtins[j])) {
            printf("%s: shell built-in command\n", split[i]);
            flags |= boolean;
        }
    }

    s = environ;
    j = 0;
    path = NULL;
    while (s[j]) {
        if (strncmp(s[j], "PATH=", 5) == 0) {
            path = strdup(&s[j][5]);
            break;
        }
        j++;
    }
    if (!path) {
        return;
    }

    k = 0;
    j = 0;
    while (j != 6000000 && path[k]) {
        j = 0;
        while (path[k + j] && path[k + j] != ':')
            j++;
        if (path[k + j] == '\0')
            j = 6000000;
        else
            path[k + j] = '\0';
        if (!(dir = opendir(&path[k]))) {
            printf("opendir failed on: %s", &path[k]);
        } else if (!(flags & boolean) || flags & flag_a) {
            while ((dirent_new = readdir(dir)) != NULL) {
                if (!(mx_strcmp(dirent_new->d_name, split[i]))) {
                    if (flags & flag_s) {
                        l = readlink(&path[k], resolved, FILENAME_MAX);
                        if (l > 0) {
                            resolved[l] = '\0';
                            printf("%s/%s -> %s/%s\n", &path[k],
                                   split[i], resolved, split[i]);
                        } else {
                            printf("%s/%s\n", &path[k], split[i]);
                        }
                    } else {
                        printf("%s/%s\n", &path[k], split[i]);
                    }
                    flags |= boolean;
                    break;
                }
            }
        }
        k += j + 1;
        closedir(dir);
    }
    if (!(flags & boolean))
        printf("%s not found\n", split[i]);
}
