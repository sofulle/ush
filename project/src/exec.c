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

    for (char **arg = args + 1; *arg != NULL; arg++) {
        if(is_first) is_first = false;
        else printf(" ");

        int ret = printf("%s", *arg);

        if(ret < 0) return ret;
    }
    printf("\n");
    
    return 0;
}

int exec_setvar(app_t *app, char **args) {
    char **splited = NULL;
    int count = 0;
    int splited_count = 0;

    for(; args[count] != NULL; count++);

    if (count > 1) {
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


    if (splited_count == 1) {
        if(getenv(splited[0]) != NULL) {
            if(setenv(splited[0], "", true) == -1) {
                mx_printerr("setenv: not enough memory\n");
                return -1;
            }
        }
        else
            var_set(app, splited[0], "");
    }
    else {
        if(getenv(splited[0]) != NULL) {
            if(setenv(splited[0], splited[1], true) == -1) {
                mx_printerr("setenv: not enough memory\n");
                return -1;
            }
        }
        else
            var_set(app, splited[0], splited[1]);
    }

    mx_del_strarr(&splited);
    return 0;
}

int exec_export(app_t *app, char **args) {
    int count = 0;

    for(; args[count] != NULL; count++);
    if (count < 2) {
        mx_printerr("ush: bad options\n");
        return -1;
    }

    for (int i = 1; i < count; i++) {
        if(!is_valid_str(args[i])) {
            mx_printerr("ush: bad option: ");
            mx_printerr(args[i]);
            mx_printerr("\n");
            return -1;
        }

        if(getenv(args[i]) != NULL) continue;

        if(var_get(app, args[i]) == NULL) continue;

        setenv(args[i], var_get(app, args[i]), true);
    }

    return 0;
}

int exec_unset(app_t *app, char **args) {
    int count = 0;

    for(; args[count] != NULL; count++);
    if (count < 2) {
        mx_printerr("ush: bad options\n");
        return -1;
    }

    for (int i = 1; i < count; i++) {
        if(!is_valid_str(args[i])) {
            mx_printerr("ush: bad option: ");
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

    for(t_list *node = app->processes; node != NULL; node = node->next) {
        process_t *process = (process_t *)node->data;

        printf("[%d]  + suspended  %s\n", process->id, process->name);
    }

    return 0;
}

int exec_fg(app_t *app, char **args) {
    args = NULL;
    process_t *process = NULL;
    int status = 0;

    for(t_list *node = app->processes; node != NULL; node = node->next) {
        process = (process_t *)node->data;
    }

    if(process == NULL) {
        mx_printerr("fg: no current job\n");
        return -1;
    }

    kill(process->pid, SIGCONT);
    waitpid(process->pid, &status, WUNTRACED);

    if (WIFSTOPPED(status)) {
        printf("\n[%d]  + %d suspended  %s\n", process->id, process->pid, process->name);
    }
    else {
        int count = mx_list_size(app->processes);
        int i = 0;

        if(count == 1) {
            process = (process_t *)app->processes->data;
            free(process->name);
            free(app->processes->data);
            free(app->processes);
            app->processes = NULL;
        }
        else if(count > 1) {
            for(t_list *node = app->processes; node != NULL; node = node->next) {
                if(count - i == 2) {
                    process = (process_t *)node->next->data;
                    free(process->name);
                    free(node->next->data);
                    free(node->next);
                    node->next = NULL;
                }

                i++;
            }
        }
        process->status = 1;
    }

    return status;
}
