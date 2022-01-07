// utils.c

#include "ush.h"

void prompt_print(app_t *app) {
    printf("%s", var_get(app, "PROMPT"));
}

int32_t get_args_count(char *str) {
    int32_t count = 0;
    int32_t i = 0;
    bool state = true;
    bool is_open = false;
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

int32_t *get_args_lengths(char *str, int32_t count) {
    int32_t *lengths = NULL;
    int32_t index = 0;
    int32_t i = 0;
    bool state = true;
    bool old_state = false;
    bool is_open = false;
    char c;

    if(str == NULL || count < 1) return NULL;

    lengths = (int32_t *) malloc(count * sizeof(int32_t));
    memset(lengths, 0, count * sizeof(int32_t));
    if(lengths == NULL) return NULL;
    
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
        }

        if(state && !old_state) {
            index++;
        }

        lengths[index]++;

        printf("%c  %d  %d\n", str[i], state, lengths[index]);

        i++;
        old_state = state;
    }

    //if(count == 1) lengths[0] = mx_strlen(str);

    if(count > 1) {
        for (int32_t i = 1; i < count; i++) {
            lengths[i]--;
        }
    }

    for (int32_t i = 0; i < count; i++) {
        printf("%d: >%d$\n", i, lengths[i]);
    }

    return lengths;
}

char **get_args(char *str, int32_t count, int32_t *lengths) {
    int32_t index = 0;
    int32_t i = 0, j = 0;
    bool state = true;
    bool old_state = false;
    bool is_open = false;
    char c;
    char **args = NULL;

    if(str == NULL || count < 0) return NULL;

    args = (char **) malloc((count + 1) * sizeof(char *));
    if(args == NULL) return NULL;
    args[count] = NULL;
    for (int32_t i = 0; i < count; i++) {
        args[i] = (char *) malloc((lengths[i] + 1) * sizeof(char));
        if(args[i] == NULL) return NULL;
        args[i][lengths[i]] = '\0';
    }
    
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
        }

        if(state && !old_state) {
            index++;
            j = 0;
        }

        args[index][j] = str[i];

        i++;
        j++;

        old_state = state;
    }

    // for (int32_t i = 0; i < count; i++) {
    //     printf("%d: [%d]>%s$\n", i, lengths[i], args[i]);
    // }

    return args;
}

int32_t clean_args(char ***args, int32_t count) {
    char **clear_args = NULL;

    if(args == NULL || count < 0) return -1;

    clear_args = (char **) malloc((count + 1) * sizeof(char *));
    if(clear_args == NULL) return -1;
    clear_args[count] = NULL;

    for (int32_t i = 0; i < count; i++) {
        char *temp = mx_strtrim(args[0][i]);
        if(temp == NULL) return -1;

        clear_args[i] = strtrim_quotes(temp);
        if(clear_args[i] == NULL) return -1;

        //printf("[%d] `%s\' -> `%s\' -> `%s\'\n", i, args[0][i], temp, clear_args[i]);

        free(args[0][i]);
        if(temp != clear_args[i]) free(temp);
    }
    free(*args);

    *args = clear_args;
    
    return 0;
}

char *strtrim_quotes(char *str) {
    int32_t size = strlen(str);
    int32_t new_size = size - 2;
    char *ret = NULL;

    if(str == NULL) return NULL;

    if(str[0] != '\'' && str[0] != '\"' && str[0] != '`') return str;

    ret = (char *) malloc((new_size + 1) * sizeof(char));
    if(ret == NULL) return NULL;
    ret[new_size] = '\0';

    for (int32_t i = 0, j = 1; i < new_size; i++, j++) {
        ret[i] = str[j];
    }
    
    return ret;
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
