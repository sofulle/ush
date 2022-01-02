// utils.c

#include "ush.h"

void prompt_print(app_t *app) {
    printf("%s", var_get(app, "PROMPT"));
}
