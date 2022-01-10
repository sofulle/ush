// exec.c

#include "ush.h"

int exec_exit(app_t *app, char **args) {
    args = NULL;
    app->is_running = false;

    return 0;
}

int exec_echo(app_t *app, char **args) {
    app = NULL;
    bool is_first = true;
    int args_count = 0;
    int arg_len = 0;
    bool is_no_newline = false;
    bool is_escape = false;
    int keys_count = 0;

    for(; args[args_count] != NULL; args_count++);

    if(args_count > 2) {
        arg_len = strlen(args[1]);

        if(args[1][0] == '-' && arg_len == 2) {
            switch (args[1][1]) {
                case 'n': {
                    is_no_newline = true;
                    keys_count++;
                    break;
                }
                case 'e': {
                    is_escape = true;
                    keys_count++;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

    for (int32_t i = keys_count + 1; args[i] != NULL; i++) {
        char *str = NULL;

        if(is_escape) str = str_replace_escape(args[i]);
        else str = args[i];

        if(is_first) is_first = false;
        else printf(" ");

        int ret = printf("%s", str);
        if(ret < 0) return ret;

        if(is_escape) free(str);
    }
    if(!is_no_newline) printf("\n");
    
    return 0;
}

int exec_setvar(app_t *app, char **args) {
    char **splited = NULL;
    char *name = NULL;
    char *value = NULL;
    int count = 0;
    int splited_count = 0;
    bool is_empty = false;

    for(; args[count] != NULL; count++);  

    if (count > 2) {
        mx_printerr("ush: too many options\n");
        return -1;
    }
    
    splited = mx_strsplit(args[0], '=');
    if (splited == NULL) {
        mx_printerr("ush: split wrong\n");
        return -1;
    }

    for(; splited[splited_count] != NULL; splited_count++);
    if (splited_count > 2 || splited_count < 1) {
        mx_printerr("ush: bad options\n");
        return -1;
    }

    if(!is_valid_str(splited[0])) {
        mx_printerr("ush: bad option: ");
        mx_printerr(splited[0]);
        mx_printerr("\n");
        return -1;
    }

    name = splited[0];

    if(splited_count == 2) {
        value = splited[1];
        if (count > 1) {
            mx_printerr("ush: too many options\n");
            return -1;
        }
        is_empty = false;
    }
    else if(args[1] != NULL) {
        value = args[1];
        is_empty = false;
    }
    else is_empty = true;


    if (is_empty) {
        if(getenv(name) != NULL) {
            if(setenv(name, "", true) == -1) {
                mx_printerr("ush: not enough memory\n");
                return -1;
            }
        }
        else var_set(app, name, "");
    }
    else {
        if(getenv(name) != NULL) {
            if(setenv(name, value, true) == -1) {
                mx_printerr("ush: not enough memory\n");
                return -1;
            }
        }
        else var_set(app, name, value);
    }

    mx_del_strarr(&splited);
    return 0;
}

int exec_export(app_t *app, char **args) {
    int count = 0;

    for(; args[count] != NULL; count++);
    if (count < 2) {
        mx_printerr("export: bad options\n");
        return -1;
    }

    for (int i = 1; i < count; i++) {
        char **splited = NULL;
        char *name = NULL;

        if(mx_get_char_index(args[i], '=') != -1) {
            char *args_buffer[2];
            args_buffer[0] = args[i];
            args_buffer[1] = NULL;

            if (exec_setvar(app, args_buffer) != 0) return -1;

            splited = mx_strsplit(args[i], '=');
            name = splited[0];
        }
        else if(!is_valid_str(args[i])) {
            mx_printerr("export: bad option: ");
            mx_printerr(args[i]);
            mx_printerr("\n");
            return -1;
        }
        else name = args[i];

        if(getenv(name) != NULL) continue;

        if(var_get(app, name) == NULL) continue;

        setenv(name, var_get(app, name), true);

        mx_del_strarr(&splited);
    }

    return 0;
}

int exec_unset(app_t *app, char **args) {
    int count = 0;

    for(; args[count] != NULL; count++);
    if (count < 2) {
        mx_printerr("unset: bad options\n");
        return -1;
    }

    for (int i = 1; i < count; i++) {
        if(!is_valid_str(args[i])) {
            mx_printerr("unset: bad option: ");
            mx_printerr(args[i]);
            mx_printerr("\n");
            return -1;
        }

        if(var_get(app, args[i]) != NULL) {
            var_unset(app, args[i]);
        }

        if(getenv(args[i]) != NULL) {
            unsetenv(args[i]);
        }
    }
    
    return 0;
}

int exec_jobs(app_t *app, char **args) {
    args = NULL;
    int id = 0;

    for(vector_t *node = app->processes; node != NULL; node = node->next, id++) {
        process_t *process = (process_t *)node->data;

        printf("[%d]  ", id + 1);
        if(process->is_last) printf("+");
        else printf(" ");
        printf(" suspended  %s\n", process->name);
    }

    return 0;
}

int exec_fg(app_t *app, char **args) {
    process_t *process = NULL;
    int id = 0;
    int status = 0;
    int args_count = 0;

    for(; args[args_count] != NULL; args_count++);
    if (args_count > 2 || args_count < 1) {
        mx_printerr("fg: too many options\n");
        return -1;
    }

    if (args_count == 1) {
        for(vector_t *node = app->processes; node != NULL; node = node->next, id++) {
            process_t *p = (process_t *)node->data;
            if(p->is_last) {
                process = p;
            }
        }
    }
    else {
        id = atoi(args[1]) - 1;
        vector_t *node = vector_get(&app->processes, id);
        if(node == NULL) {
            mx_printerr("fg: job not found: ");
            mx_printerr(args[1]);
            mx_printerr("\n");
            return -1;
        }
        process = (process_t *)node->data;
    }

    if(process == NULL) {
        mx_printerr("fg: no current job\n");
        return -1;
    }

    printf("{%d}\n", kill(process->pid, SIGCONT));
    waitpid(process->pid, &status, WUNTRACED);

    if (WIFSTOPPED(status)) {
        printf("\n[%d]  + %d suspended  %s\n", id + 1, process->pid, process->name);
    }
    else {
        vector_delete(&app->processes, id);

        bool is_last = false;
        for(vector_t *node = app->processes; node != NULL; node = node->next) {
            process_t *p = (process_t *)node->data;
            if(p->is_last) {
                is_last = true;
                break;
            }
        }
        if(!is_last) ((process_t *)app->processes->tail->data)->is_last = true;
    }

    return status;
}

int exec_pwd(app_t *app, char **args) {
    app = NULL;
    int32_t args_count = 0;
    int32_t arg_len = 0;
    bool is_physical = false;
    char *dir = NULL;

    for(; args[args_count] != NULL; args_count++);
    if (args_count > 2) {
        mx_printerr("pwd: too many options\n");
        return -1;
    }
    else if(args_count == 2) {
        arg_len = strlen(args[1]);

        if(args[1][0] != '-' && arg_len > 1) {
            mx_printerr("pwd: bad option: ");
            mx_printerr(args[1]);
            mx_printerr("\n");
            return -1;
        }
        switch (args[1][1]) {
            case 'L': {
                is_physical = false;
                break;
            }
            case 'P': {
                is_physical = true;
                break;
            }
            default: {
                mx_printerr("pwd: bad option: ");
                mx_printerr(args[1]);
                mx_printerr("\n");
                return -1;
            }
        }
    }

    if(is_physical) dir = realpath(getenv("PWD"), NULL);
    else dir = getenv("PWD");

    printf("%s\n", dir);

    if(is_physical) free(dir);
    return 0;
}

int exec_cd(app_t *app, char **args) {
    app = NULL;
    int32_t args_count = 0;
    char *new_path = NULL;
    DIR *dir = NULL;

    for(; args[args_count] != NULL; args_count++);
    if (args_count > 2) {
        mx_printerr("cd: too many options\n");
        return -1;
    }

    if(strcmp(args[1], "-") == 0) new_path = getenv("OLDPWD");
    else new_path = normalize_path(args[1]);
    dir = opendir(new_path);

    if(dir == NULL) {
        mx_printerr("cd: no such file or directory: ");
        mx_printerr(args[1]);
        mx_printerr("\n");
        return -2;
    }

    chdir(new_path);
    setenv("OLDPWD", getenv("PWD"), true);
    setenv("PWD", new_path, true);

    closedir(dir);
    if(strcmp(args[1], "-") != 0) free(new_path);
    return 0;
}

int exec_which(app_t *app, char **args) {
    int32_t args_count = 0;
    int32_t arg_len = 0;
    int32_t start = 0;
    int32_t ret = 0;
    char **paths = NULL;
    bool is_all = false;
    bool is_physical = false;

    for(; args[args_count] != NULL; args_count++);
    if (args_count < 2) {
        return 1;
    }

    arg_len = strlen(args[1]);

    if(args[1][0] == '-' && arg_len == 2) {
        switch (args[1][1]) {
            case 'a': {
                is_all = true;
                break;
            }
            case 's': {
                is_physical = true;
                break;
            }
            default: {
                mx_printerr("which: bad option: ");
                mx_printerr(args[1]);
                mx_printerr("\n");
                return -1;
            }
        }
        if(args_count > 2) start = 2;
        else return 1;
    }
    else {
        start = 1;
    }

    paths = mx_strsplit(getenv("PATH"), ':');
    if(paths == NULL) return 1;

    for (int32_t i = start; args[i] != NULL; i++) {
        bool is_exist = false;
        for (t_list *node = app->commands; node != NULL; node = node->next) {
            command_t *command = (command_t *)node->data;
            if(strcmp(args[i], command->name) == 0) {
                is_exist = true;
                printf("%s: shell built-in command\n", args[i]);
                break;
            }
        }

        if(is_exist && !is_all) continue;

        for (int32_t k = 0; paths[k] != NULL; k++) {
            DIR *dir = opendir(paths[k]);
            if(dir == NULL) continue;

            int32_t files_count = directory_files_count(paths[k], ENTRY_ALL);
            if(files_count < 0) continue;

            for (int n = 0; n < files_count; n++) {
                char *filename = readdir(dir)->d_name;
                if(strcmp(args[i], filename) == 0) {
                    is_exist = true;
                    if(is_physical) {
                        int32_t new_size = strlen(paths[k]) + strlen(args[i]) + 1;
                        char *full_path = mx_strnew(new_size);

                        strcat(full_path, paths[k]);
                        strcat(full_path, "/");
                        strcat(full_path, args[i]);

                        char *resolved_path = realpath(full_path, NULL);

                        if(strcmp(full_path, resolved_path) == 0) {
                            printf("%s\n", full_path);
                        }
                        else {
                            printf("%s -> %s\n", full_path, resolved_path);
                        }

                        free(resolved_path);
                        free(full_path);
                    }
                    else {
                        printf("%s/%s\n", paths[k], args[i]);
                    }
                    break;
                }
            }
            closedir(dir);

            if(is_exist && !is_all) break;
        }

        if(!is_exist) {
            printf("%s not found\n", args[i]);
            ret = 1;
        }
    }
    
    mx_del_strarr(&paths);
    return ret;
}
