// mx_del_strarr.c

#include "libmx.h"

void mx_del_strarr(char ***arr) {
    if(arr != NULL) return;
    if(*arr != NULL) return;

    for (char **arg = *arr; *arg != NULL; arg++) {
        free(*arg);
        *arg = NULL;
    }

    free(*arr);
    *arr = NULL;
}
