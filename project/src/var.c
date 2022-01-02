// var.c

#include "ush.h"

void var_set(app_t *app, char *name, char *value) {
    var_t *var = var_search(app, name);

    if(var == NULL) {
        var_t *var = malloc(sizeof(var_t));

        var->name = strdup(name);
        var->value = strdup(value);

        if(app->vars == NULL)
            app->vars = mx_create_node(var);
        else
            mx_push_back(&app->vars, var);
    }
    else {
        free(var->value);
        var->value = strdup(value);
    }
}

char *var_get(app_t *app, char *name) {
    for(t_list *node = app->vars; node != NULL; node = node->next) {
        var_t *var = (var_t *)node->data;

        if(!strcmp(name, var->name)) return var->value;
    }

    return NULL;
}

var_t *var_search(app_t *app, char *name) {
    for(t_list *node = app->vars; node != NULL; node = node->next) {
        var_t *var = (var_t *)node->data;

        if(!strcmp(name, var->name)) return var;
    }

    return NULL;
}
