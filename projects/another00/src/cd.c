#include "ush.h"

int argless(char **split) {
    if (!split[0])
        chdir(getenv("HOME"));
    else if (split[0][0] == '~') {
        if (split[0][1] == '\0')
            chdir(getenv("HOME"));
        else {
            struct passwd *pw;
            pw = getpwnam(&split[0][1]);
            if (pw)
                chdir(pw->pw_dir);
            else {
                mx_printerr("zsh: no such user or named directory: ");
                mx_printerr(&split[0][1]);
                mx_printerr("\n");
            }
        }
    } else if (chdir(split[0]) != 0)
        perror("chdir() to split[0] failed");
    return 0;
}

int cd(char **split, char ceedee[2][FILENAME_MAX]) {

    if (split[0][0] == '-') {
        switch (split[0][1]) {
        case '\0':
            chdir(ceedee[0]);
            break;
        case 's': {
            struct stat buf;
            char *str;
            int i;
            bool islink;

            islink = false;
            str = mx_strdup(split[1]);
            lstat(str, &buf);
            if (S_ISLNK(buf.st_mode))
                break;

            i = strlen(str) - 1;
            while (i) {
                if (str[i] == '/') {
                    str[i] = '\0';
                    lstat(str, &buf);
                    if (S_ISLNK(buf.st_mode)) {
                        islink = true;
                        break;
                    }
                }
            }
            if (!islink)
                argless(&split[1]);
        } break;
        case 'P': {
            argless(&split[1]);
            break; // TODO
        }
        default:
            chdir(split[0]);
            break;
        }
    } else
        argless(&split[0]);

    strncpy(ceedee[0], ceedee[1], FILENAME_MAX);
    getcwd(ceedee[1], FILENAME_MAX);
    return 0;
}
