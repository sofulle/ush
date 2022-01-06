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
        mx_printerr("ush: too many options\n");
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

int exec_vector(app_t *app, char **args) {
    app->is_running = app->is_running;
    args = NULL;
    vector_t *v = vector_init(strdup("process 1"));

    vector_push_back(&v, strdup("process 2"));
    vector_push_back(&v, strdup("process 3"));
    vector_push_back(&v, strdup("process 4"));

    printf("\nINIT\n");
    for (vector_t *node = v; node != NULL; node = node->next) {
        printf("%d: [%p]  head: %p  tail: %p  prev: %p  next: %p  data: %s\n", 
        node->id, (void *)node, (void *)node->head, (void *)node->tail, (void *)node->prev, (void *)node->next, (char *)node->data);
    }
    
    vector_delete(&v, 0);

    printf("\n1DEL\n");
    for (vector_t *node = v; node != NULL; node = node->next) {
        printf("%d: [%p]  head: %p  tail: %p  prev: %p  next: %p  data: %s\n", 
        node->id, (void *)node, (void *)node->head, (void *)node->tail, (void *)node->prev, (void *)node->next, (char *)node->data);
    }

    vector_delete(&v, 2);
    vector_delete(&v, 1);

    printf("\n2DEL\n");
    for (vector_t *node = v; node != NULL; node = node->next) {
        printf("%d: [%p]  head: %p  tail: %p  prev: %p  next: %p  data: %s\n", 
        node->id, (void *)node, (void *)node->head, (void *)node->tail, (void *)node->prev, (void *)node->next, (char *)node->data);
    }

    vector_delete(&v, 0);

    printf("\nLAST\n");
    for (vector_t *node = v; node != NULL; node = node->next) {
        printf("%d: [%p]  head: %p  tail: %p  prev: %p  next: %p  data: %s\n", 
        node->id, (void *)node, (void *)node->head, (void *)node->tail, (void *)node->prev, (void *)node->next, (char *)node->data);
    }

    return 0;
}
