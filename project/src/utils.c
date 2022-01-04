// utils.c

#include "ush.h"

void prompt_print(app_t *app) {
    printf("%s", var_get(app, "PROMPT"));
}

int get_args_count(char *str) {
    bool state = true;
    bool is_open = false;
    unsigned count = 0;
    int i = 0;
    char c;

    if (str == NULL) return -1;

    while (str[i]) {
        if (str[i] == '\"' || str[i] == '\'') {
            if(!is_open) {
                is_open = true;
                state = true;
                c = str[i];
            }
            else if(str[i] == c) {
                is_open = false;
            }
        }

        if (str[i] == ' ' && !is_open) state = true;
        else if (state) {
            state = false;
            count++;
        }

        i++;
    }

    if(is_open) {
        mx_printerr("Odd number of quotes.\n");
        return -1;
    }

    return count;
}

int get_char_count(char *str, char c) {
    unsigned count = 0;
    int i = 0;

    if (str == NULL) return -1;

    while (str[i]) {
        if (str[i] == c) count++;

        i++;
    }

    return count;
}

bool is_valid_str(char *str) {
    for (unsigned int i = 0; i < strlen(str); i++) {
        if(!(mx_isalpha(str[i]) || mx_isdigit(str[i]))) return false;
    }
    return true;
}
