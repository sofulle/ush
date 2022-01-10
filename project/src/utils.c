// utils.c

#include "ush.h"

void prompt_print(app_t *app) {
    printf("%s\033[32;1m%s\033[0m $ ", var_get(app, "PROMPT"), getenv("PWD"));
}

int32_t get_args_count(char *str) {
    int32_t count = 0;
    bool state = true;
    bool old_state = true;
    bool is_open_quotes = false;
    bool is_first = true;
    char open_qoute = '\0';

    if (str == NULL) return -1;

    for (int32_t i = 0; str[i] != '\0'; i++) {
        if(mx_isspace(str[i]) && !is_open_quotes) {
            state = false;
        }
        else if(str[i] == '\"' || str[i] == '\'') {
            if(!is_open_quotes) {
                state = true;
                is_open_quotes = true;
                open_qoute = str[i];
                if(!is_first) old_state = false;
            }
            else if(str[i] == open_qoute) {
                state = false;
                is_open_quotes = false;
            }
        }
        else state = true;

        if(!old_state && state) count++;
        old_state = state;

        is_first = false;
    }

    if(is_open_quotes) {
        mx_printerr("Odd number of quotes.\n");
        return -1;
    }

    return count + 1;
}

int32_t *get_args_lengths(char *str, int32_t count) {
    int32_t *lengths = NULL;
    int32_t index = 0;
    bool state = true;
    bool old_state = true;
    bool is_open_quotes = false;
    bool is_first = true;
    char open_qoute = '\0';

    if(str == NULL || count < 1) return NULL;

    lengths = (int32_t *) malloc(count * sizeof(int32_t));
    memset(lengths, 0, count * sizeof(int32_t));
    if(lengths == NULL) return NULL;

    for (int32_t i = 0; str[i] != '\0'; i++) {
        if(mx_isspace(str[i]) && !is_open_quotes) {
            state = false;
        }
        else if(str[i] == '\"' || str[i] == '\'') {
            if(!is_open_quotes) {
                state = true;
                is_open_quotes = true;
                open_qoute = str[i];
                if(!is_first) old_state = false;
            }
            else if(str[i] == open_qoute) {
                state = false;
                is_open_quotes = false;
                lengths[index]++;
            }
        }
        else state = true;

        if(!old_state && state) index++;
        if(state) lengths[index]++;

        //// if(state) printf("[%c]  old state: %d  state: %d  index: %d  len: %d\n", str[i], old_state, state, index, lengths[index]);
        //// else printf("[%c]  old state: %d  state: %d  index: %d  len: -\n", str[i], old_state, state, index);

        old_state = state;
        is_first = false;
    }

    //// printf("count: %d\n", count);
    //// for (int32_t i = 0; i < count; i++) {
    ////     printf("%d: %d\n", i, lengths[i]);
    //// }

    return lengths;
}

char **get_args(char *str, int32_t count, int32_t *lengths) {
    int32_t index = 0;
    bool state = true;
    bool old_state = true;
    bool is_open_quotes = false;
    bool is_first = true;
    char open_qoute = '\0';
    char **args = NULL;

    if(str == NULL || count < 1) return NULL;

    args = (char **) malloc((count + 1) * sizeof(char *));
    if(args == NULL) return NULL;
    memset(args, 0, (count + 1) * sizeof(char *));

    for (int32_t i = 0; i < count; i++) {
        args[i] = (char *) malloc((lengths[i] + 1) * sizeof(char));
        if(args[i] == NULL) return NULL;
        memset(args[i], 0, (lengths[i] + 1) * sizeof(char));
    }

    for (int32_t i = 0, j = 0; str[i] != '\0'; i++, j++) {
        if(mx_isspace(str[i]) && !is_open_quotes) {
            state = false;
        }
        else if(str[i] == '\"' || str[i] == '\'') {
            if(!is_open_quotes) {
                state = true;
                is_open_quotes = true;
                open_qoute = str[i];
                if(!is_first) old_state = false;
            }
            else if(str[i] == open_qoute) {
                state = false;
                is_open_quotes = false;
                args[index][j] = str[i];
            }
        }
        else state = true;

        if(!old_state && state) {
            index++;
            j = 0;
        }
        if(state) args[index][j] = str[i];

        old_state = state;
        is_first = false;
    }

    //// for (int32_t i = 0; i < count; i++) {
    ////     printf("%d: [%d]>%s$\n", i, lengths[i], args[i]);
    //// }

    return args;
}

