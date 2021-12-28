#include "ush.h"
int strcomp(const void *a, const void *b) {
    char const *char_a = *(char const **)a;
    char const *char_b = *(char const **)b;

    return strcmp(char_a, char_b);
}
// TODO importing a defined variable like that VAR=val; export VAR
int export(char **split) {
    char **s = environ;
    int i;
    int j;

    if (split[0]) {
        j = 0;
        while (split[j]) {
            i = 0;
            while (split[j][i]) {
                if (split[j][i] == '=') {
                    split[j][i] = '\0';
                    setenv(split[j], &split[j][i + 1], true);
                }
                i++;
            }
            j++;
        }
    } else {
        i = 0;
        while (s[i++])
            ;
        i--;
        qsort(s, i, sizeof(char *), strcomp);
        for (i = 0; s[i + 1]; i++) {
            printf("%s\n", s[i]);
        }
    }

    return 0;
}