int32_t clean_args(char ***args, int32_t count) {
    char **clear_args = NULL;

    if(args == NULL || count < 0) return -1;

    clear_args = (char **) malloc((count + 1) * sizeof(char *));
    if(clear_args == NULL) return -1;
    memset(clear_args, 0, (count + 1) * sizeof(char *));

    for (int32_t i = 0; i < count; i++) {
        clear_args[i] = strtrim_quotes(args[0][i]);
        if(clear_args[i] == NULL) return -1;

        //// printf("[%d] `%s\' -> `%s\' -> `%s\'\n", i, args[0][i], temp, clear_args[i]);

        free(args[0][i]);
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

    if(str[0] != '\'' && str[0] != '\"' && str[0] != '`') return strdup(str);

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

char *normalize_path(const char * src) {
    char * res;
    size_t res_len;

    size_t src_len = strlen(src);

    const char * ptr = src;
    const char * end = &src[src_len];
    const char * next;

    if (src_len == 0 || src[0] != '/') {

        // relative path

        char pwd[PATH_MAX];
        size_t pwd_len;

        if (getcwd(pwd, sizeof(pwd)) == NULL) {
            return NULL;
        }

        pwd_len = strlen(pwd);
        res = malloc(pwd_len + 1 + src_len + 1);
        memcpy(res, pwd, pwd_len);
        res_len = pwd_len;
    } 
    else {
        res = malloc((src_len > 0 ? src_len : 1) + 1);
        res_len = 0;
    }

    for (ptr = src; ptr < end; ptr=next+1) {
        size_t len;
        next = memchr(ptr, '/', end-ptr);
        if (next == NULL) {
            next = end;
        }
        len = next-ptr;
        switch(len) {
            case 2: {
                if (ptr[0] == '.' && ptr[1] == '.') {
                    const char * slash = memrchr(res, '/', res_len);
                    if (slash != NULL) {
                        res_len = slash - res;
                    }
                    continue;
                }
                break;
            }
            case 1: {
                if (ptr[0] == '.') continue;
                break;
            }
            case 0:
                continue;
        }
        res[res_len++] = '/';
        memcpy(&res[res_len], ptr, len);
        res_len += len;
    }

    if (res_len == 0) {
        res[res_len++] = '/';
    }
    res[res_len] = '\0';
    return res;
}

char *str_replace_escape(char *str) {
    uint32_t size = strlen(str);
    uint32_t new_size = 0;
    uint32_t escape_count = 0;

    char *new_str = NULL;

    if(size < 2) return strdup(str);

    for (uint32_t i = 0; i < size - 1; i++) {
        if(str[i] == '\\' && (str[i + 1] == '\\' || str[i + 1] == 'a' || str[i + 1] == 'n' || str[i + 1] == 't' || str[i + 1] == 'v')) {
            escape_count++;
        }
    }

    new_size = size - escape_count;
    new_str = (char *) malloc((size + 1) * sizeof(char));
    if(new_str == NULL) return NULL;
    memset(new_str, 0, (size + 1) * sizeof(char));

    for (uint32_t i = 0, j = 0; i < new_size; i++, j++) {
        if(str[j] == '\\' && (str[j + 1] == '\\' || str[j + 1] == 'a' || str[j + 1] == 'n' || str[j + 1] == 't' || str[j + 1] == 'v')) {
            switch (str[j + 1]) {
                case '\\': {
                    new_str[i] = '\\';
                    break;
                }
                case 'a': {
                    new_str[i] = '\a';
                    break;
                }
                case 'n': {
                    new_str[i] = '\n';
                    break;
                }
                case 't': {
                    new_str[i] = '\t';
                    break;
                }
                case 'v': {
                    new_str[i] = '\v';
                    break;
                }
                default:
                    break;
            }
            j++;
        }
        else new_str[i] = str[j];
    }

    return new_str;
}

int directory_files_count(char *dir_path, entry_t entry) {
    DIR *dir = opendir(dir_path);
    int ret = 0;

    if(dir == NULL) {
        return -1;
    }

    for(struct dirent *file = readdir(dir); file != NULL; file = readdir(dir)) {
        switch (entry) {
            case ENTRY_DEFAULT: {
                if(file->d_name[0] != '.') ret++;
                break;
            }
            case ENTRY_HIDDEN: {
                if(mx_strcmp(file->d_name, ".") && mx_strcmp(file->d_name, "..")) ret++;
                break;
            }
            case ENTRY_ALL: {
                ret++;
                break;
            }
            default: {
                ret++;
                break;
            }
        }
    }

    closedir(dir);
    return ret;
}
